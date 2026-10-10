// Optional, workspace-local emulator setup. Never installs system-wide packages.
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import {root} from './build-boot.mjs';
const manifest=JSON.parse(fs.readFileSync(path.join(root,'tools/v86-assets.json'),'utf8'));
const directory=path.join(root,'build/emulator/v86');
fs.mkdirSync(directory,{recursive:true});
for(const asset of manifest.assets) {
  const target=path.join(directory,asset.name);
  const sha=bytes=>crypto.createHash('sha256').update(bytes).digest('hex');
  if(fs.existsSync(target) && sha(fs.readFileSync(target))===asset.sha256) continue;
  const response=await fetch(asset.url,{signal:AbortSignal.timeout(30000)});
  if(!response.ok) throw new Error(`${asset.url}: HTTP ${response.status}`);
  const bytes=Buffer.from(await response.arrayBuffer());
  if(sha(bytes)!==asset.sha256) throw new Error(`${asset.name}: published asset changed; review and update tools/v86-assets.json.`);
  fs.writeFileSync(target,bytes);
  console.log(`Downloaded ${asset.name}`);
}
console.log('Local v86 emulator ready. Run node tools/boot-smoke-v86.mjs.');
