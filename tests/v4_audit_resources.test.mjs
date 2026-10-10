import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import os from 'node:os';
import {auditV4,classifySupportingInput} from '../tools/audit-v4.mjs';

const classify=(source,text)=>classifySupportingInput(source,Buffer.from(text));

test('source inputs with configuration and LIB suffixes are not hidden as binaries or build files',()=>{
  for(const source of ['src/INC/DBCS.ON','src/INC/DBCS.OFF','src/CMD/DEBUG/SYSVER.TRU',
    'src/CMD/DEBUG/SYSVER.FAL','src/SELECT/USA.INF'])
    assert.equal(classify(source,'FLAG EQU TRUE\n').role,'additional_source_input',source);
  assert.equal(classify('src/CMD/APPEND/SYSMAC.LIB','; macros\nDISP MACRO TEXT\nMOV AX,1\nENDM\n\x1a').role,
    'additional_source_input');
  assert.equal(classifySupportingInput('src/INC/COMSUBS.LIB',Buffer.from([0xf0,0x0a,0,0x20])).role,'supplied_binary');
  // Embedded textual names in an object library cannot promote it to source.
  assert.equal(classify('src/LIB/example.LIB','\x00DISP MACRO TEXT\nENDM\n').role,'supplied_binary');
});

test('runtime resources are distinguished from compiler inputs and design documentation',()=>{
  for(const source of ['src/CMD/GRAPHICS/GRAPHICS.PRO','src/SELECT/SELECT.PRT','src/SELECT/USA.PRT','src/SELECT/USA.TXT'])
    assert.equal(classify(source,'profile data\n').role,'runtime_resource_input',source);
  for(const source of ['src/DEV/PRINTER/4201/4201.CPI','src/INC/SHELL.MEU'])
    assert.equal(classifySupportingInput(source,Buffer.from([0,1,2])).role,'supplied_binary_resource',source);
  for(const source of ['src/DEV/DISPLAY/ZERO.DAT','src/BIOS/LOCSCR','src/CMD/FASTOPEN/FASTOPEN.TXT'])
    assert.equal(classify(source,'documentation or relocation input\n').role,'build_or_resource_file',source);
});

test('audit retains hidden source files and resources in its complete file accounting',()=>{
  const tempBase=path.resolve(os.tmpdir());
  const root=fs.mkdtempSync(path.join(tempBase,'msdos-v4-resource-audit-'));
  const write=(relative,content)=>{
    const file=path.join(root,relative);fs.mkdirSync(path.dirname(file),{recursive:true});fs.writeFileSync(file,content);
  };
  try {
    write('tools/units.json',JSON.stringify({units:[]}));write('CMakeLists.txt','');
    const inputs={
      'src/INC/DBCS.ON':'DBCS = TRUE\n',
      'src/CMD/DEBUG/SYSVER.TRU':'SYSVER EQU TRUE\n',
      'src/SELECT/USA.INF':'MESSAGE DB "hello",0\n',
      'src/CMD/APPEND/SYSMAC.LIB':'DISP MACRO TEXT\nENDM\n',
      'src/CMD/GRAPHICS/GRAPHICS.PRO':'PRINTER GRAPHICS\n',
      'src/SELECT/USA.TXT':'.HELPID. 1\n',
      'src/INC/SHELL.MEU':Buffer.from([0,1,2]),
      'src/INC/COMSUBS.LIB':Buffer.from([0xf0,0,0,1]),
      'src/BIOS/LOCSCR':'70\n',
    };
    for(const [source,bytes] of Object.entries(inputs)) {
      write('reference/v4.0/'+source,bytes);write('original/'+source,bytes);
    }
    const result=auditV4({root,originalRoot:path.join(root,'original')});
    assert.equal(result.additional_source_inputs,4);assert.equal(result.runtime_resource_inputs,3);
    assert.equal(result.all_reference_files,9);assert.equal(result.reference_mismatches,0);
    const report=JSON.parse(fs.readFileSync(path.join(root,'reports/v4-audit.json'),'utf8'));
    assert.equal(report.source_files,0);assert.equal(report.whole_dos4_port_complete,false);
    assert.equal(report.supporting_files.length,9);
    assert.equal(report.supporting_files.find(file=>file.source.endsWith('SYSMAC.LIB')).role,'additional_source_input');
    assert.equal(report.supporting_files.find(file=>file.source.endsWith('COMSUBS.LIB')).role,'supplied_binary');
    assert.match(fs.readFileSync(path.join(root,'reports/v4-missing.md'),'utf8'),/Runtime and help resources/);
  } finally {
    const resolvedRoot=path.resolve(root);
    assert.ok(resolvedRoot.startsWith(tempBase+path.sep) && path.basename(resolvedRoot).startsWith('msdos-v4-resource-audit-'));
    fs.rmSync(resolvedRoot,{recursive:true,force:true});
  }
});
