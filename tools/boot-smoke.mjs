// Optional real guest execution test. Requires QEMU; host tests do not replace it.
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import { spawn } from 'node:child_process';
import { root } from './build-boot.mjs';
import {smokeCases as steps,smokeScope,shellPrompt} from './boot-smoke-cases.mjs';

const image=path.resolve(root,'build/boot/msdos-cpp.img');
if(!fs.existsSync(image)) {console.error('Build the boot_image target first.');process.exit(1);}
const executable=process.env.QEMU ?? 'qemu-system-i386';
const qemu=spawn(executable,['-machine','pc','-m','16','-display','none','-monitor','none',
  '-serial','stdio','-no-reboot','-snapshot','-drive',`file=${image.replaceAll('\\','/').replaceAll(',',',,')},format=raw,if=floppy`,'-boot','a'],
  {stdio:['pipe','pipe','pipe']});
let output='',errorOutput='',stage=0,boundary=0,finished=false;
function finish(ok,reason) {
  if(finished) return;finished=true;clearTimeout(timer);qemu.kill();
  fs.mkdirSync(path.join(root,'reports'),{recursive:true});
  fs.writeFileSync(path.join(root,'reports/boot-smoke.txt'),(output+'\n'+errorOutput).trimEnd()+'\n');
  fs.writeFileSync(path.join(root,'reports/boot-runtime.json'),JSON.stringify({runtime_verified:ok,
    image_sha256:crypto.createHash('sha256').update(fs.readFileSync(image)).digest('hex'),
    emulator:'qemu',checks_completed:stage,reason,scope:smokeScope},null,2)+'\n');
  console.log(ok ? 'Boot smoke test passed: prompt, COM/CALL 5, FCB writes, COPY/REN/DEL.' : reason);
  process.exitCode=ok ? 0 : 1;
}
const timer=setTimeout(()=>finish(false,'Boot smoke test timed out; see reports/boot-smoke.txt.'),30000);
qemu.on('error',error=>finish(false,`Cannot run ${executable}: ${error.message}. Install QEMU or set QEMU to its executable path.`));
qemu.stderr.on('data',data=>{errorOutput+=data.toString();});
qemu.stdout.on('data',data=>{
  output+=data.toString().replaceAll('\r','');
  const section=output.slice(boundary),step=steps[stage];
  if(!step || !section.endsWith(shellPrompt)) return;
  if(!step.expected.every(text=>section.includes(text))) return finish(false,`Guest output failed at step ${stage}.`);
  stage++;boundary=output.length;
  if(!step.command) return finish(true,'All smoke checks passed.');
  qemu.stdin.write(step.command+'\r');
});
qemu.on('exit',(code,signal)=>{if(!finished) finish(false,`QEMU exited early (${code ?? signal}).`);});
