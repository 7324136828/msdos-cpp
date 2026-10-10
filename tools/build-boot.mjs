import fs from 'node:fs';
import path from 'node:path';
import { spawnSync } from 'node:child_process';
import crypto from 'node:crypto';
import { fileURLToPath } from 'node:url';
export const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
export function fatSet(bytes, cluster, value) {
  const p=cluster+Math.floor(cluster/2), word=bytes.readUInt16LE(p);
  bytes.writeUInt16LE(cluster&1 ? (word&15)|(value<<4) : (word&0xf000)|value,p);
}
export function makeImage(boot,kernel,files) {
  if(boot.length!==512 || boot.readUInt16LE(510)!==0xaa55) throw new Error('invalid boot sector');
  if(kernel.length>64*512) throw new Error('kernel exceeds 64 reserved sectors');
  if(files.length>224) throw new Error('root directory full');
  const image=Buffer.alloc(2880*512),fat=Buffer.alloc(9*512);
  boot.copy(image);kernel.copy(image,512);fat[0]=0xf0;fat[1]=fat[2]=255;
  let cluster=2;
  files.forEach((file,index)=> {
    if(!/^[A-Z0-9_]{1,8}\.[A-Z0-9_]{1,3}$/.test(file.name)) throw new Error(`invalid 8.3 name ${file.name}`);
    const [base,extension]=file.name.split('.');
    const entry=83*512+index*32;
    image.write(base.padEnd(8)+extension.padEnd(3),entry,'ascii');image[entry+11]=0x20;
    image.writeUInt16LE((2<<9)|(3<<5)|3,entry+24);image.writeUInt32LE(file.bytes.length,entry+28);
    if(file.bytes.length) image.writeUInt16LE(cluster,entry+26);
    const needed=Math.ceil(file.bytes.length/512);
    if(cluster+needed>2785) throw new Error('floppy image full');
    for(let n=0;n<needed;n++) {
      const current=cluster++;
      fatSet(fat,current,n+1===needed ? 0xfff : current+1);
      file.bytes.copy(image,(97+current-2)*512,n*512,Math.min((n+1)*512,file.bytes.length));
    }
  });
  fat.copy(image,65*512);fat.copy(image,74*512);
  return image;
}
function command(program,args) {
  const result=spawnSync(program,args,{cwd:root,encoding:'utf8'});
  if(result.error) throw result.error;
  if(result.status!==0) throw new Error(`${program} failed:\n${result.stdout}${result.stderr}`);
  if(result.stderr) process.stderr.write(result.stderr);
}
export function build() {
  const out=path.join(root,'build/boot');fs.mkdirSync(out,{recursive:true});
  command('nasm',['-f','bin','boot/boot.asm','-o',path.join(out,'boot.bin')]);
  // MinGW produces PE64 COFF containers even with -m16. The actual code is i386 real mode.
  command('nasm',['-f',process.platform==='win32' ? 'win64' : 'elf32','boot/entry.asm','-o',path.join(out,'entry.o')]);
  command('g++',['-m16','-march=i386','-Os','-std=c++20','-ffreestanding','-fno-exceptions','-fno-rtti',
    '-fno-stack-protector','-fno-pic','-fno-pie','-fno-asynchronous-unwind-tables','-fno-unwind-tables',
    '-fno-ident','-mno-sse','-mno-mmx','-Wall','-Wextra','-Werror','-c','boot/kernel.cpp','-o',path.join(out,'kernel.o')]);
  command('ld',[...(process.platform==='win32' ? ['--image-base','0','--disable-reloc-section'] : ['-m','elf_i386']),
    '--entry','_start','-T','boot/kernel.ld','-o',path.join(out,'kernel.linked'),path.join(out,'entry.o'),path.join(out,'kernel.o')]);
  command('objcopy',['-O','binary',path.join(out,'kernel.linked'),path.join(out,'kernel.bin')]);
  command('nasm',['-f','bin','boot/demo.asm','-o',path.join(out,'demo.com')]);
  command('nasm',['-f','bin','boot/rwtest.asm','-o',path.join(out,'rwtest.com')]);
  const files=[{name:'DEMO.COM',bytes:fs.readFileSync(path.join(out,'demo.com'))},
    {name:'RWTEST.COM',bytes:fs.readFileSync(path.join(out,'rwtest.com'))},
    {name:'README.TXT',bytes:Buffer.from('A FAT12 file read by the C++ DOS kernel.\r\n\x1a')}];
  // Unmodified historical DOS applications for compatibility experiments.
  for(const name of ['COMP.COM','CHKDSK.COM','EDLIN.COM']) {
    const original=path.join(root,'reference/v1.25/bin',name);
    if(fs.existsSync(original)) files.push({name,bytes:fs.readFileSync(original)});
  }
  const image=makeImage(fs.readFileSync(path.join(out,'boot.bin')),fs.readFileSync(path.join(out,'kernel.bin')),files);
  fs.writeFileSync(path.join(out,'msdos-cpp.img'),image);
  const imageHash=crypto.createHash('sha256').update(image).digest('hex');
  const evidence=['reports/boot-runtime-virtualbox.json','reports/boot-runtime.json'].filter(file=>{
    try {const result=JSON.parse(fs.readFileSync(path.join(root,file),'utf8'));
      return result.runtime_verified===true && result.image_sha256===imageHash;
    }catch{return false;}
  });
  const report={image:'build/boot/msdos-cpp.img',image_sha256:imageHash,
    kernel_bytes:fs.statSync(path.join(out,'kernel.bin')).size,files:files.map(file=>file.name),
    source_rewrite_complete:false,runtime_verified:evidence.length>0,runtime_evidence:evidence};
  fs.writeFileSync(path.join(out,'build-report.json'),JSON.stringify(report,null,2)+'\n');
  console.log(JSON.stringify(report,null,2));
}
if(process.argv[1] && path.resolve(process.argv[1])===fileURLToPath(import.meta.url)) {
  try {build();}catch(error){console.error(error.message);process.exitCode=1;}
}
