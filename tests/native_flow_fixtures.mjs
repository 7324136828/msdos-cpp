import assert from 'node:assert/strict';
import crypto from 'node:crypto';
import fs from 'node:fs';
import path from 'node:path';
import {generateUnit, projectRoot} from '../tools/convert.mjs';

// Small source fixtures exercise assembly control-flow contracts independently
// of the DOS routines, while running the same native C++ generator.
const fixtures = [
  ['nested_calls', `ENTRY:
MOV AX,1
CALL FIRST
ADD AX,8
RET
FIRST:
ADD AX,2
CALL SECOND
ADD AX,4
JMP SHARED_RETURN
SECOND:
ADD AX,16
RET
SHARED_RETURN:
RET`],
  ['conditional_return', `ENTRY:
CMP AX,0
JZ RETURN
MOV BX,77
RET`, {return_labels:['RETURN']}],
  ['table_jump', `ENTRY:
MOV BX,TABLE
ADD BX,SI
MOV DX,[BX]
JMP DX
FIRST:
MOV AX,11
RET
SECOND:
MOV AX,22
RET
TABLE:
DW FIRST,SECOND,0FFFDH`, {program:true}],
  ['invalid_return', `ENTRY:
MOV AX,0FFFDH
PUSH AX
RET`],
  ['interrupt_halt', `ENTRY:
MOV AX,7
INT 21H
MOV AX,9
RET`],
  ['call5_halt', `SYSTEM EQU 5
ENTRY:
MOV CL,2
CALL SYSTEM
MOV AX,9
RET`, {legacy_call:'SYSTEM'}],
  ['repeat_copy', `ENTRY:
REP MOVSB
RET`],
  ['nonlocal_return', `ENTRY:
MOV BP,SP
CALL OUTER
MOV AX,999
RET
OUTER:
CALL INNER
MOV AX,888
RET
INNER:
MOV SP,BP
MOV AX,42
RET`],
];

const output=path.resolve(process.argv[2] ?? path.join(projectRoot,'build/native_flow_fixture.cpp'));
const chunks=['// Generated test fixtures; do not edit.'];
for(const [name,source,options] of fixtures) {
  const config={name,source:`tests/fixtures/${name}.asm`,namespace:'native_fixture',exports:{[name]:'ENTRY'},...options};
  const hash=crypto.createHash('sha256').update(source).digest('hex');
  const {cpp}=generateUnit(source,config,hash);
  assert.doesNotMatch(cpp,/\bm\.tick\s*\(/,`${name} must execute without instruction ticking`);
  assert.doesNotMatch(cpp,/switch\s*\(\s*pc\s*\)/,`${name} must execute without an instruction dispatcher`);
  assert.doesNotMatch(cpp,/while\s*\(\s*!m\.halted\s*\)/,`${name} must execute without a program-counter loop`);
  chunks.push(cpp);
}
fs.mkdirSync(path.dirname(output),{recursive:true});
fs.writeFileSync(output,chunks.join('\n'));
