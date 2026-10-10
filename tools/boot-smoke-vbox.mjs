// Boot the existing image with an installed VirtualBox. Use a dedicated VM
// and a disposable floppy copy; record serial output and display screenshots.
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import {spawnSync} from 'node:child_process';
import {root} from './build-boot.mjs';
import {smokeCases,smokeScope,shellPrompt} from './boot-smoke-cases.mjs';

const executable=process.env.VBOXMANAGE ?? (process.platform==='win32'
  ? path.join(process.env.ProgramFiles ?? 'C:/Program Files','Oracle/VirtualBox/VBoxManage.exe') : 'VBoxManage');
const directory=path.join(root,'build/virtualbox');
const reports=path.join(root,'reports');
const image=fs.readFileSync(path.join(root,'build/boot/msdos-cpp.img'));
const copy=path.join(directory,'msdos-cpp-test.img');
const serial=path.join(directory,'serial.log');
const name='MS-DOS-cpp';
const stateFile=path.join(directory,'vm.json');
const digest=bytes=>crypto.createHash('sha256').update(bytes).digest('hex');
const sleep=ms=>new Promise(resolve=>setTimeout(resolve,ms));
function command(...args) {
  const result=spawnSync(executable,args,{encoding:'utf8',windowsHide:true,timeout:30000});
  if(result.error) throw result.error;
  if(result.status!==0) throw new Error(`VBoxManage ${args[0]} failed: ${(result.stdout+result.stderr).trim()}`);
  return result.stdout.trim();
}
function info(uuid) {
  return Object.fromEntries(command('showvminfo',uuid,'--machinereadable').split(/\r?\n/)
    .map(line=>/^([^=]+)="(.*)"$/.exec(line)).filter(Boolean).map(([,key,value])=>[key,value]));
}
fs.mkdirSync(directory,{recursive:true});fs.mkdirSync(reports,{recursive:true});
let uuid,prepared=false,started=false,stage=0,output='',version='',reason='',ok=false;
try {
  version=command('--version');
  if(fs.existsSync(stateFile)) {
    ({uuid}=JSON.parse(fs.readFileSync(stateFile,'utf8')));
    if(!command('list','vms').includes(`{${uuid}}`))
      throw new Error('MS-DOS-cpp is not registered for this Windows account. Run this test from the same normal Windows account as VirtualBox Manager. Existing boot reports have been preserved.');
    const state=info(uuid);
    if(path.resolve(state.CfgFile)!==path.join(directory,name,`${name}.vbox`))
      throw new Error('Saved VM does not belong to this workspace.');
    if(state.VMState!=='poweroff' && state.VMState!=='aborted')
      throw new Error('The test VM is active. Shut it down before repeating the smoke test.');
  } else {
    if(command('list','vms').split(/\r?\n/).some(line=>line.startsWith(`"${name}" `)))
      throw new Error('A VM named MS-DOS-cpp already exists; refusing to modify an unrelated VM.');
    const created=command('createvm','--name',name,'--ostype','DOS','--basefolder',directory,'--register');
    uuid=/UUID:\s*([\da-f-]+)/i.exec(created)?.[1];
    if(!uuid) throw new Error('VirtualBox did not return a VM UUID.');
    fs.writeFileSync(stateFile,JSON.stringify({name,uuid,settings:path.join(directory,name,`${name}.vbox`)},null,2)+'\n');
    command('storagectl',uuid,'--name','Floppy','--add','floppy','--controller','I82078');
  }
  prepared=true;
  fs.writeFileSync(copy,image);fs.writeFileSync(serial,'');
  command('modifyvm',uuid,'--memory','16','--vram','4','--firmware','bios','--acpi','off',
    '--ioapic','off','--paravirt-provider','none','--graphicscontroller','vboxvga',
    '--boot1','floppy','--boot2','none','--boot3','none','--boot4','none',
    '--nic1','none','--audio-enabled','off','--uart1','0x3f8','4','--uart-mode1','file',serial);
  command('storageattach',uuid,'--storagectl','Floppy','--port','0','--device','0','--type','fdd','--medium',copy);
  console.log(`Starting ${name} with VirtualBox ${version}.`);
  command('startvm',uuid,'--type','headless');started=true;
  const deadline=Date.now()+60000;
  let boundary=0;
  while(stage<smokeCases.length) {
    if(Date.now()>deadline) throw new Error(`VirtualBox boot test timed out at check ${stage}.`);
    output=fs.readFileSync(serial,'utf8').replaceAll('\r','');
    const section=output.slice(boundary);
    if(!section.endsWith(shellPrompt)) {await sleep(100);continue;}
    const step=smokeCases[stage];
    if(!step.expected.every(value=>section.includes(value))) throw new Error(`Guest output failed at check ${stage}.`);
    if(stage===0 || stage===5) command('controlvm',uuid,'screenshotpng',path.join(reports,
      stage===0 ? 'virtualbox-boot.png' : 'virtualbox-rwtest.png'));
    stage++;boundary=output.length;
    if(!step.command) break;
    command('controlvm',uuid,'keyboardputstring',step.command);
    command('controlvm',uuid,'keyboardputscancode','1c','9c');
  }
  ok=true;reason='VirtualBox boot and all guest smoke checks passed.';
} catch(error) {
  reason=error.message;
} finally {
  if(started) {
    try {command('controlvm',uuid,'screenshotpng',path.join(reports,'virtualbox-final.png'));} catch {}
    try {command('controlvm',uuid,'poweroff');} catch(error) {ok=false;reason+=' '+error.message;}
  }
  if(prepared) {
    if(fs.existsSync(serial)) output=fs.readFileSync(serial,'utf8').replaceAll('\r','');
    fs.writeFileSync(path.join(reports,'boot-smoke-virtualbox.txt'),output.trimEnd()+'\n');
    const report={runtime_verified:ok,emulator:'VirtualBox',virtualbox_version:version,vm_name:name,vm_uuid:uuid,
      image_sha256:digest(image),checks_completed:stage,reason,scope:smokeScope,
      floppy_copy:'build/virtualbox/msdos-cpp-test.img',vm_settings:`build/virtualbox/${name}/${name}.vbox`,
      screenshot:'reports/virtualbox-boot.png'};
    fs.writeFileSync(path.join(reports,'boot-runtime-virtualbox.json'),JSON.stringify(report,null,2)+'\n');
  }
  console.log(output);console.log(reason);process.exitCode=ok ? 0 : 1;
}
