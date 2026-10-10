#!/usr/bin/env node
// MIT. Source-based lifting: emits executable C++ operations, not pseudocode.
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import { fileURLToPath } from 'node:url';

export const projectRoot = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const regs = new Set('AX CX DX BX SP BP SI DI ES CS SS DS AL CL DL BL AH CH DH BH'.split(' '));
const byteRegs = new Set('AL CL DL BL AH CH DH BH'.split(' '));
const identifiers = /^[A-Z_$?@][A-Z_0-9$?@]*$/i;
const branches = {
  JZ: 'm.flags.zero', JE: 'm.flags.zero', JNZ: '!m.flags.zero', JNE: '!m.flags.zero',
  JC: 'm.flags.carry', JB: 'm.flags.carry', JNAE: 'm.flags.carry',
  JNC: '!m.flags.carry', JAE: '!m.flags.carry', JNB: '!m.flags.carry',
  JBE: '(m.flags.carry || m.flags.zero)', JNA: '(m.flags.carry || m.flags.zero)',
  JA: '(!m.flags.carry && !m.flags.zero)', JNBE: '(!m.flags.carry && !m.flags.zero)',
  JS: 'm.flags.sign', JNS: '!m.flags.sign', JO: 'm.flags.overflow', JNO: '!m.flags.overflow',
  JP: 'm.flags.parity', JPE: 'm.flags.parity', JNP: '!m.flags.parity', JPO: '!m.flags.parity',
  JL: '(m.flags.sign != m.flags.overflow)', JNGE: '(m.flags.sign != m.flags.overflow)',
  JGE: '(m.flags.sign == m.flags.overflow)', JNL: '(m.flags.sign == m.flags.overflow)',
  JLE: '(m.flags.zero || m.flags.sign != m.flags.overflow)',
  JG: '(!m.flags.zero && m.flags.sign == m.flags.overflow)',
  JCXZ: '(m.get(Reg::CX) == 0)',
};
const strings = {
  LODSB: ['lods', 8], LODSW: ['lods', 16], STOSB: ['stos', 8], STOSW: ['stos', 16],
  MOVSB: ['movs', 8], MOVSW: ['movs', 16], CMPSB: ['cmps', 8], CMPSW: ['cmps', 16],
  SCASB: ['scas', 8], SCASW: ['scas', 16],
};
const scpAliases = {
  LODB: 'LODSB', LODW: 'LODSW', STOB: 'STOSB', STOW: 'STOSW',
  MOVB: 'MOVSB', MOVW: 'MOVSW', CMPB: 'CMPSB', CMPW: 'CMPSW',
  SCAB: 'SCASB', SCAW: 'SCASW', UP: 'CLD', DOWN: 'STD', JP: 'JMP', JMPS: 'JMP',
};
const supported = new Set([
  'MOV', 'LEA', 'ADD', 'ADC', 'SUB', 'SBB', 'CMP', 'AND', 'OR', 'XOR', 'TEST',
  'INC', 'DEC', 'NEG', 'NOT', 'SHL', 'SAL', 'SHR', 'SAR', 'ROL', 'ROR', 'RCL', 'RCR',
  'PUSH', 'POP', 'XCHG', 'CLC', 'STC', 'CMC', 'CLD', 'STD', 'NOP', 'CLI', 'STI',
  'CALL', 'RET', 'JMP', 'LOOP', 'LOOPE', 'LOOPZ', 'LOOPNE', 'LOOPNZ', 'INT', 'CBW', 'CWD', 'LAHF', 'SAHF', 'DAA', 'AAM', 'AAD', 'LDS', 'LES',
  ...Object.keys(branches), ...Object.keys(strings),
]);

export function sourceText(buffer) {
  // CP/M EOF and binary padding occur in the historical sources.
  return buffer.toString('latin1').split('\x1a')[0].split('\x00')[0];
}

export function stripComment(line) {
  let quote = '';
  for (let i = 0; i < line.length; i++) {
    const c = line[i];
    if (quote) {
      if (c === quote) {
        if (line[i + 1] === quote) i++;
        else quote = '';
      }
    } else if (c === '"' || c === "'") quote = c;
    else if (c === ';') return line.slice(0, i);
  }
  if (quote) throw new Error(`unterminated quoted string: ${line}`);
  return line;
}

export function splitOperands(text) {
  let quote = '', depth = 0, start = 0;
  const out = [];
  for (let i = 0; i < text.length; i++) {
    const c = text[i];
    if (quote) {
      if (c === quote) {
        if (text[i + 1] === quote) i++;
        else quote = '';
      }
    } else if (c === '"' || c === "'") quote = c;
    else if ('[('.includes(c)) depth++;
    else if ('])'.includes(c)) depth--;
    else if (c === ',' && depth === 0) {
      out.push(text.slice(start, i).trim());
      start = i + 1;
    }
    if (depth < 0) throw new Error('unbalanced operand delimiters');
  }
  if (quote || depth !== 0) throw new Error('unbalanced operand delimiters');
  if (text.slice(start).trim()) out.push(text.slice(start).trim());
  return out;
}

function tokens(text) {
  const out = [];
  let pos = 0;
  while (pos < text.length) {
    const rest = text.slice(pos);
    let match;
    if ((match = /^\s+/.exec(rest))) { pos += match[0].length; continue; }
    if (rest[0] === '"' || rest[0] === "'") {
      const quote = rest[0];
      let str = '', end = 1, closed = false;
      for (; end < rest.length; end++) {
        if (rest[end] === quote) {
          if (rest[end + 1] === quote) { str += quote; end++; }
          else { end++; closed = true; break; }
        } else str += rest[end];
      }
      if (!closed || str.length !== 1) throw new Error('expressions require single-byte character literals');
      out.push(String(str.charCodeAt(0))); pos += end; continue;
    }
    if ((match = /^(?:[0-9][0-9A-F]*H\b|[01]+B\b|[0-9]+\b|[A-Z_$?@][A-Z_0-9$?@]*\b|[()+\-*/~&|^])/i.exec(rest))) {
      out.push(match[0].toUpperCase()); pos += match[0].length; continue;
    }
    throw new Error(`unsupported expression near ${JSON.stringify(rest)}`);
  }
  return out;
}

