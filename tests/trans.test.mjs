import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import { spawnSync } from 'node:child_process';

const executable = process.env.TRANS_EXE ?? path.resolve('build/trans' + (process.platform === 'win32' ? '.exe' : ''));
const fixtures = new URL('./fixtures/', import.meta.url);
function translate(text) {
  const directory = fs.mkdtempSync(path.join(os.tmpdir(), 'msdos-trans-'));
  try {
    const input = path.join(directory, 'input.z80'), output = path.join(directory, 'output.asm');
    fs.writeFileSync(input, text + '\x1a');
    const result = spawnSync(executable, [input, output], { encoding: 'utf8', timeout: 15000 });
    assert.ifError(result.error);
    const bytes = fs.existsSync(output) ? fs.readFileSync(output) : null;
    return { ...result, bytes, text: bytes?.toString('latin1').split('\x1a')[0] };
  } finally { fs.rmSync(directory, { recursive: true, force: true }); }
}
test('register mapping, opcode dispatch, comments, and CRLF', () => {
  const result = translate(' LD A,42 ; comment\r\n LD HL,1234H\r\n RET\r\n');
  assert.equal(result.status, 0, result.stderr);
  assert.equal(result.text, '\tMOV\tAL,42 \t; comment\r\n\tMOV\tBX,1234H\r\n\tRET\r\n');
  assert.equal(result.bytes.length % 128, 0);
});
test('bit operations exercise decimal-adjust conversion to hexadecimal', () => {
  const result = translate(' SET 7,A\r\n RES 0,B\r\n');
  assert.equal(result.status, 0, result.stderr);
  assert.match(result.text, /080H/);
  assert.match(result.text, /0FEH/);
});
test('multiple input/output records and indirect table jumps', () => {
  const result = translate(' LD A,B\r\n'.repeat(100));
  assert.equal(result.status, 0, result.stderr);
  assert.equal(result.text, '\tMOV\tAL,CH\r\n'.repeat(100));
});
test('all twenty indirect opcode handlers match the previous backend byte for byte', () => {
  // Captured with the previous C++ dispatcher before changing generated control
  // flow. Include the original warning/error text and DOS record EOF padding.
  const input = fs.readFileSync(new URL('trans-control-flow.z80', fixtures), 'latin1').split('\x1a')[0];
  const expected = fs.readFileSync(new URL('trans-control-flow.expected.asm', fixtures));
  const result = translate(input);
  assert.equal(result.status, 0, result.stderr);
  assert.deepEqual(result.bytes, expected);
});
test('opcode error unwinds nested DOS calls and continues at the next source line', () => {
  // OPERR jumps to ENLIN while FINDOP's return token remains on the DOS stack.
  // ENLIN resets SP; a native C++ call stack must not control this recovery.
  const input = fs.readFileSync(new URL('trans-error-recovery.z80', fixtures), 'latin1').split('\x1a')[0];
  const expected = fs.readFileSync(new URL('trans-error-recovery.expected.asm', fixtures));
  const result = translate(input);
  assert.equal(result.status, 0, result.stderr);
  assert.deepEqual(result.bytes, expected);
  assert.match(result.text, /\*\*\* Opcode Error \r\n\tMOV\tAL,42\r\n$/);
});
