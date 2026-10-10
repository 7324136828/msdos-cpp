import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import { sourceText, stripComment, splitOperands, constant, expression, generateUnit, parseUnit, projectRoot, loadUnitDependencies } from '../tools/convert.mjs';
import os from 'node:os';
import {auditV4} from '../tools/audit-v4.mjs';

const config = { name: 'sample', source: 'sample.asm', namespace: 'sample', exports: { sample: 'START' } };

test('semicolon, comma, and doubled quotes within literals', () => {
  assert.equal(stripComment(' DB "a;b",13 ; comment'), ' DB "a;b",13 ');
  assert.deepEqual(splitOperands('"a,b",[BX+2],"it""s"'), ['"a,b"', '[BX+2]', '"it""s"']);
  assert.throws(() => stripComment('DB "broken'), /unterminated/);
});
test('historical EOF/padding does not become instructions', () => {
  assert.equal(sourceText(Buffer.from('RET\x1a\x00bad')), 'RET');
});

test('SCP equates may immediately follow a colon',()=>{
  const result=generateUnit('VALUE:EQU 42\nSTART:\nMOV AX,VALUE\nRET',config,'hash');
  assert.equal(result.unit.symbols.VALUE,42);
});

test('AAM and AAD support default and explicit byte bases',()=>{
  const cpp=generateUnit('START:\nAAM\nAAD 16\nRET',config,'hash').cpp;
  assert.match(cpp,/m.aam\(10\)/);assert.match(cpp,/m.aad\(16\)/);
  assert.throws(()=>generateUnit('START:\nAAM 256\nRET',config,'hash'),/base outside/);
  assert.throws(()=>generateUnit('START:\nAAD AX\nRET',config,'hash'),/unresolved symbol/);
});

test('complete MASM modules expand reviewed original macros and preserve source lines',()=>{
  const text='FRAME STRUC\nOLD_BP DW ?\nRETURN DD ?\nVALUE DD ?\nFRAME ENDS\nCODE SEGMENT\nINCLUDE macros.inc\nSTART PROC FAR\nEnter START\nLDS SI,[BP].VALUE\nexit: Mexit\nRET SIZE FRAME - 6\nSTART ENDP\nCODE ENDS\nEND';
  const module={...config,module:true,abi:'far',includes:['macros.inc']};
  const dependencies=[{source:'macros.inc',bytes:Buffer.from('Enter MACRO Routine\nPUSH BP\nMOV BP,SP\nENDM\nMexit MACRO\nMOV SP,BP\nPOP BP\nENDM')}];
  const parsed=parseUnit(text,module,dependencies);
  assert.equal(parsed.symbols.VALUE,6);assert.equal(parsed.symbols.FRAME,10);
  assert.equal(parsed.code[0].line,9);assert.equal(parsed.code[2].line,10);
  const cpp=generateUnit(text,module,'hash',dependencies).cpp;
  assert.match(cpp,/m.push\(m.get\(Reg::CS\)\)/);
  assert.match(cpp,/m.set\(Reg::CS, m.pop\(\)\)/);
  assert.match(cpp,/m.get\(Reg::SP\) \+ 4U/);
  assert.match(cpp,/Source line 9: PUSH BP/);
  assert.throws(()=>generateUnit(text,{...module,start:'START'},'hash',dependencies),/cannot select/);
  assert.throws(()=>generateUnit(text.replace('macros.inc','unknown.inc'),module,'hash',dependencies),/unreviewed include/);
  assert.throws(()=>generateUnit(text.replace('LDS SI','LDS AL'),module,'hash',dependencies),/word register/);
  assert.throws(()=>generateUnit(text.replace('LDS SI,[BP].VALUE','MOV DWORD PTR [BX],0'),module,'hash',dependencies),/separate semantics/);
  assert.throws(()=>generateUnit(text.replace('LDS SI,[BP].VALUE','IN AL,DX'),module,'hash',dependencies),/unsupported/);
  const external=text.replace('START PROC FAR','EXTRN unused:WORD\nSTART PROC FAR');
  assert.doesNotThrow(()=>generateUnit(external,module,'hash',dependencies));
  assert.throws(()=>generateUnit(external.replace('LDS SI,[BP].VALUE','MOV AX,unused'),module,'hash',dependencies),/unresolved symbol/);
  assert.match(generateUnit(text.replace('LDS SI,[BP].VALUE','MOV AL,ES:0FFFEH'),module,'hash',dependencies).cpp,
    /m.read\(8, m.get\(Reg::ES\), static_cast<std::uint16_t>\(65534\)\)/);
});