// Deliberately not eval(): unknown names/syntax must stop a conversion.
export function expression(text, symbols = {}, allowRegisters = false) {
  if (Object.hasOwn(symbols, text.trim().toUpperCase()) && !regs.has(text.trim().toUpperCase()))
    return String(symbols[text.trim().toUpperCase()]);
  const ts = tokens(text);
  let i = 0;
  const precedence = { OR: 1, '|': 1, XOR: 2, '^': 2, AND: 3, '&': 3,
    SHL: 4, SHR: 4, '+': 5, '-': 5, '*': 6, '/': 6, MOD: 6 };
  function atom() {
    const token = ts[i++];
    if (!token) throw new Error('missing expression operand');
    if (token === '(') {
      const value = binary(0);
      if (ts[i++] !== ')') throw new Error('missing closing parenthesis');
      return `(${value})`;
    }
    if (['+', '-', '~', 'NOT'].includes(token)) return `(${token === 'NOT' ? '~' : token}${atom()})`;
    if (/^[0-9][0-9A-F]*H$/.test(token)) return String(parseInt(token.slice(0, -1), 16));
    if (/^[01]+B$/.test(token)) return String(parseInt(token.slice(0, -1), 2));
    if (/^\d+$/.test(token)) return token;
    if (allowRegisters && regs.has(token)) return `m.get(Reg::${token})`;
    if (Object.hasOwn(symbols, token)) return String(symbols[token]);
    throw new Error(`unresolved symbol ${token}`);
  }
  function binary(min) {
    let left = atom();
    while (Object.hasOwn(precedence, ts[i]) && precedence[ts[i]] >= min) {
      const operator = ts[i++], right = binary(precedence[operator] + 1);
      const cpp = { OR: '|', XOR: '^', AND: '&', SHL: '<<', SHR: '>>', MOD: '%' }[operator] ?? operator;
      left = `(${left} ${cpp} ${right})`;
    }
    return left;
  }
  const result = binary(0);
  if (i !== ts.length) throw new Error(`unexpected expression token ${ts[i]}`);
  return result;
}

// Evaluate only the AST that expression() accepted, using a second numeric parser.
export function constant(text, symbols = {}) {
  const cpp = expression(text, symbols);
  const ts = cpp.match(/\d+|<<|>>|[()+\-*/~&|^%]/g) ?? [];
  let index = 0;
  const prec = { '|': 1, '^': 2, '&': 3, '<<': 4, '>>': 4, '+': 5, '-': 5, '*': 6, '/': 6, '%': 6 };
  function atom() {
    const t = ts[index++];
    if (t === '(') { const n = binary(0); if (ts[index++] !== ')') throw new Error('bad constant'); return n; }
    if (t === '-') return -atom();
    if (t === '+') return atom();
    if (t === '~') return ~atom();
    if (!/^\d+$/.test(t ?? '')) throw new Error('bad constant');
    return Number(t);
  }
  function binary(min) {
    let n = atom();
    while (Object.hasOwn(prec, ts[index]) && prec[ts[index]] >= min) {
      const op = ts[index++], r = binary(prec[op] + 1);
      if ((op === '/' || op === '%') && r === 0) throw new Error('division by zero in constant');
      n = { '+': () => n + r, '-': () => n - r, '*': () => n * r, '/': () => Math.trunc(n / r),
        '%': () => n % r, '&': () => n & r, '|': () => n | r, '^': () => n ^ r,
        '<<': () => n << r, '>>': () => n >> r }[op]();
    }
    return n;
  }
  const n = binary(0);
  if (index !== ts.length || !Number.isSafeInteger(n)) throw new Error('constant outside supported integer range');
  return n;
}

function locate(lines, marker, from = 0) {
  const key = marker.trim().replace(/\s+/g, ' ').toUpperCase();
  const i = lines.findIndex((line, n) => n >= from && stripComment(line).trim().replace(/\s+/g, ' ').toUpperCase().startsWith(key));
  if (i < 0) throw new Error(`source boundary not found: ${marker}`);
  return i;
}

// Expand the actual reviewed mapper macros. Unsupported macro directives or
// include files remain errors; module mode never discards executable bodies.
function expandModule(text, config, dependencies) {
  const macros = new Map();
  for (const dependency of dependencies) {
    const lines = sourceText(dependency.bytes).split(/\r?\n/);
    for (let i=0;i<lines.length;i++) {
      const line=stripComment(lines[i]).trim();if(!line) continue;
      if (/^(IF1|ENDIF|%OUT\b)/i.test(line)) continue; // assembly listing only
      const declaration=/^([\w$?@]+)\s+MACRO(?:\s+(\w+))?$/i.exec(line);
      if(!declaration) throw new Error(`${dependency.source}:${i+1}: unsupported macro declaration`);
      const body=[];let ended=false;
      for(i++;i<lines.length;i++) {
        const value=stripComment(lines[i]).trim();
        if(/^ENDM$/i.test(value)) {ended=true;break;}
        if(value) body.push(value);
      }
      if(!ended) throw new Error('unterminated macro');
      const key=declaration[1].toUpperCase();
      if(macros.has(key)) throw new Error(`duplicate macro ${key}`);
      macros.set(key,{parameter:declaration[2],body});
    }
  }
  const expand=(line,depth=0)=>{
    if(depth>16) throw new Error('recursive macro expansion');
    const match=/^(?:(\w+:)\s*)?(\w+)(?:\s+(.*))?$/.exec(stripComment(line).trim());
    const macro=match && macros.get(match[2].toUpperCase());
    if(!macro) return [line];
    const argument=match[3]?.trim();
    if(Boolean(argument)!==Boolean(macro.parameter)) throw new Error(`invalid arguments to macro ${match[2]}`);
    const body=macro.body.flatMap(value=>expand(macro.parameter
      ? value.replace(new RegExp(`\\b${macro.parameter}\\b`,'gi'),argument) : value,depth+1));
    return match[1] ? [match[1],...body] : body;
  };
  const output=[],lineMap=[];
  text.split(/\r?\n/).forEach((line,index)=>{
    for(const expanded of expand(line)) {output.push(expanded);lineMap.push(index+1);}
  });
  return {lines:output,lineMap};
}

export function loadUnitDependencies(config, sourceRoot) {
  return (config.includes ?? []).map(source=>({source,bytes:fs.readFileSync(path.join(sourceRoot,source))}));
}

