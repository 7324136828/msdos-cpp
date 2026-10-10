// Shared guest checks: every command must return to the shell prompt.
export const shellPrompt='A:\\> ';
export const smokeCases = [
  {expected:['Your DOS workspace','DOS 1.25 compatibility'],command:'DIR'},
  {expected:['DEMO.COM','README.TXT'],command:'TYPE README.TXT'},
  {expected:['A FAT12 file read by the C++ DOS kernel.'],command:'DEMO README.TXT'},
  {expected:['DOS .COM execution and INT 21h work.','A FAT12 file read by the C++ DOS kernel.'],command:'DEMO README.TXT'},
  {expected:['DOS .COM execution and INT 21h work.','A FAT12 file read by the C++ DOS kernel.'],command:'RWTEST'},
  {expected:['CALL 5 dispatch works.','FCB create/write/read/truncate/rename/delete passed.',
    'Absolute sector read and flags-stack cleanup passed.'],command:'COPY README.TXT COPY.TXT'},
  {expected:['1 File(s) copied'],command:'TYPE COPY.TXT'},
  {expected:['A FAT12 file read by the C++ DOS kernel.'],command:'REN COPY.TXT RENAMED.TXT'},
  {expected:[],command:'TYPE RENAMED.TXT'},
  {expected:['A FAT12 file read by the C++ DOS kernel.'],command:'DEL RENAMED.TXT'},
  {expected:[],command:'TYPE RENAMED.TXT'},
  {expected:['File not found'],command:'VER'},
  {expected:['DOS 1.25 compatibility'],command:'HELP'},
  {expected:['Commands','Keyboard','Ctrl+L clear screen'],command:'CLS'},
  {expected:[],command:'HISTORY'},
  {expected:['VER','HELP','CLS','HISTORY']},
];
export const smokeScope='modern terminal prompt, DIR, TYPE, repeated native COM execution, CALL 5, FCB writes, INT 25h, shell COPY/REN/DEL, VER/HELP/CLS/HISTORY';
