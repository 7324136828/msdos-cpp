import fs from 'node:fs';
import path from 'node:path';
import { makeImage, fatSet } from './build-boot.mjs';
const boot=Buffer.alloc(512);boot.writeUInt16LE(0xaa55,510);
const bytes=Buffer.from(Array.from({length:1300},(_,i)=>i*7&255));
const image=makeImage(boot,Buffer.from([0xf4]),[
  {name:'HELLO.TXT',bytes:Buffer.from('FCB read works.\r\n\x1a')},
  {name:'MULTI.BIN',bytes},
  {name:'TEST.COM',bytes:Buffer.from([0xb4,9,0xcd,0x21,0xcd,0x20])},
]);
// Fragment the second file: its clusters 3 -> 4 -> 5 become 3 -> 7 -> 5.
image.copy(image,(97+7-2)*512,(97+4-2)*512,(97+4-1)*512);
for(const base of [65,74]) {
  const fat=image.subarray(base*512,(base+9)*512);
  fatSet(fat,3,7);fatSet(fat,7,5);fatSet(fat,4,0);
}
const out=path.resolve(process.argv[2] ?? 'build/boot/test-fixture.img');
fs.mkdirSync(path.dirname(out),{recursive:true});fs.writeFileSync(out,image);