export function parseUnit(text, config, dependencies = []) {
  if(config.module && (config.sections || config.start || config.end)) throw new Error('complete module cannot select source sections');
  if(config.module && config.abi!=='far') throw new Error('module requires an explicit far ABI');
  const {lines,lineMap}=config.module ? expandModule(text,config,dependencies)
    : {lines:text.split(/\r?\n/),lineMap:null};
  if(config.sections && (config.start || config.end)) throw new Error('sections cannot be combined with start/end');
  const ranges=(config.sections ?? [config]).map(section=>{
    const start=section.start ? locate(lines,section.start) : 0;
    return {start,end:section.end ? locate(lines,section.end,start+1) : lines.length};
  });
  if(!ranges.length) throw new Error('empty section list');
  const start=Math.min(...ranges.map(r=>r.start)),end=Math.max(...ranges.map(r=>r.end));
  for(let a=0;a<ranges.length;a++) for(let b=0;b<a;b++)
    if(ranges[a].start<ranges[b].end && ranges[b].start<ranges[a].end) throw new Error('overlapping source sections');
  const symbols = {}, widths = {}, labels = {}, equates = [], data = [], code = [];
  // Resolve named field offsets from the actual MASM structure declaration.
  // No caller-supplied numeric offsets: changing the source changes the lift.
  const structureRanges=[];
  const structures=config.module ? lines.map(line=>/^([\w$?@]+)\s+STRUC$/i.exec(stripComment(line).trim())?.[1]).filter(Boolean)
    : config.structures ?? [];
  for(const structure of structures) {
    const begin=locate(lines,`${structure} STRUC`),finish=locate(lines,`${structure} ENDS`,begin+1);
    structureRanges.push({begin,finish});
    let offset=0;
    for(let n=begin+1;n<finish;n++) {
      const line=stripComment(lines[n]).trim();if(!line) continue;
      const field=/^(?:([A-Z_$?@][A-Z_0-9$?@]*)\s+)?(DB|DW|DD)\s+(\?|0H?|.+\s+DUP\s*\(\?\))$/i.exec(line);
      if(!field) throw new Error(`${config.source}:${n+1}: unsupported structure field`);
      const width={DB:8,DW:16,DD:32}[field[2].toUpperCase()];
      if(field[1]) {
        const name=field[1].toUpperCase();if(Object.hasOwn(symbols,name)) throw new Error(`duplicate structure field ${name}`);
        symbols[name]=offset;widths[name]=width;
      }
      const count=/^(\?|0H?)$/i.test(field[3]) ? 1 : constant(field[3].replace(/\s+DUP\s*\(\?\)$/i,''));
      if(count<0 || offset+count*width/8>65536) throw new Error('structure exceeds segment');
      offset+=count*width/8;
    }
    if(config.module) symbols[structure.toUpperCase()]=offset;
  }
  if (config.program) labels.__ENTRY = 0;
  let dataPos = config.data_origin ?? 0x2000;
  let segment = null, repeat = null;
  function bind(name) {
    name = name.toUpperCase();
    if (Object.hasOwn(labels, name)) throw new Error(`duplicate label ${name}`);
    labels[name] = code.length;
  }
  for (let i = start; i < end; i++) {
    if(config.module && structureRanges.some(({begin,finish})=>i>=begin && i<=finish)) continue;
    if(!ranges.some(r=>i>=r.start && i<r.end)) {
      if(repeat || segment) throw new Error('instruction prefix crosses source section boundary');
      continue;
    }
    let line = stripComment(lines[i]).trim();
    if (!line) continue;
    const sourceLine=lineMap ? lineMap[i] : i+1;
    const loc = `${config.source}:${sourceLine}`;
    try {
      let match;
      if(config.module) {
        if(/^(PAGE|TITLE|PUBLIC)\b|^\.(SALL|XLIST|LIST)$/i.test(line)) continue;
        // Declarations alone have no runtime effect. Any use of an external
        // symbol still fails resolution unless a real implementation is bound.
        if(/^EXTRN\s+[\w$?@]+:(BYTE|WORD|DWORD|NEAR|FAR)$/i.test(line)) continue;
        if(/^[\w$?@]+\s+(SEGMENT\b.*|ENDS|ENDP)$/i.test(line) || /^END$/i.test(line)) continue;
        if((match=/^INCLUDE\s+(\S+)$/i.exec(line))) {
          if(!dependencies.some(item=>path.basename(item.source).toUpperCase()===match[1].toUpperCase()))
            throw new Error(`unreviewed include ${match[1]}`);
          continue;
        }
        if((match=/^([\w$?@]+)\s+PROC\s+FAR$/i.exec(line))) {bind(match[1]);continue;}
      }
      if ((match = /^([A-Z_$?@][A-Z_0-9$?@]*)(?::\s*|\s+)(?:EQU|=)\s+(.+)$/i.exec(line))) {
        equates.push({ name: match[1].toUpperCase(), value: match[2].replaceAll('$', String(dataPos)) }); continue;
      }
      if ((match = /^PROCEDURE\s+([A-Z_$?@][A-Z_0-9$?@]*),NEAR$/i.exec(line))) {
        bind(match[1]); continue;
      }
      if ((match = /^([A-Z_$?@][A-Z_0-9$?@]*):\s*(.*)$/i.exec(line))) {
        const name = match[1].toUpperCase();
        line = match[2];
        let next = line;
        if (!next) {
          for (let j = i + 1; j < end; j++) {
            next = stripComment(lines[j]).trim();
            if (next) break;
          }
        }
        if (/^(DB|DW|DS|DM)\b/i.test(next)) symbols[name] = dataPos;
        else if (name === 'START' && config.program) symbols[name] = dataPos;
        else bind(name);
        if (!line) continue;
      }
      if (/^(DB|DW|DS|DM)\b/i.test(line)) {
        const directive = /^(DB|DW|DS|DM)\s+(.+)$/i.exec(line);
        if (!directive) throw new Error('missing data operand');
        const type = directive[1].toUpperCase();
        if (type === 'DM' && config.dialect !== 'scp') throw new Error('DM is an SCP directive');
        const names = Object.keys(symbols).filter(name => symbols[name] === dataPos);
        if (type === 'DS') {
          for (const eq of equates) { try { symbols[eq.name] = constant(eq.value, symbols); } catch {} }
          const size = constant(directive[2], symbols);
          if (size < 0 || dataPos + size > 0xff00) throw new Error('data exceeds 16-bit segment');
          for (const name of names) widths[name] = size === 1 ? 8 : size === 2 ? 16 : undefined;
          dataPos += size;
        } else {
          const width = type === 'DW' ? 16 : 8;
          for (const name of names) widths[name] = width;
          const previous = data.length;
          for (const item of splitOperands(directive[2])) {
            const str = /^(["'])(.*)\1$/.exec(item);
            if (str && width === 8) {
              for (const c of str[2].replaceAll(str[1] + str[1], str[1])) data.push([dataPos++, 8, c.charCodeAt(0)]);
            } else {
              data.push([dataPos, width, { expression: item, line: i + 1 }]);
              dataPos += width / 8;
            }
          }
          if (type === 'DM') {
            if (data.length === previous) throw new Error('empty DM declaration');
            const last = data.at(-1);
            if (typeof last[2] === 'object') last[2].mark = true;
            else last[2] |= 0x80;
          }
          if (dataPos > 0xff00) throw new Error('data exceeds 16-bit segment');
        }
        continue;
      }
      const opMatch = /^(\S+)(?:\s+(.*))?$/.exec(line);
      let op = opMatch[1].toUpperCase(), args = opMatch[2] ?? '';
      if (['ORG', 'PUT'].includes(op)) {
        if (!config.program) throw new Error('origin directives require a program unit');
        const origin = constant(args);
        if (origin !== 0x100) {
          // TRANS uses ORG 1 only to allocate numeric token equates, after all executable code/data.
          if (op !== 'ORG' || config.dialect !== 'scp' || origin !== 1) throw new Error('unsupported origin');
          dataPos = origin;
        }
        continue;
      }
      if (op === 'ASSUME') continue; // Static assembler annotation; operands still determine actual segment.
      if (op === 'SEG') {
        if (segment || !['ES', 'CS', 'DS', 'SS'].includes(args.toUpperCase())) throw new Error('invalid segment prefix');
        segment = args.toUpperCase(); continue;
      }
      if (['REP', 'REPE', 'REPZ', 'REPNE', 'REPNZ'].includes(op)) {
        if (repeat) throw new Error('duplicate repeat prefix');
        repeat = op;
        if (!args) continue;
        const next = /^(\S+)(?:\s+(.*))?$/.exec(args);
        op = next[1].toUpperCase(); args = next[2] ?? '';
      }
      if (config.dialect === 'scp') op = scpAliases[op] ?? op;
      if (!supported.has(op)) throw new Error(`unsupported instruction/directive ${op}`);
      if (repeat && !strings[op]) throw new Error('repeat prefix on non-string instruction');
      if (segment && !strings[op] && !args.includes('[')) throw new Error('segment prefix on non-memory instruction');
      code.push({ op, args, segment, repeat, line: sourceLine, raw: lines[i].trim() });
      segment = repeat = null;
    } catch (error) { throw new Error(`${loc}: ${error.message}`); }
  }
  if (repeat || segment) throw new Error('dangling instruction prefix');
  for (let pass = 0; pass <= equates.length; pass++) {
    for (const eq of equates) {
      if (!Object.hasOwn(symbols, eq.name)) { try { symbols[eq.name] = constant(eq.value, symbols); } catch {} }
    }
  }
  for (const eq of equates) if (!Object.hasOwn(symbols, eq.name)) throw new Error(`unresolved equate ${eq.name}`);
  for (const name of Object.keys(labels)) {
    if (Object.hasOwn(symbols, name)) throw new Error(`symbol appears as both code and data: ${name}`);
    symbols[name] = labels[name];
  }
  for (const item of data) {
    if (typeof item[2] === 'object') {
      const fixup = item[2];
      try { item[2] = constant(fixup.expression, symbols) | (fixup.mark ? 0x80 : 0); }
      catch (error) { throw new Error(`${config.source}:${fixup.line}: ${error.message}`); }
    }
  }
  if (code.length >= 0xffff) throw new Error('unit exceeds control-flow index range');
  return { code, symbols, widths, labels, data, config };
}

function operand(text, unit, prefix = null) {
  let explicitWidth = null, segment = text.includes('[') ? prefix : null;
  text = text.trim();
  text=text.replace(/\[(BX|BP|SI|DI)\]\.([\w$?@]+)/gi,'[$1.$2]');
  const size = /^(BYTE|WORD|DWORD)\s+PTR\s*/i.exec(text);
  if (size) { explicitWidth = {BYTE:8,WORD:16,DWORD:32}[size[1].toUpperCase()]; text = text.slice(size[0].length); }
  const seg = /^(ES|CS|DS|SS):\s*/i.exec(text);
  if (seg) { segment = seg[1].toUpperCase(); text = text.slice(seg[0].length); }
  const innerSize = /^(BYTE|WORD|DWORD)\s+PTR\s*/i.exec(text);
  if (innerSize) { explicitWidth = {BYTE:8,WORD:16,DWORD:32}[innerSize[1].toUpperCase()]; text = text.slice(innerSize[0].length); }
  if(seg && !text.includes('[') && !regs.has(text.toUpperCase())) text=`[${text}]`;
  text=text.replace(/^\[(.+)\](\s*[+-].+)$/,'[$1$2]');
  if (text.startsWith('[') && text.endsWith(']')) {
    const originalInner=text.slice(1,-1);
    const field=/\b(?:BX|BP|SI|DI)\.([A-Z_$?@][A-Z_0-9$?@]*)\b/i.exec(originalInner);
    const inner = originalInner.replace(/\b(BX|BP|SI|DI)\.([A-Z_$?@][A-Z_0-9$?@]*)\b/gi,'$1+$2');
    const address = `static_cast<std::uint16_t>(${expression(inner, unit.symbols, true)})`;
    const defaultSegment = /\bBP\b/i.test(inner) ? 'SS' : 'DS';
    const segmentCode = `m.get(Reg::${segment ?? defaultSegment})`;
    const width = explicitWidth ?? (field ? unit.widths[field[1].toUpperCase()] : unit.widths[inner.toUpperCase()]);
    return { kind: 'memory', width, address, segment: segmentCode,
      read: (bits) => `m.read(${bits}, ${segmentCode}, ${address})`,
      write: (value, bits) => `m.write(${bits}, ${segmentCode}, ${address}, ${value});` };
  }
  if (segment || explicitWidth) throw new Error('segment/size modifier requires a memory operand');
  const name = text.toUpperCase();
  if (regs.has(name)) {
    return { kind: 'register', width: byteRegs.has(name) ? 8 : 16,
      read: () => `m.get(Reg::${name})`, write: (value) => `m.set(Reg::${name}, ${value});` };
  }
  return { kind: 'immediate', width: null, read: () => expression(text, unit.symbols) };
}

function target(text, unit, index) {
  const label = text.replace(/^SHORT\s+/i, '').trim().toUpperCase();
  if (label === '$+5' && unit.config.relative_skip_jump) {
    if (unit.code[index + 1]?.op !== 'JMP' || !identifiers.test(unit.code[index + 1].args))
      throw new Error('relative skip requires exactly one direct JMP after this branch');
    return String(index + 2);
  }
  if (label === 'RET' || unit.config.return_labels?.includes(label)) return '0xfffe';
  if (!Object.hasOwn(unit.labels, label)) throw new Error(`unresolved branch/call target ${label}`);
  return String(unit.labels[label]);
}

function nativeLabel(unit, destination) {
  const index=Number(destination);
  if(index===0xfffe) return 'dos_near_return';
  if(index===0xffff) return 'dos_finished';
  if(index===unit.code.length) return 'dos_invalid_fallthrough';
  const name=Object.keys(unit.labels).find(name=>unit.labels[name]===index);
  return name ? `dos_${name.replace(/[^A-Z0-9_]/gi,'_')}_${index}` : `dos_continuation_${index}`;
}

function legacyCall(ins,unit) {
  return ins.op==='CALL' && ins.args.toUpperCase()===unit.config.legacy_call &&
    unit.symbols[ins.args.toUpperCase()]===5;
}

function nativeTargets(unit) {
  const entries=new Set(Object.values(unit.config.exports).map(name=>unit.labels[name]));
  const continuations=new Set(),direct=new Set(entries);
  const indirect=unit.code.some(ins=>ins.op==='JMP' && regs.has(ins.args.toUpperCase()));
  unit.code.forEach((ins,index)=>{
    if(ins.op==='CALL' && !legacyCall(ins,unit)) continuations.add(index+1);
    if(ins.op==='JMP' || branches[ins.op] || ins.op.startsWith('LOOP') || (ins.op==='CALL' && !legacyCall(ins,unit))) {
      if(ins.op==='JMP' && (regs.has(ins.args.toUpperCase()) || (ins.args==='0' && unit.config.legacy_call))) return;
      direct.add(Number(target(ins.args,unit,index)));
    }
  });
  const indirectTargets=indirect ? new Set([...Object.values(unit.labels),...continuations].filter(index=>index<unit.code.length)) : new Set();
  return {entries,continuations,indirectTargets,labels:new Set([...direct,...continuations,...indirectTargets])};
}

function nativeReturn(unit) {
  return nativeTargets(unit).continuations.size ? 'goto dos_return;'
    : 'if (dos_return_token != 0xffff) throw std::runtime_error("invalid native DOS return token"); return;';
}

export function emitInstruction(ins, unit, index) {
  let args = splitOperands(ins.args);
  let forcedWidth = null;
  // SCP assembler's MOV B,[BX],0 spelling.
  if (unit.config.dialect === 'scp' && ['B', 'W'].includes(args[0]?.toUpperCase())) {
    forcedWidth = args.shift().toUpperCase() === 'B' ? 8 : 16;
  }
  const op = ins.op;
  const noArg = { CLC: 'm.flags.carry = false;', STC: 'm.flags.carry = true;',
    CMC: 'm.flags.carry = !m.flags.carry;', CLD: 'm.flags.direction = false;',
    STD: 'm.flags.direction = true;', CLI: 'm.flags.interrupts = false;', STI: 'm.flags.interrupts = true;',
    NOP: '', CBW: 'm.set(Reg::AX, (m.get(Reg::AL) & 0x80U) ? 0xff00U | m.get(Reg::AL) : m.get(Reg::AL));',
    CWD: 'm.set(Reg::DX, (m.get(Reg::AX) & 0x8000U) ? 0xffffU : 0U);',
    LAHF: 'm.lahf();', SAHF: 'm.sahf();', DAA: 'm.daa();' };
  let body = '';
  const transfer=destination=>`goto ${nativeLabel(unit,destination)};`;
  const checkCount = (count) => { if (args.length !== count) throw new Error(`${op} expects ${count} operand(s)`); };
  if (Object.hasOwn(noArg, op)) { checkCount(0); body = noArg[op]; }
  else if (op === 'AAM' || op === 'AAD') {
    if (args.length > 1) throw new Error(`${op} expects zero or one operand`);
    const base = args.length ? constant(args[0], unit.symbols) : 10;
    if (base < 0 || base > 255) throw new Error(`${op} base outside byte range`);
    body = `m.${op.toLowerCase()}(${base});`;
  }
  else if (op === 'RET') {
    if(args.length>1) throw new Error('RET expects zero or one operand');
    const cleanup=args.length ? constant(args[0].replace(/\bSIZE\s+(\w+)/gi,'$1'),unit.symbols) : 0;
    if(cleanup<0 || cleanup>65535) throw new Error('invalid RET cleanup');
    return `dos_return_token = m.pop(); ${unit.config.abi==='far' ? 'm.set(Reg::CS, m.pop()); ' : ''}${cleanup ? `m.set(Reg::SP, m.get(Reg::SP) + ${cleanup}U); ` : ''}${nativeReturn(unit)}`;
  }
  else if (op === 'LDS' || op === 'LES') {
    checkCount(2);const a=operand(args[0],unit),b=operand(args[1],unit,ins.segment);
    if(a.kind!=='register' || a.width!==16 || ['CS','SS','DS','ES'].includes(args[0].toUpperCase()) ||
        b.kind!=='memory' || (b.width && b.width!==32)) throw new Error(`${op} requires a word register and far memory pointer`);
    // Read both halves before changing DS; the memory source may itself use DS.
    body=`const auto address = ${b.address}; const auto segment = ${b.segment}; `+
      `const auto offset = m.read16(segment, address); const auto selector = m.read16(segment, static_cast<std::uint16_t>(address + 2U)); `+
      a.write('offset',16)+` m.set(Reg::${op==='LDS' ? 'DS' : 'ES'}, selector);`;
  }
  else if (op === 'JMP' || branches[op]) {
    checkCount(1);
    if (op === 'JMP' && args[0] === '0' && unit.config.legacy_call)
      return 'm.interrupt(0x20); return;';
    if (op === 'JMP' && regs.has(args[0].toUpperCase())) {
      const reg = args[0].toUpperCase();
      if (byteRegs.has(reg)) throw new Error('indirect jump requires a word register');
      const destinations=[...nativeTargets(unit).indirectTargets].sort((a,b)=>a-b);
      return `switch (m.get(Reg::${reg})) {\n`+
        destinations.map(destination=>`                case ${destination}: ${transfer(destination)}`).join('\n')+
        '\n                case 0xffff: return;\n                case 0xfffe: goto dos_near_return;\n'+
        '                default: throw std::runtime_error("invalid native indirect jump target");\n            }';
    }
    const dest = target(args[0], unit, index);
    return op === 'JMP' ? transfer(dest) : `if (${branches[op]}) ${transfer(dest)}`;
  } else if (op === 'CALL') {
    checkCount(1);
    if (legacyCall(ins,unit))
      return 'const auto old_ah = m.get(Reg::AH); m.set(Reg::AH, m.get(Reg::CL)); m.interrupt(0x21); m.set(Reg::AH, old_ah); if (m.halted) return;';
    return `m.push(${index + 1}); ${transfer(target(args[0], unit, index))}`;
  } else if (op.startsWith('LOOP')) {
    checkCount(1);
    const condition = ['LOOPE', 'LOOPZ'].includes(op) ? ' && m.flags.zero' :
      ['LOOPNE', 'LOOPNZ'].includes(op) ? ' && !m.flags.zero' : '';
    return `m.set(Reg::CX, m.get(Reg::CX) - 1U); if (m.get(Reg::CX) != 0${condition}) ${transfer(target(args[0], unit, index))}`;
  } else if (op === 'INT') {
    checkCount(1);
    const number = constant(args[0], unit.symbols);
    if (number < 0 || number > 255) throw new Error('interrupt number outside byte range');
    body = `m.interrupt(${number}); if (m.halted) return;`;
  } else if (strings[op]) {
    checkCount(0);
    const [method, bits] = strings[op];
    if (ins.segment && ['stos', 'scas'].includes(method)) throw new Error('destination string instruction cannot override ES');
    const call = `m.${method}(${bits}${ins.segment ? `, Reg::${ins.segment}` : ''});`;
    if (ins.repeat) {
      if (!['REP', 'REPE', 'REPZ', 'REPNE', 'REPNZ'].includes(ins.repeat)) throw new Error('unknown repeat prefix');
      const compare = method === 'cmps' || method === 'scas';
      const stop = compare ? (['REPNE', 'REPNZ'].includes(ins.repeat) ? 'if (m.flags.zero) break;' : 'if (!m.flags.zero) break;') : '';
      body = `while (m.get(Reg::CX) != 0) { ${call} m.set(Reg::CX, m.get(Reg::CX) - 1U); ${stop} }`;
    } else body = call;
  } else if (op === 'PUSH' || op === 'POP') {
    checkCount(1);
    const a = operand(args[0], unit, ins.segment);
    if (a.width !== 16) throw new Error('only 16-bit stack operands supported');
    if (args[0].toUpperCase() === 'SP') throw new Error('8086 PUSH/POP SP require separate semantics');
    if (op === 'PUSH') body = `m.push(${a.read(16)});`;
    else { if (!a.write) throw new Error('POP requires writable operand'); body = a.write('m.pop()', 16); }
  } else {
    const unary = ['INC', 'DEC', 'NEG', 'NOT'].includes(op);
    const shift = ['SHL', 'SAL', 'SHR', 'SAR', 'ROL', 'ROR', 'RCL', 'RCR'].includes(op);
    if (shift && args.length === 1 && unit.config.dialect === 'scp') args.push('1');
    checkCount(unary ? 1 : 2);
    const a = operand(args[0], unit, ins.segment);
    const b = unary ? null : operand(args[1], unit, ins.segment);
    if (!a.write && !['CMP', 'TEST'].includes(op)) throw new Error('destination is not writable');
    if (a.kind === 'memory' && b?.kind === 'memory' && op !== 'LEA') throw new Error('memory-to-memory operation unsupported');
    const bits = forcedWidth ?? a.width ?? (shift ? null : b?.width) ??
      (unit.config.dialect === 'scp' && a.kind === 'memory' ? 16 : null);
    if (!bits) throw new Error('ambiguous operand width: add BYTE/WORD PTR');
    if(![8,16].includes(bits)) throw new Error('32-bit data operations require separate semantics');
    if (a.width && a.width !== bits) throw new Error('destination width mismatch');
    if (!shift && op !== 'LEA' && b?.width && b.width !== bits) throw new Error('operand width mismatch');
    const av = a.read(bits), bv = b?.read(bits);
    if (op === 'MOV') body = a.write(`static_cast<std::uint16_t>(${bv})`, bits);
    else if (op === 'LEA') {
      if (a.kind !== 'register' || bits !== 16 || b.kind !== 'memory') throw new Error('LEA requires a word register and address');
      body = a.write(b.address, bits);
    } else if (op === 'XCHG') {
      if (!b.write) throw new Error('XCHG requires writable operands');
      body = `const auto old = ${av}; ${a.write(bv, bits)} ${b.write('old', bits)}`;
    } else if (['ADD', 'ADC', 'SUB', 'SBB', 'CMP'].includes(op)) {
      const method = ['ADD', 'ADC'].includes(op) ? 'add' : 'sub';
      const result = `m.${method}(${av}, static_cast<std::uint16_t>(${bv}), ${bits}${['ADC', 'SBB'].includes(op) ? ', m.flags.carry' : ''})`;
      body = op === 'CMP' ? `${result};` : a.write(result, bits);
    } else if (['AND', 'OR', 'XOR', 'TEST'].includes(op)) {
      const kind = { AND: 'And', TEST: 'And', OR: 'Or', XOR: 'Xor' }[op];
      const result = `m.logic(${av}, static_cast<std::uint16_t>(${bv}), ${bits}, Logic::${kind})`;
      body = op === 'TEST' ? `${result};` : a.write(result, bits);
    } else if (unary) {
      const result = op === 'NOT' ? `~${av}` : `m.${op.toLowerCase()}(${av}, ${bits})`;
      body = a.write(result, bits);
    } else if (shift) {
      if (b.kind === 'register' && args[1].toUpperCase() !== 'CL') throw new Error('shift count must be CL or immediate');
      const kind = { SHL: 'Shl', SAL: 'Shl', SHR: 'Shr', SAR: 'Sar', ROL: 'Rol', ROR: 'Ror', RCL: 'Rcl', RCR: 'Rcr' }[op];
      body = a.write(`m.shift(${av}, static_cast<unsigned>(${bv}), ${bits}, Shift::${kind})`, bits);
    } else throw new Error(`missing code generator for ${op}`);
  }
  return body;
}

export function generateUnit(text, config, hash, dependencies = []) {
  const unit = parseUnit(text, config, dependencies);
  const flow=nativeTargets(unit);
  const exports=Object.entries(config.exports);
  for(const [name,label] of exports) {
    if(!identifiers.test(name) || !Object.hasOwn(unit.labels,label)) throw new Error(`invalid export ${name}: ${label}`);
  }
  const multiEntry=exports.length>1;
  const chunks = ['// Generated by tools/convert.mjs. Edit the converter or original inputs, not this file.',
    `// Source: ${config.source}`, `// SHA-256: ${hash}`, '// Copyright (c) Microsoft Corporation. MIT License.',
    '// Compiled C++ control flow. DOS stack/table tokens and data addresses are relocated.',
    '#include "msdos/machine.hpp"', '#include "ports.hpp"', '', `namespace msdos::${config.namespace} {`,
    'namespace {'];
  if(multiEntry) chunks.push(`enum class NativeEntry { ${exports.map(([name])=>name).join(', ')} };`);
  chunks.push(`void execute_${config.name}(Machine& m${multiEntry ? ', NativeEntry entry' : ''}) {`,
    '    if (m.halted) return;', '    std::uint16_t dos_return_token = 0;');
  if(multiEntry) chunks.push('    switch (entry) {',...exports.map(([name,label])=>
    `        case NativeEntry::${name}: goto ${nativeLabel(unit,unit.labels[label])};`),
    '        default: throw std::runtime_error("invalid native entry point");','    }');
  else chunks.push(`    goto ${nativeLabel(unit,unit.labels[exports[0][1]])};`);
  unit.code.forEach((ins, index) => {
    let code;
    try { code = emitInstruction(ins, unit, index); }
    catch (error) { throw new Error(`${config.source}:${ins.line}: ${error.message}`); }
    if(flow.labels.has(index)) chunks.push(`${nativeLabel(unit,index)}:`);
    chunks.push(`    { // Source line ${ins.line}: ${ins.raw.replaceAll('*/', '* /')}`,
      `        ${code}`, '    }');
  });
  if(flow.labels.has(unit.code.length)) chunks.push('dos_invalid_fallthrough:');
  chunks.push('    throw std::runtime_error("native routine fell through its source boundary");');
  if(flow.labels.has(0xffff)) chunks.push('dos_finished:', '    return;');
  // These are actual dynamic DOS returns, not a per-instruction dispatcher.
  // The original stack can be reset by nonlocal jumps (notably in TRANS).
  if(flow.continuations.size || flow.labels.has(0xfffe) || flow.indirectTargets.size)
    chunks.push('dos_near_return:', `    dos_return_token = m.pop(); ${nativeReturn(unit)}`);
  if(flow.continuations.size) chunks.push('dos_return:',
    '    switch (dos_return_token) {', '        case 0xffff: return;',
    '        case 0xfffe: goto dos_near_return;',
    ...[...flow.continuations].sort((a,b)=>a-b).map(destination=>
      `        case ${destination}: goto ${nativeLabel(unit,destination)};`),
    '        default: throw std::runtime_error("invalid native DOS return token");','    }');
  chunks.push('}', '} // namespace');
  for (const [name] of exports) {
    chunks.push('', `void ${name}(Machine& m) {`);
    if (config.program && name === config.name) {
      for (const [address, bits, value] of unit.data)
        chunks.push(`    m.write(${bits}, m.get(Reg::DS), ${address}, ${value});`);
    }
    if(config.abi==='far') chunks.push('    m.push(m.get(Reg::CS));');
    chunks.push('    m.push(0xffff);', `    execute_${config.name}(m${multiEntry ? ', NativeEntry::'+name : ''});`, '}');
  }
  chunks.push(`} // namespace msdos::${config.namespace}`, '');
  const dependencyComments=dependencies.map(item=>`// Include: ${item.source}; SHA-256: ${crypto.createHash('sha256').update(item.bytes).digest('hex')}\n`).join('');
  return { cpp: dependencyComments+chunks.join('\n'), unit };
}

function walk(directory) {
  return fs.readdirSync(directory, { withFileTypes: true }).sort((a, b) => a.name.localeCompare(b.name)).flatMap(entry =>
    entry.isDirectory() && !entry.name.startsWith('.') ? walk(path.join(directory, entry.name)) :
      entry.isFile() ? [path.join(directory, entry.name)] : []);
}

export function inventory(sourceRoot) {
  const files = walk(sourceRoot).filter(file => /\.(asm|inc|c|h)$/i.test(file));
  return files.map(file => {
    const bytes = fs.readFileSync(file), text = sourceText(bytes);
    const lines = text.split(/\r?\n/), histogram = {}, includes = [], procedures = [], hardware = [];
    lines.forEach((raw, n) => {
      let line;
      try { line = stripComment(raw).trim(); } catch { line = raw.trim(); }
      if (!line) return;
      const include = /^INCLUDE\s+(\S+)/i.exec(line);
      if (include) includes.push({ file: include[1], line: n + 1 });
      const proc = /^(?:PROCEDURE\s+([\w$?@]+)|([\w$?@]+)\s+PROC\b)/i.exec(line);
      if (proc) procedures.push({ name: proc[1] ?? proc[2], line: n + 1 });
      const clean = line.replace(/^[\w$?@]+:\s*/, '');
      const op = /^([A-Z][A-Z0-9]*)\b/i.exec(clean)?.[1].toUpperCase();
      if (op) histogram[op] = (histogram[op] ?? 0) + 1;
      if (/^(INT|IRET|IN|OUT|CLI|STI|HLT|LIDT|LGDT|LMSW|SMSW)\b/i.test(clean))
        hardware.push({ line: n + 1, source: clean });
    });
    return { path: path.relative(sourceRoot, file).split(path.sep).join('/'),
      kind: path.extname(file).slice(1).toLowerCase(), sha256: crypto.createHash('sha256').update(bytes).digest('hex'),
      bytes: bytes.length, lines: lines.length, includes, procedures, operation_histogram: histogram,
      hardware_boundaries: hardware, status: 'not_ported' };
  });
}

export function run(command, { root = projectRoot, sourceOverride } = {}) {
  const config = JSON.parse(fs.readFileSync(path.join(root, 'tools/units.json'), 'utf8'));
  const sourceRoot = path.resolve(root, sourceOverride ?? config.source_root);
  if (!fs.existsSync(sourceRoot)) throw new Error(`source directory not found: ${sourceRoot}`);
  fs.mkdirSync(path.join(root, 'generated'), { recursive: true });
  fs.mkdirSync(path.join(root, 'reports'), { recursive: true });
  const translations = [];
  const header = ['#pragma once', '// Generated declarations for source-lifted routines.', '#include "msdos/machine.hpp"'];
  // Validate every unit before writing any generated source file.
  for (const unitConfig of config.units) {
    const bytes = fs.readFileSync(path.join(sourceRoot, unitConfig.source));
    const hash = crypto.createHash('sha256').update(bytes).digest('hex');
    const dependencies=loadUnitDependencies(unitConfig,sourceRoot);
    const result = generateUnit(sourceText(bytes), unitConfig, hash,dependencies);
    translations.push({ ...result, config: unitConfig, hash });
    header.push(`namespace msdos::${unitConfig.namespace} {`);
    for (const name of Object.keys(unitConfig.exports)) header.push(`void ${name}(Machine& m);`);
    header.push('}');
  }
  if (command === 'generate') {
    for (const translation of translations) {
      const output = path.join(root, 'generated', translation.config.output);
      fs.mkdirSync(path.dirname(output), { recursive: true });
      fs.writeFileSync(output, translation.cpp);
    }
    fs.writeFileSync(path.join(root, 'generated/ports.hpp'), header.join('\n') + '\n');
  }
  const files = inventory(sourceRoot);
  const units = translations.map(({ unit, config: c, hash }) => ({
    name: c.name, source: c.source, source_sha256: hash, generated: `generated/${c.output}`,
    namespace: c.namespace, exports: c.exports, instructions: unit.code.length,
    start_line: unit.code[0].line, end_line: unit.code.at(-1).line,
    status: 'translated_validation_separate', full_source: Boolean(c.program || c.module),
    execution_backend:'native_cpp_control_flow',
    abi:c.abi ?? 'near',dependencies:(c.includes ?? []).map(source=>({source,
      sha256:crypto.createHash('sha256').update(fs.readFileSync(path.join(sourceRoot,source))).digest('hex')})),
  }));
  for (const file of files) {
    const matches = units.filter(unit => unit.source === file.path);
    if (matches.length) {
      file.status = matches.some(unit => unit.full_source) ? 'translated_validation_separate' : 'partially_translated';
      file.translated_units = matches.map(unit => unit.name);
    }
  }
  const versions = {};
  for (const file of files) {
    const version = file.path.split('/')[0];
    versions[version] ??= { files: 0, assembly: 0, includes: 0, lines: 0, bytes: 0 };
    versions[version].files++;
    versions[version].assembly += Number(file.kind === 'asm');
    versions[version].includes += Number(file.kind === 'inc');
    versions[version].lines += file.lines;
    versions[version].bytes += file.bytes;
  }
  const report = { schema: 1, source_root: config.source_root, target: 'bootable DOS compatibility kernel and portable C++20 validation',
    whole_operating_system_ported: false, versions, translated_units: units, files };
  fs.writeFileSync(path.join(root, 'reports/source-inventory.json'), JSON.stringify(report, null, 2) + '\n');
  const reportLines = ['# Conversion coverage', '',
    'This is a partial port. Translation counts are not proof of behavioral equivalence.', '',
    '| Version | Source files | ASM | INC | Lines |', '| --- | ---: | ---: | ---: | ---: |',
    ...Object.entries(versions).map(([v, c]) => `| ${v} | ${c.files} | ${c.assembly} | ${c.includes} | ${c.lines} |`),
    '', '| Unit | Source | Instructions | Full source file? |', '| --- | --- | ---: | --- |',
    ...units.map(u => `| ${u.name} | ${u.source}:${u.start_line} | ${u.instructions} | ${u.full_source ? 'yes' : 'no'} |`),
    '', 'The complete original kernel, COMMAND, BIOS, most utilities, memory managers, networking, and other versions remain unported.',
    'boot/kernel.cpp manually implements selected DOS 1.25 compatibility services; it is not a complete translation of the original kernel.',
    'Full DOS 4.0 mapper bodies retain external DOS/BIOS interrupt dependencies. See the [file-by-file DOS 4.0 audit](v4-missing.md) for remaining files and validation scope.',
    'The JSON inventory contains source hashes, include edges, procedure locations, and hardware boundaries for every ASM/INC/C/H file.',
    'All generated units use compiled C++ statements, labels, branches and loops; there is no per-instruction tick or program-counter executor.',
    'DOS register/memory/flag semantics and relocated return/table tokens are retained as compatibility data. These units cannot execute self-modifying code or preserve original binary layout.', ''];
  fs.writeFileSync(path.join(root, 'reports/coverage.md'), reportLines.join('\n'));
  return { files: files.length, units: units.length, instructions: units.reduce((n, u) => n + u.instructions, 0), versions };
}

if (process.argv[1] && path.resolve(process.argv[1]) === fileURLToPath(import.meta.url)) {
  try {
    const command = process.argv[2] ?? 'generate';
    if (!['generate', 'audit'].includes(command)) throw new Error('usage: node tools/convert.mjs [generate|audit] [--source PATH]');
    const sourceIndex = process.argv.indexOf('--source');
    if (sourceIndex >= 0 && !process.argv[sourceIndex + 1]) throw new Error('--source requires a directory');
    console.log(JSON.stringify(run(command, { sourceOverride: sourceIndex < 0 ? undefined : process.argv[sourceIndex + 1] }), null, 2));
  } catch (error) { console.error(error.message); process.exitCode = 1; }
}
