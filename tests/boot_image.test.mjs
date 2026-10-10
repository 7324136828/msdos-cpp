import test from 'node:test';
import assert from 'node:assert/strict';
import { makeImage } from '../tools/build-boot.mjs';

const boot=Buffer.alloc(512);boot.writeUInt16LE(0xaa55,510);
test('FAT12 image size, both FAT copies, root metadata, and chains',()=>{
  const payload=Buffer.from(Array.from({length:1300},(_,i)=>i&255));
  const image=makeImage(boot,Buffer.from([0x90,0xf4]),[{name:'HELLO.COM',bytes:payload}]);
  assert.equal(image.length,1474560);
  assert.equal(image.readUInt16LE(510),0xaa55);
  assert.deepEqual(image.subarray(65*512,74*512),image.subarray(74*512,83*512));
  const entry=image.subarray(83*512,83*512+32);
  assert.equal(entry.subarray(0,11).toString('ascii'),'HELLO   COM');
  assert.equal(entry.readUInt32LE(28),payload.length);
  let cluster=entry.readUInt16LE(26);const recovered=[];
  const fat=image.subarray(65*512,74*512);
  while(cluster<0xff8){
    assert.ok(cluster>=2 && cluster<2785);
    recovered.push(image.subarray((97+cluster-2)*512,(98+cluster-2)*512));
    // Independent extraction: 12 consecutive bits, not the writer's word/nibble formula.
    let next=0;for(let bit=0;bit<12;bit++){const p=cluster*12+bit;next|=((fat[Math.floor(p/8)]>>(p%8))&1)<<bit;}
    cluster=next;assert.ok(recovered.length<=3);
  }
  assert.deepEqual(Buffer.concat(recovered).subarray(0,payload.length),payload);
});
test('invalid boot sector, oversized kernel, bad names, and full media are rejected',()=>{
  assert.throws(()=>makeImage(Buffer.alloc(512),Buffer.alloc(0),[]),/boot sector/);
  assert.throws(()=>makeImage(boot,Buffer.alloc(32769),[]),/kernel exceeds/);
  assert.throws(()=>makeImage(boot,Buffer.alloc(0),[{name:'bad/../../x',bytes:Buffer.alloc(1)}]),/8.3 name/);
  assert.throws(()=>makeImage(boot,Buffer.alloc(0),[{name:'BIG.COM',bytes:Buffer.alloc(1474560)}]),/image full/);
});
