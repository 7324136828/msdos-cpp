import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import { spawnSync } from 'node:child_process';

const executable = process.env.HEX2BIN_EXE ?? path.resolve('build/hex2bin' + (process.platform === 'win32' ? '.exe' : ''));
function record(address, data, type = 0) {
  const values = [data.length, address >> 8, address & 255, type, ...data];
  const checksum = (-values.reduce((n, b) => n + b, 0)) & 255;
  return ':' + [...values, checksum].map(b => b.toString(16).padStart(2, '0').toUpperCase()).join('') + '\r\n';
}
function convert(text, offset) {
  const directory = fs.mkdtempSync(path.join(os.tmpdir(), 'msdos-hex2bin-'));
  try {
    const input = path.join(directory, 'input.hex'), output = path.join(directory, 'output.com');
    fs.writeFileSync(input, text);
    const result = spawnSync(executable, [input, output, ...(offset === undefined ? [] : [offset])], { encoding: 'utf8', timeout: 15000 });
    assert.ifError(result.error);
    return { ...result, bytes: fs.existsSync(output) ? fs.readFileSync(output) : null };
  } finally { fs.rmSync(directory, { recursive: true, force: true }); }
}
test('default offset, out-of-order records, and zero-filled gaps', () => {
  const result = convert(record(0x105, [0xbe, 0xef]) + record(0x100, [1,2]) + record(0, [], 1));
  assert.equal(result.status, 0, result.stderr);
  assert.deepEqual(result.bytes, Buffer.from([1,2,0,0,0,0xbe,0xef]));
});
test('positive and unsigned load offsets', () => {
  for (const offset of ['+1', '1']) {
    const result = convert(record(0, [0xca,0xfe]) + record(0, [], 1), offset);
    assert.equal(result.status, 0, result.stderr);
    assert.deepEqual(result.bytes, Buffer.from([0,0xca,0xfe]));
  }
});
test('input crosses the original 1024-byte FCB buffer boundary', () => {
  let text = ''; const expected = [];
  for (let i = 0; i < 128; i++) {
    const data = Array.from({ length: 16 }, (_, j) => (i * 16 + j) & 255);
    expected.push(...data); text += record(0x100 + i * 16, data);
  }
  const result = convert(text + record(0, [], 1));
  assert.equal(result.status, 0, result.stderr);
  assert.deepEqual(result.bytes, Buffer.from(expected));
});
test('original ignores record type and checksum rather than claiming strict Intel HEX validation', () => {
  const result = convert(':010100AA7F00\r\n:00000001FF\r\n');
  assert.equal(result.status, 0, result.stderr);
  assert.deepEqual(result.bytes, Buffer.from([0x7f]));
});
test('control-Z and zero-count records finish immediately', () => {
  assert.deepEqual(convert('\x1a').bytes, Buffer.alloc(0));
  const result = convert(':00garbage');
  assert.equal(result.status, 0, result.stderr);
  assert.deepEqual(result.bytes, Buffer.alloc(0));
});
test('invalid digit reports original message and produces original partial output', () => {
  const result = convert(':01010000GG00\r\n');
  assert.equal(result.status, 1);
  assert.match(result.stderr, /Error in HEX file--conversion aborted/);
  assert.deepEqual(result.bytes, Buffer.alloc(0));
});
test('offset wrapping and output segment bounds', () => {
  const result = convert(record(0, [0x90]) + record(0, [], 1));
  assert.equal(result.status, 0, result.stderr);
  assert.equal(result.bytes.length, 0xff01);
  assert.equal(result.bytes[0xff00], 0x90);
  const outOfRange = convert(record(0xfff0, [0x90]), '+0');
  assert.equal(outOfRange.status, 1);
  assert.match(outOfRange.stderr, /Address out of range/);
});
test('unterminated input and malformed CLI offset fail', () => {
  assert.equal(convert(':').status, 1);
  const result = convert(record(0, [], 1), '-x');
  assert.equal(result.status, 1);
  assert.equal(result.bytes, null);
});
test('same-file conversion is rejected before writing', () => {
  const directory = fs.mkdtempSync(path.join(os.tmpdir(), 'msdos-hex2bin-'));
  try {
    const file = path.join(directory, 'same.hex');
    const text = record(0x100, [0x42]) + record(0, [], 1);
    fs.writeFileSync(file, text);
    const result = spawnSync(executable, [file, file], { encoding: 'utf8' });
    assert.equal(result.status, 1);
    assert.equal(fs.readFileSync(file, 'utf8'), text);
  } finally { fs.rmSync(directory, { recursive: true, force: true }); }
});