test('DOS 4 audit counts dependencies and extra inputs without claiming translation, and rejects missing/stale copies',()=>{
  const root=fs.mkdtempSync(path.join(os.tmpdir(),'msdos-v4-audit-'));
  const write=(relative,content)=>{
    const file=path.join(root,relative);fs.mkdirSync(path.dirname(file),{recursive:true});fs.writeFileSync(file,content);
  };
  try {
    const source='v4.0/src/MAPPER/GETVER.ASM',include='v4.0/src/MAPPER/MACROS.INC';
    const config={name:'mapper_getver',source,namespace:'v400',output:'v4.0/mapper/getver.cpp',module:true,abi:'far',
      includes:[include],exports:{dosgetversion:'DOSGETVERSION'}};
    const bytes=fs.readFileSync(path.join(projectRoot,'reference',source));
    const dependencies=[{source:include,bytes:fs.readFileSync(path.join(projectRoot,'reference',include))}];
    write('reference/'+source,bytes);write('reference/'+include,dependencies[0].bytes);
    write('reference/v4.0/src/TOOLS/utility.c','int main(void) { return 0; }\n');
    write('reference/v4.0/src/INC/input.mac','helper macro\nendm\n');
    write('reference/v4.0/src/TOOLS/utility.exe',Buffer.from([0x4d,0x5a]));
    write('reference/v4.0/src/TOOLS/MAKEFILE','all:\n');
    write('tools/units.json',JSON.stringify({units:[config]}));
    const cpp=generateUnit(sourceText(bytes),config,crypto.createHash('sha256').update(bytes).digest('hex'),dependencies).cpp;
    write('generated/'+config.output,cpp);write('CMakeLists.txt','add_library(test generated/'+config.output+')\n');
    const originalRoot=path.join(root,'original');fs.cpSync(path.join(root,'reference/v4.0'),originalRoot,{recursive:true});
    let result=auditV4({root,originalRoot});
    assert.equal(result.source_files,3);assert.equal(result.complete_source_files,1);assert.equal(result.pending_source_files,2);
    assert.equal(result.additional_source_inputs,1);assert.equal(result.all_reference_files,6);assert.equal(result.reference_mismatches,0);
    const report=()=>JSON.parse(fs.readFileSync(path.join(root,'reports/v4-audit.json'),'utf8'));
    assert.equal(report().whole_dos4_port_complete,false);
    assert.equal(report().files.find(file=>file.source===include).status,'expanded_macro_dependency');
    assert.equal(report().files.find(file=>file.kind==='c').status,'not_translated');
    assert.equal(report().supporting_files.find(file=>file.kind==='exe').role,'supplied_binary');
    write('reference/v4.0/src/TOOLS/utility.c','changed\n');
    assert.throws(()=>auditV4({root,originalRoot}),/reference copies differ/);
    assert.deepEqual(report().reference_mismatches,['v4.0/src/TOOLS/utility.c']);
    fs.copyFileSync(path.join(originalRoot,'src/TOOLS/utility.c'),path.join(root,'reference/v4.0/src/TOOLS/utility.c'));
    fs.unlinkSync(path.join(root,'reference/v4.0/src/TOOLS/utility.exe'));
    assert.throws(()=>auditV4({root,originalRoot}),/reference copies differ/);
    assert.deepEqual(report().reference_mismatches,['v4.0/src/TOOLS/utility.exe']);
    fs.copyFileSync(path.join(originalRoot,'src/TOOLS/utility.exe'),path.join(root,'reference/v4.0/src/TOOLS/utility.exe'));
    write('generated/'+config.output,cpp+'// stale\n');
    assert.throws(()=>auditV4({root,originalRoot}),/Stale or modified C\+\+/);
    write('generated/'+config.output,cpp);write('CMakeLists.txt','');
    assert.throws(()=>auditV4({root,originalRoot}),/missing from CMake/);
    write('CMakeLists.txt','add_library(test generated/'+config.output+')\n');
    result=auditV4({root,originalRoot:path.join(root,'unavailable')});
    assert.equal(result.reference_mismatches,0);assert.equal(report().original_comparison_available,false);
    assert.equal(report().files[0].reference_matches_original,null);
  } finally {fs.rmSync(root,{recursive:true,force:true});}
});
test('MASM/SCP expressions are parsed without eval', () => {
  assert.equal(constant('(START+15)/16', { START: 0x102 }), 17);
  assert.equal(constant('"A"-"0"-10'), 7);
  assert.equal(constant('0FFFH AND NOT 0FH'), 0xff0);
  assert.equal(constant('-100H'), -256);
  assert.equal(constant('101B'), 5);
  assert.match(expression('SI+BX', {}, true), /Reg::SI.*Reg::BX/);
  assert.throws(() => expression('process.exit()'), /unsupported expression|unresolved symbol/);
  assert.throws(() => constant('1/0'), /division by zero/);
  assert.throws(() => constant('MISSING+1'), /unresolved symbol/);
});
test('unsupported operations, wrong widths, and unresolved targets fail closed', () => {
  assert.throws(() => generateUnit('START:\nIN AL,DX\nRET', config, 'hash'), /unsupported/);
  assert.throws(() => generateUnit('START:\nMOV [BX],1\nRET', config, 'hash'), /ambiguous operand width/);
  assert.throws(() => generateUnit('START:\nMOV AX,BL\nRET', config, 'hash'), /width mismatch/);
  assert.throws(() => generateUnit('START:\nJMP MISSING\nRET', config, 'hash'), /unresolved branch/);
  assert.throws(() => generateUnit('START:\nMOV WORD PTR [BX],[SI]\nRET', config, 'hash'), /memory-to-memory/);
  assert.throws(() => generateUnit('START:\nPUSH SP\nRET', config, 'hash'), /separate semantics/);
});
test('SCP JP is unconditional; MASM JP tests parity', () => {
  const text = 'START:\nJP END\nEND:\nRET';
  assert.equal(parseUnit(text, { ...config, dialect: 'scp' }).code[0].op, 'JMP');
  assert.match(generateUnit(text, config, 'hash').cpp, /m.flags.parity/);
  assert.doesNotMatch(generateUnit(text, { ...config, dialect: 'scp' }, 'hash').cpp, /m.flags.parity/);
});
test('segment prefixes and standalone repeat prefixes survive conversion', () => {
  const text = 'START:\nSEG ES\nMOV CX,[6]\nREP\nSTOW\nRET';
  const cpp = generateUnit(text, { ...config, dialect: 'scp' }, 'hash').cpp;
  assert.match(cpp, /m.read\(16, m.get\(Reg::ES\)/);
  assert.match(cpp, /while \(m.get\(Reg::CX\) != 0\).*m.stos\(16\)/);
  assert.throws(() => generateUnit('START:\nREP', config, 'hash'), /dangling/);
});
test('checked-in generated files match original assembly and converter output', () => {
  const manifest = JSON.parse(fs.readFileSync(path.join(projectRoot, 'tools/units.json')));
  for (const unit of manifest.units) {
    const bytes = fs.readFileSync(path.resolve(projectRoot, manifest.source_root, unit.source));
    const hash = crypto.createHash('sha256').update(bytes).digest('hex');
    const result = generateUnit(sourceText(bytes), unit, hash,loadUnitDependencies(unit,path.resolve(projectRoot,manifest.source_root)));
    assert.equal(fs.readFileSync(path.join(projectRoot, 'generated', unit.output), 'utf8'), result.cpp);
  }
});

test('MASM field offsets come from structures and disjoint sections retain source lines',()=>{
  const text='FCB STRUC\nDB 12 DUP (?)\nBLOCK DW ?\nSIZE DW ?\nFCB ENDS\nA:\nMOV AX,[DI.SIZE]\nCALL B\nRET\nIGNORED:\nIN AL,DX\nB:\nMOV BL,[SI.BLOCK]\nRET\nENDMARK:';
  const unit={...config,exports:{sample:'A'},structures:['FCB'],sections:[{start:'A:',end:'IGNORED:'},{start:'B:',end:'ENDMARK:'}]};
  const parsed=parseUnit(text,unit);assert.equal(parsed.symbols.SIZE,14);assert.equal(parsed.code[3].line,13);
  // Byte register conflicts with a WORD structure field: reject instead of truncating.
  assert.throws(()=>generateUnit(text,unit,'hash'),/width mismatch/);
  const good=text.replace('MOV BL,[SI.BLOCK]','MOV BX,[SI.BLOCK]');
  const cpp=generateUnit(good,unit,'hash').cpp;assert.match(cpp,/Reg::DI\).*14/);assert.match(cpp,/Source line 13: MOV BX/);
  assert.throws(()=>parseUnit(text,{...unit,start:'A:'}),/sections cannot/);
  assert.throws(()=>parseUnit(text,{...unit,sections:[{start:'A:',end:'ENDMARK:'},{start:'B:',end:'ENDMARK:'}]}),/overlapping/);
  assert.throws(()=>parseUnit(text.replace('SIZE DW ?','SIZE DQ ?'),unit),/unsupported structure/);
});
