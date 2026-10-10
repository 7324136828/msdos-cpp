// Account for every DOS 4.0 ASM/INC/C/H file. Reference copies and wrappers
// are not counted as translated implementations. Full generated modules must
// reproduce exactly from their original source and declared macro dependencies.
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import {spawnSync} from 'node:child_process';
import {fileURLToPath} from 'node:url';
import {projectRoot,inventory,sourceText,generateUnit,loadUnitDependencies} from './convert.mjs';

const sha=bytes=>crypto.createHash('sha256').update(bytes).digest('hex');
export function classifySupportingInput(source,bytes) {
  const normalized=source.replaceAll('\\','/').toLowerCase();
  const extension=path.extname(normalized).slice(1);
  if(['mac','equ','ext','str','skl','lc','sw','def','stc','msg','on','off','tru','fal','inf'].includes(extension))
    return {role:'additional_source_input',reason:'Declaration, macro, assembly configuration, or message source input'};
  // A .LIB suffix does not establish that the file is an object library:
  // APPEND.ASM includes SYSMAC.LIB as source, while COMSUBS.LIB is OMF binary.
  const plainText=bytes.every(byte=>byte>=0x20 || [0x09,0x0a,0x0c,0x0d,0x1a].includes(byte));
  if(extension==='lib' && plainText && /^\s*[A-Za-z_?$@][\w?$@]*\s+MACRO\b/im.test(sourceText(bytes)))
    return {role:'additional_source_input',reason:'Textual assembler macro library'};
  if(['pro','prt'].includes(extension) || normalized==='src/select/usa.txt')
    return {role:'runtime_resource_input',reason:extension==='pro' ? 'GRAPHICS printer profile interpreted at runtime'
      : extension==='prt' ? 'SELECT printer profile interpreted at runtime' : 'SELECT help source converted by ASC2HLP'};
  if(['cpi','meu'].includes(extension))
    return {role:'supplied_binary_resource',reason:extension==='cpi' ? 'Compiled printer code-page data' : 'Compiled DOS Shell menu data'};
  if(['exe','com','sys','lib','obj','ob'].includes(extension))
    return {role:'supplied_binary',reason:'Supplied executable, object, or binary library'};
  return {role:'build_or_resource_file',reason:'Build input, documentation, or other supporting data'};
}
function allFiles(directory) {
  return fs.readdirSync(directory,{withFileTypes:true}).sort((a,b)=>a.name.localeCompare(b.name))
    .flatMap(entry=>entry.isDirectory() ? allFiles(path.join(directory,entry.name))
      : entry.isFile() ? [{path:path.relative(directory,path.join(directory,entry.name)),absolute:path.join(directory,entry.name)}] : [])
    .map(file=>({...file,path:path.relative(directory,file.absolute).replaceAll('\\','/')}));
}
export function auditV4({root=projectRoot,originalRoot=path.join(root,'../MS-DOS/v4.0'),validate=false,testExecutable}={}) {
  const referenceRoot=path.join(root,'reference/v4.0');
  const originalAvailable=fs.existsSync(originalRoot);
  const originals=originalAvailable ? inventory(originalRoot) : [];
  const references=inventory(referenceRoot);
  const originalMap=new Map(originals.map(file=>[file.path,file]));
  const referenceMap=new Map(references.map(file=>[file.path,file]));
  const allOriginals=new Map((originalAvailable ? allFiles(originalRoot) : []).map(file=>[file.path,{...file,sha256:sha(fs.readFileSync(file.absolute))}]));
  const allReferences=new Map(allFiles(referenceRoot).map(file=>[file.path,{...file,sha256:sha(fs.readFileSync(file.absolute))}]));
  const manifest=JSON.parse(fs.readFileSync(path.join(root,'tools/units.json'),'utf8'));
  const cmake=fs.readFileSync(path.join(root,'CMakeLists.txt'),'utf8');
  const units=manifest.units.filter(unit=>unit.source.startsWith('v4.0/'));
  const ports=new Map();
  const dependencies=new Set();
  for(const config of units) {
    const bytes=fs.readFileSync(path.join(root,'reference',config.source));
    const includes=loadUnitDependencies(config,path.join(root,'reference'));
    const translation=generateUnit(sourceText(bytes),config,sha(bytes),includes);
    const output='generated/'+config.output;
    if(fs.readFileSync(path.join(root,output),'utf8')!==translation.cpp)
      throw new Error(`Stale or modified C++ translation: ${output}`);
    if(!cmake.includes(output)) throw new Error(`Translation is missing from CMake: ${output}`);
    ports.set(config.source.slice('v4.0/'.length),{name:config.name,source_sha256:sha(bytes),
      cpp:output,cpp_sha256:sha(Buffer.from(translation.cpp)),complete_source:Boolean(config.program || config.module),
      execution_backend:'native_cpp_control_flow',
      exports:Object.keys(config.exports),instructions:translation.unit.code.length,abi:config.abi ?? 'near',
      dependencies:includes.map(item=>({source:item.source,sha256:sha(item.bytes)}))});
    for(const include of config.includes ?? []) dependencies.add(include.slice('v4.0/'.length));
  }
  const paths=[...new Set([...originalMap.keys(),...referenceMap.keys()])].sort();
  const files=paths.map(source=>{
    const original=originalMap.get(source),reference=referenceMap.get(source),file=original ?? reference;
    const port=ports.get(source);
    const parts=source.split('/');
    const subsystem=parts[1]==='CMD' || parts[1]==='DEV' ? parts.slice(1,3).join('/') : parts[1];
    return {source:'v4.0/'+source,kind:file.kind,subsystem,lines:file.lines,sha256:file.sha256,
      reference_present:Boolean(reference),original_present:originalAvailable ? Boolean(original) : null,
      reference_matches_original:originalAvailable ? Boolean(original && reference && original.sha256===reference.sha256) : null,
      translation:port ?? null,status:port?.complete_source ? 'complete_source_translated'
        : port ? 'partially_translated' : dependencies.has(source) ? 'expanded_macro_dependency' : 'not_translated',
      includes:file.includes,hardware_boundaries:file.hardware_boundaries.length};
  });
  const complete=files.filter(file=>file.status==='complete_source_translated');
  const pending=files.filter(file=>file.status!=='complete_source_translated');
  const allPaths=[...new Set([...allOriginals.keys(),...allReferences.keys()])].sort();
  const mismatches=allPaths.filter(file=>!allReferences.has(file) ||
    (originalAvailable && allOriginals.get(file)?.sha256!==allReferences.get(file)?.sha256));
  const supporting=allPaths.filter(file=>!originalMap.has(file) && !referenceMap.has(file)).map(file=>{
    const original=allOriginals.get(file),reference=allReferences.get(file),extension=path.extname(file).slice(1).toLowerCase();
    return {source:'v4.0/'+file,kind:extension || 'extensionless',sha256:(original ?? reference).sha256,
      ...classifySupportingInput(file,fs.readFileSync((original ?? reference).absolute)),
      status:'reference_only',reference_present:Boolean(reference),reference_matches_original:originalAvailable
        ? Boolean(original && reference && original.sha256===reference.sha256) : null};
  });
  const additional=supporting.filter(file=>file.role==='additional_source_input');
  const resources=supporting.filter(file=>file.role==='runtime_resource_input' || file.role==='supplied_binary_resource');
  const subsystems={};
  for(const file of files) {
    subsystems[file.subsystem] ??= {files:0,complete:0,pending:0,lines:0};
    const group=subsystems[file.subsystem];group.files++;group.lines+=file.lines;
    if(file.status==='complete_source_translated') group.complete++;else group.pending++;
  }
  let validation=null;
  if(validate) {
    const executable=testExecutable ? path.resolve(root,testExecutable) : path.join(root,'build',process.platform==='win32' ? 'msdos_v4_tests.exe' : 'msdos_v4_tests');
    if(!fs.existsSync(executable)) throw new Error('Build the project before running the validation audit.');
    const result=spawnSync(executable,[],{encoding:'utf8',windowsHide:true,timeout:30000});
    validation={passed:result.status===0,exit_code:result.status,output:(result.stdout ?? '')+(result.stderr ?? ''),
      executable_sha256:sha(fs.readFileSync(executable)),test_source_sha256:sha(fs.readFileSync(path.join(root,'tests/v4_mapper_tests.cpp'))),
      scope:'C++ mapper contracts with controlled DOS/BIOS service responses; no DOS 4.0 kernel or original-binary equivalence claim'};
    if(result.error) validation.error=result.error.message;
  }
  const report={schema:1,version:'4.0',original_root:path.relative(root,originalRoot).replaceAll('\\','/'),
    original_comparison_available:originalAvailable,source_files:files.length,complete_source_files:complete.length,
    pending_source_files:pending.length,all_original_files:originalAvailable ? allOriginals.size : null,
    all_reference_files:allReferences.size,additional_source_inputs:additional.length,runtime_resource_inputs:resources.length,
    reference_mismatches:mismatches.map(file=>'v4.0/'+file),
    whole_dos4_port_complete:pending.length===0 && additional.length===0 && mismatches.length===0,
    boot_image_version:'1.25 compatibility kernel; DOS 4.0 mapper translations are host-library modules',
    validation,subsystems,files,supporting_files:supporting};
  const out=path.join(root,'reports');fs.mkdirSync(out,{recursive:true});
  fs.writeFileSync(path.join(out,'v4-audit.json'),JSON.stringify(report,null,2)+'\n');
  const md=['# MS-DOS 4.0 C++ translation audit','',
    `${files.length} ASM/INC/C/H source files: **${complete.length} fully transcribed module bodies**, **${pending.length} still pending**.`,
    `${additional.length} additional macro, declaration, assembly configuration and message source inputs are also listed below; these remain reference-only.`,
    `${resources.length} runtime/help resource inputs must also be preserved or rebuilt by their owning modules.`,
    originalAvailable ? `All ${allOriginals.size} original/reference file hashes compared; ${mismatches.length} missing or differing copies.` :
      'Original sibling tree unavailable; source-copy comparison was not performed.',
    '', 'Complete module transcription means all source instructions and used macros generate buildable C++; it does not establish a complete DOS service implementation or binary equivalence.',
    'The boot image still uses the DOS 1.25 compatibility kernel. DOS 4.0 mapper modules run in the host C++ library against explicit DOS/BIOS interrupt boundaries.',
    validation ? `Validation: ${validation.passed ? 'passed' : 'FAILED'} — ${validation.scope}.` : 'Validation was not run by this audit.',
    '', '| Subsystem | Source files | Complete | Pending | Lines |','| --- | ---: | ---: | ---: | ---: |',
    ...Object.entries(subsystems).sort(([a],[b])=>a.localeCompare(b)).map(([name,count])=>
      `| ${name} | ${count.files} | ${count.complete} | ${count.pending} | ${count.lines} |`),
    '', '## Completed modules','', '| Original source | C++ translation | Instructions |','| --- | --- | ---: |',
    ...complete.map(file=>`| [${file.source}](../reference/${file.source}) | [${file.translation.cpp}](../${file.translation.cpp}) | ${file.translation.instructions} |`),
    '', '## Files still needing full translation','',
    'Include files, macro dependencies, configuration wrappers and original C/H files remain visible in this list; copying or renaming them does not make them C++ ports.',
    '', '| Original source | Kind | Lines | Status |','| --- | --- | ---: | --- |',
    ...pending.map(file=>`| [${file.source}](../reference/${file.source}) | ${file.kind} | ${file.lines} | ${file.status} |`),
    '', '## Additional source inputs','',
    'The older inventory only counted ASM/INC/C/H. These macro, declaration, assembly configuration and message inputs must also be handled when porting their owning modules. In particular, SYSMAC.LIB contains source macros rather than binary objects, and USA.INF is assembled through PANEL.INF.',
    '', '| Original source input | Kind | Status |','| --- | --- | --- |',
    ...additional.map(file=>`| [${file.source}](../reference/${file.source}) | ${file.kind} | ${file.status} |`),
    '', '## Runtime and help resources','',
    'These printer profiles, help sources, code-page data and menu data are separate from executable C++ translation. Preserve their formats or regenerate them for the ported consumers.',
    '', '| Original resource | Kind | Role |','| --- | --- | --- |',
    ...resources.map(file=>`| [${file.source}](../reference/${file.source}) | ${file.kind} | ${file.reason} |`),
    '', 'The JSON report also accounts for every build/resource file and supplied binary. Binary components without corresponding source require source recovery or a separately scoped reimplementation; they are not counted as completed C++ translations.',''];
  fs.writeFileSync(path.join(out,'v4-missing.md'),md.join('\n'));
  if(mismatches.length) throw new Error('DOS 4.0 reference copies differ from the original; see reports/v4-audit.json.');
  if(validate && !validation.passed) throw new Error('DOS 4.0 mapper validation failed; see reports/v4-audit.json.');
  return {source_files:files.length,complete_source_files:complete.length,pending_source_files:pending.length,
    additional_source_inputs:additional.length,runtime_resource_inputs:resources.length,all_reference_files:allReferences.size,
    reference_mismatches:mismatches.length,validation_passed:validation?.passed ?? null};
}
if(process.argv[1] && path.resolve(process.argv[1])===fileURLToPath(import.meta.url)) {
  try {
    const index=process.argv.indexOf('--test-executable');
    if(index>=0 && !process.argv[index+1]) throw new Error('--test-executable requires a path');
    console.log(JSON.stringify(auditV4({validate:process.argv.includes('--validate'),testExecutable:index>=0 ? process.argv[index+1] : undefined}),null,2));
  }
  catch(error){console.error(error.message);process.exitCode=1;}
}
