import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import {createRequire} from 'node:module';
import {root} from './build-boot.mjs';
import {smokeCases,smokeScope,shellPrompt} from './boot-smoke-cases.mjs';

const directory=path.join(root,'build/emulator/v86');
const image=fs.readFileSync(path.join(root,'build/boot/msdos-cpp.img'));
const manifest=JSON.parse(fs.readFileSync(path.join(root,'tools/v86-assets.json'),'utf8'));
const digest=bytes=>crypto.createHash('sha256').update(bytes).digest('hex');
for(const asset of manifest.assets) {
  const file=path.join(directory,asset.name);
  if(!fs.existsSync(file)) throw new Error('Run node tools/setup-v86.mjs to download the local emulator.');
  if(digest(fs.readFileSync(file))!==asset.sha256) throw new Error(`Emulator asset hash mismatch: ${asset.name}`);
}
const {V86}=createRequire(import.meta.url)(path.join(directory,'libv86.cjs'));
const buffer=bytes=>Uint8Array.from(bytes).buffer;
const local=name=>({buffer:buffer(fs.readFileSync(path.join(directory,name)))});
const emulator=new V86({wasm_path:path.join(directory,'v86.wasm'),
  bios:local('seabios.bin'),vga_bios:local('vgabios.bin'),fda:{buffer:buffer(image)},
  memory_size:16*1024*1024,boot_order:0x321,autostart:true});
let output='',stage=0,boundary=0,finished=false;
function finish(ok,reason) {
  if(finished) return;finished=true;clearTimeout(timer);emulator.destroy();
  fs.mkdirSync(path.join(root,'reports'),{recursive:true});
  fs.writeFileSync(path.join(root,'reports/boot-smoke.txt'),output.trimEnd()+'\n');
  fs.writeFileSync(path.join(root,'reports/boot-runtime.json'),JSON.stringify({runtime_verified:ok,
    emulator:'v86',emulator_assets:manifest.assets.map(({name,sha256})=>({name,sha256})),
    image_sha256:digest(image),checks_completed:stage,reason,scope:smokeScope},null,2)+'\n');
  console.log(output);
  console.log(reason);
  process.exitCode=ok ? 0 : 1;
}
const timer=setTimeout(()=>finish(false,'Boot smoke test timed out.'),45000);
emulator.add_listener('serial0-output-byte',byte=>{
  if(finished) return;
  if(byte!==13) output+=String.fromCharCode(byte);
  if(!output.endsWith(shellPrompt)) return;
  const step=smokeCases[stage],section=output.slice(boundary);
  if(!step.expected.every(value=>section.includes(value))) return finish(false,`Guest check ${stage} failed.`);
  stage++;boundary=output.length;
  if(!step.command) return finish(true,'Boot smoke test passed.');
  setImmediate(()=>{if(!finished) emulator.serial0_send(step.command+'\r');});
});
