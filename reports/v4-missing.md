# MS-DOS 4.0 C++ translation audit

973 ASM/INC/C/H source files: **30 fully transcribed module bodies**, **943 still pending**.
103 additional macro, declaration, assembly configuration and message source inputs are also listed below; these remain reference-only.
7 runtime/help resource inputs must also be preserved or rebuilt by their owning modules.
Original sibling tree unavailable; source-copy comparison was not performed.

Complete module transcription means all source instructions and used macros generate buildable C++; it does not establish a complete DOS service implementation or binary equivalence.
The boot image still uses the DOS 1.25 compatibility kernel. DOS 4.0 mapper modules run in the host C++ library against explicit DOS/BIOS interrupt boundaries.
Validation: passed — C++ mapper contracts with controlled DOS/BIOS service responses; no DOS 4.0 kernel or original-binary equivalence claim.

| Subsystem | Source files | Complete | Pending | Lines |
| --- | ---: | ---: | ---: | ---: |
| BIOS | 35 | 0 | 35 | 21417 |
| BOOT | 1 | 0 | 1 | 530 |
| CMD/APPEND | 3 | 0 | 3 | 3646 |
| CMD/ASSIGN | 4 | 0 | 4 | 2229 |
| CMD/ATTRIB | 7 | 0 | 7 | 4197 |
| CMD/BACKUP | 5 | 0 | 5 | 5501 |
| CMD/CHKDSK | 17 | 0 | 17 | 8907 |
| CMD/COMMAND | 39 | 0 | 39 | 19870 |
| CMD/COMP | 7 | 0 | 7 | 3163 |
| CMD/DEBUG | 13 | 0 | 13 | 10950 |
| CMD/DISKCOMP | 7 | 0 | 7 | 4295 |
| CMD/DISKCOPY | 7 | 0 | 7 | 4636 |
| CMD/EDLIN | 7 | 0 | 7 | 4845 |
| CMD/EXE2BIN | 9 | 0 | 9 | 2405 |
| CMD/FASTOPEN | 6 | 0 | 6 | 8266 |
| CMD/FC | 16 | 0 | 16 | 1943 |
| CMD/FDISK | 38 | 0 | 38 | 14583 |
| CMD/FILESYS | 7 | 0 | 7 | 1662 |
| CMD/FIND | 3 | 0 | 3 | 1784 |
| CMD/FORMAT | 14 | 0 | 14 | 11592 |
| CMD/GRAFTABL | 11 | 0 | 11 | 8460 |
| CMD/GRAPHICS | 14 | 0 | 14 | 9591 |
| CMD/IFSFUNC | 12 | 0 | 12 | 12141 |
| CMD/JOIN | 4 | 0 | 4 | 1198 |
| CMD/KEYB | 25 | 0 | 25 | 8409 |
| CMD/LABEL | 2 | 0 | 2 | 1443 |
| CMD/MEM | 5 | 0 | 5 | 2344 |
| CMD/MODE | 22 | 0 | 22 | 14737 |
| CMD/MORE | 2 | 0 | 2 | 305 |
| CMD/NLSFUNC | 6 | 0 | 6 | 2145 |
| CMD/PRINT | 6 | 0 | 6 | 7818 |
| CMD/RECOVER | 13 | 0 | 13 | 4520 |
| CMD/REPLACE | 5 | 0 | 5 | 2722 |
| CMD/RESTORE | 23 | 0 | 23 | 7757 |
| CMD/SHARE | 5 | 0 | 5 | 5744 |
| CMD/SORT | 2 | 0 | 2 | 1086 |
| CMD/SUBST | 4 | 0 | 4 | 1247 |
| CMD/SYS | 4 | 0 | 4 | 6842 |
| CMD/TREE | 6 | 0 | 6 | 2823 |
| CMD/XCOPY | 3 | 0 | 3 | 6244 |
| DEV/ANSI | 6 | 0 | 6 | 3404 |
| DEV/COUNTRY | 2 | 0 | 2 | 2519 |
| DEV/DISPLAY | 41 | 0 | 41 | 13182 |
| DEV/DRIVER | 1 | 0 | 1 | 1297 |
| DEV/KEYBOARD | 27 | 0 | 27 | 32174 |
| DEV/PRINTER | 14 | 0 | 14 | 15474 |
| DEV/RAMDRIVE | 10 | 0 | 10 | 7156 |
| DEV/SMARTDRV | 13 | 0 | 13 | 10299 |
| DEV/VDISK | 5 | 0 | 5 | 5340 |
| DEV/XMA2EMS | 12 | 0 | 12 | 15068 |
| DEV/XMAEM | 15 | 0 | 15 | 6911 |
| DOS | 83 | 0 | 83 | 34087 |
| H | 19 | 0 | 19 | 3021 |
| INC | 80 | 0 | 80 | 25830 |
| MAPPER | 63 | 30 | 33 | 6551 |
| MEMM | 67 | 0 | 67 | 30282 |
| SELECT | 64 | 0 | 64 | 37727 |
| TOOLS | 32 | 0 | 32 | 2303 |

## Completed modules

| Original source | C++ translation | Instructions |
| --- | --- | ---: |
| [v4.0/src/MAPPER/ALLOCSEG.ASM](../reference/v4.0/src/MAPPER/ALLOCSEG.ASM) | [generated/v4.0/mapper/allocseg.cpp](../generated/v4.0/mapper/allocseg.cpp) | 42 |
| [v4.0/src/MAPPER/CASEMAP.ASM](../reference/v4.0/src/MAPPER/CASEMAP.ASM) | [generated/v4.0/mapper/casemap.cpp](../generated/v4.0/mapper/casemap.cpp) | 35 |
| [v4.0/src/MAPPER/CHDIR.ASM](../reference/v4.0/src/MAPPER/CHDIR.ASM) | [generated/v4.0/mapper/chdir.cpp](../generated/v4.0/mapper/chdir.cpp) | 31 |
| [v4.0/src/MAPPER/CLOSE.ASM](../reference/v4.0/src/MAPPER/CLOSE.ASM) | [generated/v4.0/mapper/close.cpp](../generated/v4.0/mapper/close.cpp) | 31 |
| [v4.0/src/MAPPER/DBCS.ASM](../reference/v4.0/src/MAPPER/DBCS.ASM) | [generated/v4.0/mapper/dbcs.cpp](../generated/v4.0/mapper/dbcs.cpp) | 34 |
| [v4.0/src/MAPPER/DELETE.ASM](../reference/v4.0/src/MAPPER/DELETE.ASM) | [generated/v4.0/mapper/delete.cpp](../generated/v4.0/mapper/delete.cpp) | 28 |
| [v4.0/src/MAPPER/DEVCONFG.ASM](../reference/v4.0/src/MAPPER/DEVCONFG.ASM) | [generated/v4.0/mapper/devconfg.cpp](../generated/v4.0/mapper/devconfg.cpp) | 77 |
| [v4.0/src/MAPPER/EXIT.ASM](../reference/v4.0/src/MAPPER/EXIT.ASM) | [generated/v4.0/mapper/exit.cpp](../generated/v4.0/mapper/exit.cpp) | 30 |
| [v4.0/src/MAPPER/FREESEG.ASM](../reference/v4.0/src/MAPPER/FREESEG.ASM) | [generated/v4.0/mapper/freeseg.cpp](../generated/v4.0/mapper/freeseg.cpp) | 30 |
| [v4.0/src/MAPPER/GETVER.ASM](../reference/v4.0/src/MAPPER/GETVER.ASM) | [generated/v4.0/mapper/getver.cpp](../generated/v4.0/mapper/getver.cpp) | 29 |
| [v4.0/src/MAPPER/GET_TOD.ASM](../reference/v4.0/src/MAPPER/GET_TOD.ASM) | [generated/v4.0/mapper/get_tod.cpp](../generated/v4.0/mapper/get_tod.cpp) | 38 |
| [v4.0/src/MAPPER/IOCTL.ASM](../reference/v4.0/src/MAPPER/IOCTL.ASM) | [generated/v4.0/mapper/ioctl.cpp](../generated/v4.0/mapper/ioctl.cpp) | 42 |
| [v4.0/src/MAPPER/LSEEK.ASM](../reference/v4.0/src/MAPPER/LSEEK.ASM) | [generated/v4.0/mapper/lseek.cpp](../generated/v4.0/mapper/lseek.cpp) | 36 |
| [v4.0/src/MAPPER/MKDIR.ASM](../reference/v4.0/src/MAPPER/MKDIR.ASM) | [generated/v4.0/mapper/mkdir.cpp](../generated/v4.0/mapper/mkdir.cpp) | 28 |
| [v4.0/src/MAPPER/PUTMSG.ASM](../reference/v4.0/src/MAPPER/PUTMSG.ASM) | [generated/v4.0/mapper/putmsg.cpp](../generated/v4.0/mapper/putmsg.cpp) | 30 |
| [v4.0/src/MAPPER/QFILEMOD.ASM](../reference/v4.0/src/MAPPER/QFILEMOD.ASM) | [generated/v4.0/mapper/qfilemod.cpp](../generated/v4.0/mapper/qfilemod.cpp) | 32 |
| [v4.0/src/MAPPER/QHANDTYP.ASM](../reference/v4.0/src/MAPPER/QHANDTYP.ASM) | [generated/v4.0/mapper/qhandtyp.cpp](../generated/v4.0/mapper/qhandtyp.cpp) | 33 |
| [v4.0/src/MAPPER/QVERIFY.ASM](../reference/v4.0/src/MAPPER/QVERIFY.ASM) | [generated/v4.0/mapper/qverify.cpp](../generated/v4.0/mapper/qverify.cpp) | 29 |
| [v4.0/src/MAPPER/READ.ASM](../reference/v4.0/src/MAPPER/READ.ASM) | [generated/v4.0/mapper/read.cpp](../generated/v4.0/mapper/read.cpp) | 32 |
| [v4.0/src/MAPPER/REALLSEG.ASM](../reference/v4.0/src/MAPPER/REALLSEG.ASM) | [generated/v4.0/mapper/reallseg.cpp](../generated/v4.0/mapper/reallseg.cpp) | 37 |
| [v4.0/src/MAPPER/RMDIR.ASM](../reference/v4.0/src/MAPPER/RMDIR.ASM) | [generated/v4.0/mapper/rmdir.cpp](../generated/v4.0/mapper/rmdir.cpp) | 28 |
| [v4.0/src/MAPPER/SCROLLUP.ASM](../reference/v4.0/src/MAPPER/SCROLLUP.ASM) | [generated/v4.0/mapper/scrollup.cpp](../generated/v4.0/mapper/scrollup.cpp) | 67 |
| [v4.0/src/MAPPER/SCURPOS.ASM](../reference/v4.0/src/MAPPER/SCURPOS.ASM) | [generated/v4.0/mapper/scurpos.cpp](../generated/v4.0/mapper/scurpos.cpp) | 55 |
| [v4.0/src/MAPPER/SEL_DISK.ASM](../reference/v4.0/src/MAPPER/SEL_DISK.ASM) | [generated/v4.0/mapper/sel_disk.cpp](../generated/v4.0/mapper/sel_disk.cpp) | 28 |
| [v4.0/src/MAPPER/SET_TOD.ASM](../reference/v4.0/src/MAPPER/SET_TOD.ASM) | [generated/v4.0/mapper/set_tod.cpp](../generated/v4.0/mapper/set_tod.cpp) | 45 |
| [v4.0/src/MAPPER/SFILEINF.ASM](../reference/v4.0/src/MAPPER/SFILEINF.ASM) | [generated/v4.0/mapper/sfileinf.cpp](../generated/v4.0/mapper/sfileinf.cpp) | 31 |
| [v4.0/src/MAPPER/SFILEMOD.ASM](../reference/v4.0/src/MAPPER/SFILEMOD.ASM) | [generated/v4.0/mapper/sfilemod.cpp](../generated/v4.0/mapper/sfilemod.cpp) | 30 |
| [v4.0/src/MAPPER/SVERIFY.ASM](../reference/v4.0/src/MAPPER/SVERIFY.ASM) | [generated/v4.0/mapper/sverify.cpp](../generated/v4.0/mapper/sverify.cpp) | 31 |
| [v4.0/src/MAPPER/WCHSTRA.ASM](../reference/v4.0/src/MAPPER/WCHSTRA.ASM) | [generated/v4.0/mapper/wchstra.cpp](../generated/v4.0/mapper/wchstra.cpp) | 113 |
| [v4.0/src/MAPPER/WRITE.ASM](../reference/v4.0/src/MAPPER/WRITE.ASM) | [generated/v4.0/mapper/write.cpp](../generated/v4.0/mapper/write.cpp) | 32 |

## Files still needing full translation

Include files, macro dependencies, configuration wrappers and original C/H files remain visible in this list; copying or renaming them does not make them C++ ports.

| Original source | Kind | Lines | Status |
| --- | --- | ---: | --- |
| [v4.0/src/BIOS/BIOSTRUC.INC](../reference/v4.0/src/BIOS/BIOSTRUC.INC) | inc | 104 | not_translated |
| [v4.0/src/BIOS/CLOCKSUB.INC](../reference/v4.0/src/BIOS/CLOCKSUB.INC) | inc | 79 | not_translated |
| [v4.0/src/BIOS/CMOSEQU.INC](../reference/v4.0/src/BIOS/CMOSEQU.INC) | inc | 50 | not_translated |
| [v4.0/src/BIOS/DEFEMS.INC](../reference/v4.0/src/BIOS/DEFEMS.INC) | inc | 33 | not_translated |
| [v4.0/src/BIOS/DEVMARK.INC](../reference/v4.0/src/BIOS/DEVMARK.INC) | inc | 24 | not_translated |
| [v4.0/src/BIOS/JUMPMAC.INC](../reference/v4.0/src/BIOS/JUMPMAC.INC) | inc | 31 | not_translated |
| [v4.0/src/BIOS/MS96TPI.INC](../reference/v4.0/src/BIOS/MS96TPI.INC) | inc | 537 | not_translated |
| [v4.0/src/BIOS/MSAUX.ASM](../reference/v4.0/src/BIOS/MSAUX.ASM) | asm | 282 | not_translated |
| [v4.0/src/BIOS/MSBDATA.INC](../reference/v4.0/src/BIOS/MSBDATA.INC) | inc | 722 | not_translated |
| [v4.0/src/BIOS/MSBIO1.ASM](../reference/v4.0/src/BIOS/MSBIO1.ASM) | asm | 646 | not_translated |
| [v4.0/src/BIOS/MSBIO2.ASM](../reference/v4.0/src/BIOS/MSBIO2.ASM) | asm | 573 | not_translated |
| [v4.0/src/BIOS/MSCLOCK.ASM](../reference/v4.0/src/BIOS/MSCLOCK.ASM) | asm | 296 | not_translated |
| [v4.0/src/BIOS/MSCON.ASM](../reference/v4.0/src/BIOS/MSCON.ASM) | asm | 329 | not_translated |
| [v4.0/src/BIOS/MSDISK.ASM](../reference/v4.0/src/BIOS/MSDISK.ASM) | asm | 2444 | not_translated |
| [v4.0/src/BIOS/MSDSKPR.INC](../reference/v4.0/src/BIOS/MSDSKPR.INC) | inc | 23 | not_translated |
| [v4.0/src/BIOS/MSEQU.INC](../reference/v4.0/src/BIOS/MSEQU.INC) | inc | 77 | not_translated |
| [v4.0/src/BIOS/MSEXTRN.INC](../reference/v4.0/src/BIOS/MSEXTRN.INC) | inc | 98 | not_translated |
| [v4.0/src/BIOS/MSGROUP.INC](../reference/v4.0/src/BIOS/MSGROUP.INC) | inc | 46 | not_translated |
| [v4.0/src/BIOS/MSHARD.ASM](../reference/v4.0/src/BIOS/MSHARD.ASM) | asm | 428 | not_translated |
| [v4.0/src/BIOS/MSINIT.ASM](../reference/v4.0/src/BIOS/MSINIT.ASM) | asm | 2819 | not_translated |
| [v4.0/src/BIOS/MSIOCTL.INC](../reference/v4.0/src/BIOS/MSIOCTL.INC) | inc | 1362 | not_translated |
| [v4.0/src/BIOS/MSLOAD.ASM](../reference/v4.0/src/BIOS/MSLOAD.ASM) | asm | 1091 | not_translated |
| [v4.0/src/BIOS/MSLOAD.INC](../reference/v4.0/src/BIOS/MSLOAD.INC) | inc | 8 | not_translated |
| [v4.0/src/BIOS/MSLPT.ASM](../reference/v4.0/src/BIOS/MSLPT.ASM) | asm | 271 | not_translated |
| [v4.0/src/BIOS/MSMACRO.INC](../reference/v4.0/src/BIOS/MSMACRO.INC) | inc | 193 | not_translated |
| [v4.0/src/BIOS/MSSTACK.INC](../reference/v4.0/src/BIOS/MSSTACK.INC) | inc | 307 | not_translated |
| [v4.0/src/BIOS/MSVOLID.INC](../reference/v4.0/src/BIOS/MSVOLID.INC) | inc | 298 | not_translated |
| [v4.0/src/BIOS/PSOPTION.INC](../reference/v4.0/src/BIOS/PSOPTION.INC) | inc | 63 | not_translated |
| [v4.0/src/BIOS/PUSHPOP.INC](../reference/v4.0/src/BIOS/PUSHPOP.INC) | inc | 21 | not_translated |
| [v4.0/src/BIOS/READCLOC.INC](../reference/v4.0/src/BIOS/READCLOC.INC) | inc | 166 | not_translated |
| [v4.0/src/BIOS/STKINIT.INC](../reference/v4.0/src/BIOS/STKINIT.INC) | inc | 272 | not_translated |
| [v4.0/src/BIOS/SYSCONF.ASM](../reference/v4.0/src/BIOS/SYSCONF.ASM) | asm | 3392 | not_translated |
| [v4.0/src/BIOS/SYSIMES.ASM](../reference/v4.0/src/BIOS/SYSIMES.ASM) | asm | 39 | not_translated |
| [v4.0/src/BIOS/SYSINIT1.ASM](../reference/v4.0/src/BIOS/SYSINIT1.ASM) | asm | 2668 | not_translated |
| [v4.0/src/BIOS/SYSINIT2.ASM](../reference/v4.0/src/BIOS/SYSINIT2.ASM) | asm | 1625 | not_translated |
| [v4.0/src/BOOT/MSBOOT.ASM](../reference/v4.0/src/BOOT/MSBOOT.ASM) | asm | 530 | not_translated |
| [v4.0/src/CMD/APPEND/APPEND.ASM](../reference/v4.0/src/CMD/APPEND/APPEND.ASM) | asm | 3462 | not_translated |
| [v4.0/src/CMD/APPEND/APPENDM.ASM](../reference/v4.0/src/CMD/APPEND/APPENDM.ASM) | asm | 37 | not_translated |
| [v4.0/src/CMD/APPEND/APPENDP.INC](../reference/v4.0/src/CMD/APPEND/APPENDP.INC) | inc | 147 | not_translated |
| [v4.0/src/CMD/ASSIGN/ASSGMAIN.ASM](../reference/v4.0/src/CMD/ASSIGN/ASSGMAIN.ASM) | asm | 1771 | not_translated |
| [v4.0/src/CMD/ASSIGN/ASSGMSG.INC](../reference/v4.0/src/CMD/ASSIGN/ASSGMSG.INC) | inc | 74 | not_translated |
| [v4.0/src/CMD/ASSIGN/ASSGPARM.ASM](../reference/v4.0/src/CMD/ASSIGN/ASSGPARM.ASM) | asm | 141 | not_translated |
| [v4.0/src/CMD/ASSIGN/ASSGPARM.INC](../reference/v4.0/src/CMD/ASSIGN/ASSGPARM.INC) | inc | 243 | not_translated |
| [v4.0/src/CMD/ATTRIB/ATTRIB.C](../reference/v4.0/src/CMD/ATTRIB/ATTRIB.C) | c | 2686 | not_translated |
| [v4.0/src/CMD/ATTRIB/ATTRIB.H](../reference/v4.0/src/CMD/ATTRIB/ATTRIB.H) | h | 444 | not_translated |
| [v4.0/src/CMD/ATTRIB/ATTRIBA.ASM](../reference/v4.0/src/CMD/ATTRIB/ATTRIBA.ASM) | asm | 480 | not_translated |
| [v4.0/src/CMD/ATTRIB/MSGRET.H](../reference/v4.0/src/CMD/ATTRIB/MSGRET.H) | h | 61 | not_translated |
| [v4.0/src/CMD/ATTRIB/PARSE.H](../reference/v4.0/src/CMD/ATTRIB/PARSE.H) | h | 186 | not_translated |
| [v4.0/src/CMD/ATTRIB/_MSGRET.ASM](../reference/v4.0/src/CMD/ATTRIB/_MSGRET.ASM) | asm | 233 | not_translated |
| [v4.0/src/CMD/ATTRIB/_PARSE.ASM](../reference/v4.0/src/CMD/ATTRIB/_PARSE.ASM) | asm | 107 | not_translated |
| [v4.0/src/CMD/BACKUP/BACKPARS.H](../reference/v4.0/src/CMD/BACKUP/BACKPARS.H) | h | 224 | not_translated |
| [v4.0/src/CMD/BACKUP/BACKUP.C](../reference/v4.0/src/CMD/BACKUP/BACKUP.C) | c | 4385 | not_translated |
| [v4.0/src/CMD/BACKUP/BACKUP.H](../reference/v4.0/src/CMD/BACKUP/BACKUP.H) | h | 503 | not_translated |
| [v4.0/src/CMD/BACKUP/_MSGRET.ASM](../reference/v4.0/src/CMD/BACKUP/_MSGRET.ASM) | asm | 268 | not_translated |
| [v4.0/src/CMD/BACKUP/_PARSE.ASM](../reference/v4.0/src/CMD/BACKUP/_PARSE.ASM) | asm | 121 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKCHNG.INC](../reference/v4.0/src/CMD/CHKDSK/CHKCHNG.INC) | inc | 289 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKDATA.INC](../reference/v4.0/src/CMD/CHKDSK/CHKDATA.INC) | inc | 245 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKDISK.ASM](../reference/v4.0/src/CMD/CHKDSK/CHKDISK.ASM) | asm | 328 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKDISP.ASM](../reference/v4.0/src/CMD/CHKDSK/CHKDISP.ASM) | asm | 140 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKDSK1.ASM](../reference/v4.0/src/CMD/CHKDSK/CHKDSK1.ASM) | asm | 682 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKDSK2.ASM](../reference/v4.0/src/CMD/CHKDSK/CHKDSK2.ASM) | asm | 449 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKEQU.INC](../reference/v4.0/src/CMD/CHKDSK/CHKEQU.INC) | inc | 206 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKFAT.ASM](../reference/v4.0/src/CMD/CHKDSK/CHKFAT.ASM) | asm | 1064 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKINIT.ASM](../reference/v4.0/src/CMD/CHKDSK/CHKINIT.ASM) | asm | 1548 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKMACRO.INC](../reference/v4.0/src/CMD/CHKDSK/CHKMACRO.INC) | inc | 105 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKMSG.INC](../reference/v4.0/src/CMD/CHKDSK/CHKMSG.INC) | inc | 1048 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKPARSE.INC](../reference/v4.0/src/CMD/CHKDSK/CHKPARSE.INC) | inc | 286 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKPRMT.ASM](../reference/v4.0/src/CMD/CHKDSK/CHKPRMT.ASM) | asm | 93 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKPROC.ASM](../reference/v4.0/src/CMD/CHKDSK/CHKPROC.ASM) | asm | 1892 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKPROC2.ASM](../reference/v4.0/src/CMD/CHKDSK/CHKPROC2.ASM) | asm | 479 | not_translated |
| [v4.0/src/CMD/CHKDSK/CHKSEG.INC](../reference/v4.0/src/CMD/CHKDSK/CHKSEG.INC) | inc | 21 | not_translated |
| [v4.0/src/CMD/CHKDSK/PATHMAC.INC](../reference/v4.0/src/CMD/CHKDSK/PATHMAC.INC) | inc | 32 | not_translated |
| [v4.0/src/CMD/COMMAND/COMEQU.ASM](../reference/v4.0/src/CMD/COMMAND/COMEQU.ASM) | asm | 242 | not_translated |
| [v4.0/src/CMD/COMMAND/COMMAND1.ASM](../reference/v4.0/src/CMD/COMMAND/COMMAND1.ASM) | asm | 639 | not_translated |
| [v4.0/src/CMD/COMMAND/COMMAND2.ASM](../reference/v4.0/src/CMD/COMMAND/COMMAND2.ASM) | asm | 620 | not_translated |
| [v4.0/src/CMD/COMMAND/COMSEG.ASM](../reference/v4.0/src/CMD/COMMAND/COMSEG.ASM) | asm | 43 | not_translated |
| [v4.0/src/CMD/COMMAND/COMSW.ASM](../reference/v4.0/src/CMD/COMMAND/COMSW.ASM) | asm | 6 | not_translated |
| [v4.0/src/CMD/COMMAND/COPY.ASM](../reference/v4.0/src/CMD/COMMAND/COPY.ASM) | asm | 1001 | not_translated |
| [v4.0/src/CMD/COMMAND/COPYPR1.ASM](../reference/v4.0/src/CMD/COMMAND/COPYPR1.ASM) | asm | 276 | not_translated |
| [v4.0/src/CMD/COMMAND/COPYPR2.ASM](../reference/v4.0/src/CMD/COMMAND/COPYPR2.ASM) | asm | 450 | not_translated |
| [v4.0/src/CMD/COMMAND/CPARSE.ASM](../reference/v4.0/src/CMD/COMMAND/CPARSE.ASM) | asm | 418 | not_translated |
| [v4.0/src/CMD/COMMAND/ENVDATA.ASM](../reference/v4.0/src/CMD/COMMAND/ENVDATA.ASM) | asm | 29 | not_translated |
| [v4.0/src/CMD/COMMAND/FORDATA.ASM](../reference/v4.0/src/CMD/COMMAND/FORDATA.ASM) | asm | 20 | not_translated |
| [v4.0/src/CMD/COMMAND/IFEQU.ASM](../reference/v4.0/src/CMD/COMMAND/IFEQU.ASM) | asm | 18 | not_translated |
| [v4.0/src/CMD/COMMAND/INIT.ASM](../reference/v4.0/src/CMD/COMMAND/INIT.ASM) | asm | 1686 | not_translated |
| [v4.0/src/CMD/COMMAND/IPARSE.ASM](../reference/v4.0/src/CMD/COMMAND/IPARSE.ASM) | asm | 38 | not_translated |
| [v4.0/src/CMD/COMMAND/PARSE2.ASM](../reference/v4.0/src/CMD/COMMAND/PARSE2.ASM) | asm | 423 | not_translated |
| [v4.0/src/CMD/COMMAND/PATH1.ASM](../reference/v4.0/src/CMD/COMMAND/PATH1.ASM) | asm | 508 | not_translated |
| [v4.0/src/CMD/COMMAND/PATH2.ASM](../reference/v4.0/src/CMD/COMMAND/PATH2.ASM) | asm | 459 | not_translated |
| [v4.0/src/CMD/COMMAND/RDATA.ASM](../reference/v4.0/src/CMD/COMMAND/RDATA.ASM) | asm | 612 | not_translated |
| [v4.0/src/CMD/COMMAND/RUCODE.ASM](../reference/v4.0/src/CMD/COMMAND/RUCODE.ASM) | asm | 740 | not_translated |
| [v4.0/src/CMD/COMMAND/TBATCH.ASM](../reference/v4.0/src/CMD/COMMAND/TBATCH.ASM) | asm | 1045 | not_translated |
| [v4.0/src/CMD/COMMAND/TBATCH2.ASM](../reference/v4.0/src/CMD/COMMAND/TBATCH2.ASM) | asm | 753 | not_translated |
| [v4.0/src/CMD/COMMAND/TCMD1A.ASM](../reference/v4.0/src/CMD/COMMAND/TCMD1A.ASM) | asm | 599 | not_translated |
| [v4.0/src/CMD/COMMAND/TCMD1B.ASM](../reference/v4.0/src/CMD/COMMAND/TCMD1B.ASM) | asm | 730 | not_translated |
| [v4.0/src/CMD/COMMAND/TCMD2A.ASM](../reference/v4.0/src/CMD/COMMAND/TCMD2A.ASM) | asm | 552 | not_translated |
| [v4.0/src/CMD/COMMAND/TCMD2B.ASM](../reference/v4.0/src/CMD/COMMAND/TCMD2B.ASM) | asm | 598 | not_translated |
| [v4.0/src/CMD/COMMAND/TCODE.ASM](../reference/v4.0/src/CMD/COMMAND/TCODE.ASM) | asm | 541 | not_translated |
| [v4.0/src/CMD/COMMAND/TDATA.ASM](../reference/v4.0/src/CMD/COMMAND/TDATA.ASM) | asm | 778 | not_translated |
| [v4.0/src/CMD/COMMAND/TENV.ASM](../reference/v4.0/src/CMD/COMMAND/TENV.ASM) | asm | 634 | not_translated |
| [v4.0/src/CMD/COMMAND/TENV2.ASM](../reference/v4.0/src/CMD/COMMAND/TENV2.ASM) | asm | 663 | not_translated |
| [v4.0/src/CMD/COMMAND/TFOR.ASM](../reference/v4.0/src/CMD/COMMAND/TFOR.ASM) | asm | 551 | not_translated |
| [v4.0/src/CMD/COMMAND/TMISC1.ASM](../reference/v4.0/src/CMD/COMMAND/TMISC1.ASM) | asm | 655 | not_translated |
| [v4.0/src/CMD/COMMAND/TMISC2.ASM](../reference/v4.0/src/CMD/COMMAND/TMISC2.ASM) | asm | 487 | not_translated |
| [v4.0/src/CMD/COMMAND/TPARSE.ASM](../reference/v4.0/src/CMD/COMMAND/TPARSE.ASM) | asm | 76 | not_translated |
| [v4.0/src/CMD/COMMAND/TPIPE.ASM](../reference/v4.0/src/CMD/COMMAND/TPIPE.ASM) | asm | 661 | not_translated |
| [v4.0/src/CMD/COMMAND/TPRINTF.ASM](../reference/v4.0/src/CMD/COMMAND/TPRINTF.ASM) | asm | 366 | not_translated |
| [v4.0/src/CMD/COMMAND/TRANMSG.ASM](../reference/v4.0/src/CMD/COMMAND/TRANMSG.ASM) | asm | 672 | not_translated |
| [v4.0/src/CMD/COMMAND/TSPC.ASM](../reference/v4.0/src/CMD/COMMAND/TSPC.ASM) | asm | 489 | not_translated |
| [v4.0/src/CMD/COMMAND/TUCODE.ASM](../reference/v4.0/src/CMD/COMMAND/TUCODE.ASM) | asm | 521 | not_translated |
| [v4.0/src/CMD/COMMAND/UINIT.ASM](../reference/v4.0/src/CMD/COMMAND/UINIT.ASM) | asm | 271 | not_translated |
| [v4.0/src/CMD/COMP/COMP1.ASM](../reference/v4.0/src/CMD/COMP/COMP1.ASM) | asm | 190 | not_translated |
| [v4.0/src/CMD/COMP/COMP2.ASM](../reference/v4.0/src/CMD/COMP/COMP2.ASM) | asm | 1932 | not_translated |
| [v4.0/src/CMD/COMP/COMPEQ.INC](../reference/v4.0/src/CMD/COMP/COMPEQ.INC) | inc | 185 | not_translated |
| [v4.0/src/CMD/COMP/COMPMS.INC](../reference/v4.0/src/CMD/COMP/COMPMS.INC) | inc | 157 | not_translated |
| [v4.0/src/CMD/COMP/COMPP.ASM](../reference/v4.0/src/CMD/COMP/COMPP.ASM) | asm | 130 | not_translated |
| [v4.0/src/CMD/COMP/COMPPAR.ASM](../reference/v4.0/src/CMD/COMP/COMPPAR.ASM) | asm | 410 | not_translated |
| [v4.0/src/CMD/COMP/COMPSM.ASM](../reference/v4.0/src/CMD/COMP/COMPSM.ASM) | asm | 159 | not_translated |
| [v4.0/src/CMD/DEBUG/DEBASM.ASM](../reference/v4.0/src/CMD/DEBUG/DEBASM.ASM) | asm | 1441 | not_translated |
| [v4.0/src/CMD/DEBUG/DEBCOM1.ASM](../reference/v4.0/src/CMD/DEBUG/DEBCOM1.ASM) | asm | 969 | not_translated |
| [v4.0/src/CMD/DEBUG/DEBCOM2.ASM](../reference/v4.0/src/CMD/DEBUG/DEBCOM2.ASM) | asm | 1308 | not_translated |
| [v4.0/src/CMD/DEBUG/DEBCOM3.ASM](../reference/v4.0/src/CMD/DEBUG/DEBCOM3.ASM) | asm | 678 | not_translated |
| [v4.0/src/CMD/DEBUG/DEBCONST.ASM](../reference/v4.0/src/CMD/DEBUG/DEBCONST.ASM) | asm | 1129 | not_translated |
| [v4.0/src/CMD/DEBUG/DEBDATA.ASM](../reference/v4.0/src/CMD/DEBUG/DEBDATA.ASM) | asm | 227 | not_translated |
| [v4.0/src/CMD/DEBUG/DEBEMS.ASM](../reference/v4.0/src/CMD/DEBUG/DEBEMS.ASM) | asm | 1127 | not_translated |
| [v4.0/src/CMD/DEBUG/DEBEQU.ASM](../reference/v4.0/src/CMD/DEBUG/DEBEQU.ASM) | asm | 164 | not_translated |
| [v4.0/src/CMD/DEBUG/DEBERR.ASM](../reference/v4.0/src/CMD/DEBUG/DEBERR.ASM) | asm | 183 | not_translated |
| [v4.0/src/CMD/DEBUG/DEBMES.ASM](../reference/v4.0/src/CMD/DEBUG/DEBMES.ASM) | asm | 1524 | not_translated |
| [v4.0/src/CMD/DEBUG/DEBUASM.ASM](../reference/v4.0/src/CMD/DEBUG/DEBUASM.ASM) | asm | 954 | not_translated |
| [v4.0/src/CMD/DEBUG/DEBUG.ASM](../reference/v4.0/src/CMD/DEBUG/DEBUG.ASM) | asm | 1236 | not_translated |
| [v4.0/src/CMD/DEBUG/SYSVER.INC](../reference/v4.0/src/CMD/DEBUG/SYSVER.INC) | inc | 10 | not_translated |
| [v4.0/src/CMD/DISKCOMP/COMPINIT.ASM](../reference/v4.0/src/CMD/DISKCOMP/COMPINIT.ASM) | asm | 1123 | not_translated |
| [v4.0/src/CMD/DISKCOMP/DCMPMACR.INC](../reference/v4.0/src/CMD/DISKCOMP/DCMPMACR.INC) | inc | 171 | not_translated |
| [v4.0/src/CMD/DISKCOMP/DCOMPMS.INC](../reference/v4.0/src/CMD/DISKCOMP/DCOMPMS.INC) | inc | 232 | not_translated |
| [v4.0/src/CMD/DISKCOMP/DCOMPP.ASM](../reference/v4.0/src/CMD/DISKCOMP/DCOMPP.ASM) | asm | 123 | not_translated |
| [v4.0/src/CMD/DISKCOMP/DCOMPPAR.ASM](../reference/v4.0/src/CMD/DISKCOMP/DCOMPPAR.ASM) | asm | 427 | not_translated |
| [v4.0/src/CMD/DISKCOMP/DCOMPSM.ASM](../reference/v4.0/src/CMD/DISKCOMP/DCOMPSM.ASM) | asm | 159 | not_translated |
| [v4.0/src/CMD/DISKCOMP/DISKCOMP.ASM](../reference/v4.0/src/CMD/DISKCOMP/DISKCOMP.ASM) | asm | 2060 | not_translated |
| [v4.0/src/CMD/DISKCOPY/COPYINIT.ASM](../reference/v4.0/src/CMD/DISKCOPY/COPYINIT.ASM) | asm | 1046 | not_translated |
| [v4.0/src/CMD/DISKCOPY/DCOPYMS.INC](../reference/v4.0/src/CMD/DISKCOPY/DCOPYMS.INC) | inc | 269 | not_translated |
| [v4.0/src/CMD/DISKCOPY/DCOPYP.ASM](../reference/v4.0/src/CMD/DISKCOPY/DCOPYP.ASM) | asm | 139 | not_translated |
| [v4.0/src/CMD/DISKCOPY/DCOPYPAR.ASM](../reference/v4.0/src/CMD/DISKCOPY/DCOPYPAR.ASM) | asm | 413 | not_translated |
| [v4.0/src/CMD/DISKCOPY/DCOPYSM.ASM](../reference/v4.0/src/CMD/DISKCOPY/DCOPYSM.ASM) | asm | 153 | not_translated |
| [v4.0/src/CMD/DISKCOPY/DCPYMACR.INC](../reference/v4.0/src/CMD/DISKCOPY/DCPYMACR.INC) | inc | 164 | not_translated |
| [v4.0/src/CMD/DISKCOPY/DISKCOPY.ASM](../reference/v4.0/src/CMD/DISKCOPY/DISKCOPY.ASM) | asm | 2452 | not_translated |
| [v4.0/src/CMD/EDLIN/EDLCMD1.ASM](../reference/v4.0/src/CMD/EDLIN/EDLCMD1.ASM) | asm | 664 | not_translated |
| [v4.0/src/CMD/EDLIN/EDLCMD2.ASM](../reference/v4.0/src/CMD/EDLIN/EDLCMD2.ASM) | asm | 1203 | not_translated |
| [v4.0/src/CMD/EDLIN/EDLEQU.ASM](../reference/v4.0/src/CMD/EDLIN/EDLEQU.ASM) | asm | 156 | not_translated |
| [v4.0/src/CMD/EDLIN/EDLIN.ASM](../reference/v4.0/src/CMD/EDLIN/EDLIN.ASM) | asm | 1843 | not_translated |
| [v4.0/src/CMD/EDLIN/EDLMES.ASM](../reference/v4.0/src/CMD/EDLIN/EDLMES.ASM) | asm | 665 | not_translated |
| [v4.0/src/CMD/EDLIN/EDLPARSE.ASM](../reference/v4.0/src/CMD/EDLIN/EDLPARSE.ASM) | asm | 281 | not_translated |
| [v4.0/src/CMD/EDLIN/EDLSTDSW.INC](../reference/v4.0/src/CMD/EDLIN/EDLSTDSW.INC) | inc | 33 | not_translated |
| [v4.0/src/CMD/EXE2BIN/DISPLAY.ASM](../reference/v4.0/src/CMD/EXE2BIN/DISPLAY.ASM) | asm | 110 | not_translated |
| [v4.0/src/CMD/EXE2BIN/E2BCHNG.INC](../reference/v4.0/src/CMD/EXE2BIN/E2BCHNG.INC) | inc | 22 | not_translated |
| [v4.0/src/CMD/EXE2BIN/E2BEQU.INC](../reference/v4.0/src/CMD/EXE2BIN/E2BEQU.INC) | inc | 50 | not_translated |
| [v4.0/src/CMD/EXE2BIN/E2BINIT.ASM](../reference/v4.0/src/CMD/EXE2BIN/E2BINIT.ASM) | asm | 1123 | not_translated |
| [v4.0/src/CMD/EXE2BIN/E2BMACRO.INC](../reference/v4.0/src/CMD/EXE2BIN/E2BMACRO.INC) | inc | 81 | not_translated |
| [v4.0/src/CMD/EXE2BIN/E2BPARSE.INC](../reference/v4.0/src/CMD/EXE2BIN/E2BPARSE.INC) | inc | 180 | not_translated |
| [v4.0/src/CMD/EXE2BIN/E2BTABLE.INC](../reference/v4.0/src/CMD/EXE2BIN/E2BTABLE.INC) | inc | 190 | not_translated |
| [v4.0/src/CMD/EXE2BIN/LOCATE.ASM](../reference/v4.0/src/CMD/EXE2BIN/LOCATE.ASM) | asm | 593 | not_translated |
| [v4.0/src/CMD/EXE2BIN/LOCMES.ASM](../reference/v4.0/src/CMD/EXE2BIN/LOCMES.ASM) | asm | 56 | not_translated |
| [v4.0/src/CMD/FASTOPEN/FASTINIT.ASM](../reference/v4.0/src/CMD/FASTOPEN/FASTINIT.ASM) | asm | 2971 | not_translated |
| [v4.0/src/CMD/FASTOPEN/FASTOPEN.ASM](../reference/v4.0/src/CMD/FASTOPEN/FASTOPEN.ASM) | asm | 2052 | not_translated |
| [v4.0/src/CMD/FASTOPEN/FASTP.ASM](../reference/v4.0/src/CMD/FASTOPEN/FASTP.ASM) | asm | 132 | not_translated |
| [v4.0/src/CMD/FASTOPEN/FASTSEEK.ASM](../reference/v4.0/src/CMD/FASTOPEN/FASTSEEK.ASM) | asm | 2945 | not_translated |
| [v4.0/src/CMD/FASTOPEN/FASTSEGS.INC](../reference/v4.0/src/CMD/FASTOPEN/FASTSEGS.INC) | inc | 20 | not_translated |
| [v4.0/src/CMD/FASTOPEN/FASTSM.ASM](../reference/v4.0/src/CMD/FASTOPEN/FASTSM.ASM) | asm | 146 | not_translated |
| [v4.0/src/CMD/FC/ERROR.C](../reference/v4.0/src/CMD/FC/ERROR.C) | c | 17 | not_translated |
| [v4.0/src/CMD/FC/FC.C](../reference/v4.0/src/CMD/FC/FC.C) | c | 865 | not_translated |
| [v4.0/src/CMD/FC/FC.H](../reference/v4.0/src/CMD/FC/FC.H) | h | 15 | not_translated |
| [v4.0/src/CMD/FC/FGETL.C](../reference/v4.0/src/CMD/FC/FGETL.C) | c | 53 | not_translated |
| [v4.0/src/CMD/FC/GETL.ASM](../reference/v4.0/src/CMD/FC/GETL.ASM) | asm | 147 | not_translated |
| [v4.0/src/CMD/FC/INTERNAT.H](../reference/v4.0/src/CMD/FC/INTERNAT.H) | h | 31 | not_translated |
| [v4.0/src/CMD/FC/ITOUPPER.ASM](../reference/v4.0/src/CMD/FC/ITOUPPER.ASM) | asm | 120 | not_translated |
| [v4.0/src/CMD/FC/MAXMIN.ASM](../reference/v4.0/src/CMD/FC/MAXMIN.ASM) | asm | 38 | not_translated |
| [v4.0/src/CMD/FC/MESSAGES.ASM](../reference/v4.0/src/CMD/FC/MESSAGES.ASM) | asm | 32 | not_translated |
| [v4.0/src/CMD/FC/MOVE.ASM](../reference/v4.0/src/CMD/FC/MOVE.ASM) | asm | 74 | not_translated |
| [v4.0/src/CMD/FC/NTOI.C](../reference/v4.0/src/CMD/FC/NTOI.C) | c | 38 | not_translated |
| [v4.0/src/CMD/FC/STRING.ASM](../reference/v4.0/src/CMD/FC/STRING.ASM) | asm | 116 | not_translated |
| [v4.0/src/CMD/FC/TOOLS.H](../reference/v4.0/src/CMD/FC/TOOLS.H) | h | 95 | not_translated |
| [v4.0/src/CMD/FC/TTYPES.H](../reference/v4.0/src/CMD/FC/TTYPES.H) | h | 72 | not_translated |
| [v4.0/src/CMD/FC/UPDATE.C](../reference/v4.0/src/CMD/FC/UPDATE.C) | c | 129 | not_translated |
| [v4.0/src/CMD/FC/XTAB.ASM](../reference/v4.0/src/CMD/FC/XTAB.ASM) | asm | 101 | not_translated |
| [v4.0/src/CMD/FDISK/BOOTREC.ASM](../reference/v4.0/src/CMD/FDISK/BOOTREC.ASM) | asm | 36 | not_translated |
| [v4.0/src/CMD/FDISK/CONVERT.C](../reference/v4.0/src/CMD/FDISK/CONVERT.C) | c | 479 | not_translated |
| [v4.0/src/CMD/FDISK/C_MENUS.C](../reference/v4.0/src/CMD/FDISK/C_MENUS.C) | c | 1068 | not_translated |
| [v4.0/src/CMD/FDISK/DISKOUT.C](../reference/v4.0/src/CMD/FDISK/DISKOUT.C) | c | 355 | not_translated |
| [v4.0/src/CMD/FDISK/DISPLAY.C](../reference/v4.0/src/CMD/FDISK/DISPLAY.C) | c | 407 | not_translated |
| [v4.0/src/CMD/FDISK/DOS.H](../reference/v4.0/src/CMD/FDISK/DOS.H) | h | 81 | not_translated |
| [v4.0/src/CMD/FDISK/DOSCALL.H](../reference/v4.0/src/CMD/FDISK/DOSCALL.H) | h | 2812 | not_translated |
| [v4.0/src/CMD/FDISK/D_MENUS.C](../reference/v4.0/src/CMD/FDISK/D_MENUS.C) | c | 942 | not_translated |
| [v4.0/src/CMD/FDISK/EXTERN.H](../reference/v4.0/src/CMD/FDISK/EXTERN.H) | h | 83 | not_translated |
| [v4.0/src/CMD/FDISK/FDBOOT.ASM](../reference/v4.0/src/CMD/FDISK/FDBOOT.ASM) | asm | 119 | not_translated |
| [v4.0/src/CMD/FDISK/FDCHNG.INC](../reference/v4.0/src/CMD/FDISK/FDCHNG.INC) | inc | 89 | not_translated |
| [v4.0/src/CMD/FDISK/FDISK.C](../reference/v4.0/src/CMD/FDISK/FDISK.C) | c | 1122 | not_translated |
| [v4.0/src/CMD/FDISK/FDISK.H](../reference/v4.0/src/CMD/FDISK/FDISK.H) | h | 224 | not_translated |
| [v4.0/src/CMD/FDISK/FDISKMSG.C](../reference/v4.0/src/CMD/FDISK/FDISKMSG.C) | c | 1182 | not_translated |
| [v4.0/src/CMD/FDISK/FDISKMSG.H](../reference/v4.0/src/CMD/FDISK/FDISKMSG.H) | h | 132 | not_translated |
| [v4.0/src/CMD/FDISK/FDPARSE.C](../reference/v4.0/src/CMD/FDISK/FDPARSE.C) | c | 313 | not_translated |
| [v4.0/src/CMD/FDISK/GLOBAL.C](../reference/v4.0/src/CMD/FDISK/GLOBAL.C) | c | 85 | not_translated |
| [v4.0/src/CMD/FDISK/INPUT.C](../reference/v4.0/src/CMD/FDISK/INPUT.C) | c | 674 | not_translated |
| [v4.0/src/CMD/FDISK/INT13.C](../reference/v4.0/src/CMD/FDISK/INT13.C) | c | 444 | not_translated |
| [v4.0/src/CMD/FDISK/MAIN.C](../reference/v4.0/src/CMD/FDISK/MAIN.C) | c | 333 | not_translated |
| [v4.0/src/CMD/FDISK/MAINMENU.C](../reference/v4.0/src/CMD/FDISK/MAINMENU.C) | c | 231 | not_translated |
| [v4.0/src/CMD/FDISK/MAKEPART.C](../reference/v4.0/src/CMD/FDISK/MAKEPART.C) | c | 283 | not_translated |
| [v4.0/src/CMD/FDISK/MESSAGES.C](../reference/v4.0/src/CMD/FDISK/MESSAGES.C) | c | 163 | not_translated |
| [v4.0/src/CMD/FDISK/MSGRET.H](../reference/v4.0/src/CMD/FDISK/MSGRET.H) | h | 56 | not_translated |
| [v4.0/src/CMD/FDISK/PARSE.H](../reference/v4.0/src/CMD/FDISK/PARSE.H) | h | 189 | not_translated |
| [v4.0/src/CMD/FDISK/PARTINFO.C](../reference/v4.0/src/CMD/FDISK/PARTINFO.C) | c | 262 | not_translated |
| [v4.0/src/CMD/FDISK/PRINT.H](../reference/v4.0/src/CMD/FDISK/PRINT.H) | h | 15 | not_translated |
| [v4.0/src/CMD/FDISK/PROFILE.C](../reference/v4.0/src/CMD/FDISK/PROFILE.C) | c | 802 | not_translated |
| [v4.0/src/CMD/FDISK/PROFILE.H](../reference/v4.0/src/CMD/FDISK/PROFILE.H) | h | 44 | not_translated |
| [v4.0/src/CMD/FDISK/REBOOT.ASM](../reference/v4.0/src/CMD/FDISK/REBOOT.ASM) | asm | 43 | not_translated |
| [v4.0/src/CMD/FDISK/SPACE.C](../reference/v4.0/src/CMD/FDISK/SPACE.C) | c | 440 | not_translated |
| [v4.0/src/CMD/FDISK/SUBTYPE.H](../reference/v4.0/src/CMD/FDISK/SUBTYPE.H) | h | 159 | not_translated |
| [v4.0/src/CMD/FDISK/TDISPLAY.C](../reference/v4.0/src/CMD/FDISK/TDISPLAY.C) | c | 134 | not_translated |
| [v4.0/src/CMD/FDISK/TEST.H](../reference/v4.0/src/CMD/FDISK/TEST.H) | h | 26 | not_translated |
| [v4.0/src/CMD/FDISK/VDISPLAY.C](../reference/v4.0/src/CMD/FDISK/VDISPLAY.C) | c | 180 | not_translated |
| [v4.0/src/CMD/FDISK/VIDEO.C](../reference/v4.0/src/CMD/FDISK/VIDEO.C) | c | 147 | not_translated |
| [v4.0/src/CMD/FDISK/_MSGRET.ASM](../reference/v4.0/src/CMD/FDISK/_MSGRET.ASM) | asm | 263 | not_translated |
| [v4.0/src/CMD/FDISK/_PARSE.ASM](../reference/v4.0/src/CMD/FDISK/_PARSE.ASM) | asm | 166 | not_translated |
| [v4.0/src/CMD/FILESYS/FILESYS.C](../reference/v4.0/src/CMD/FILESYS/FILESYS.C) | c | 923 | not_translated |
| [v4.0/src/CMD/FILESYS/FSCHNG.INC](../reference/v4.0/src/CMD/FILESYS/FSCHNG.INC) | inc | 25 | not_translated |
| [v4.0/src/CMD/FILESYS/MSGRET.H](../reference/v4.0/src/CMD/FILESYS/MSGRET.H) | h | 54 | not_translated |
| [v4.0/src/CMD/FILESYS/PARSE.H](../reference/v4.0/src/CMD/FILESYS/PARSE.H) | h | 156 | not_translated |
| [v4.0/src/CMD/FILESYS/PSDATA.H](../reference/v4.0/src/CMD/FILESYS/PSDATA.H) | h | 147 | not_translated |
| [v4.0/src/CMD/FILESYS/_MSGRET.ASM](../reference/v4.0/src/CMD/FILESYS/_MSGRET.ASM) | asm | 191 | not_translated |
| [v4.0/src/CMD/FILESYS/_PARSE.ASM](../reference/v4.0/src/CMD/FILESYS/_PARSE.ASM) | asm | 166 | not_translated |
| [v4.0/src/CMD/FIND/FIND.ASM](../reference/v4.0/src/CMD/FIND/FIND.ASM) | asm | 1665 | not_translated |
| [v4.0/src/CMD/FIND/FIND.INC](../reference/v4.0/src/CMD/FIND/FIND.INC) | inc | 100 | not_translated |
| [v4.0/src/CMD/FIND/FINDMES.ASM](../reference/v4.0/src/CMD/FIND/FINDMES.ASM) | asm | 19 | not_translated |
| [v4.0/src/CMD/FORMAT/DISPLAY.ASM](../reference/v4.0/src/CMD/FORMAT/DISPLAY.ASM) | asm | 178 | not_translated |
| [v4.0/src/CMD/FORMAT/FILESIZE.INC](../reference/v4.0/src/CMD/FORMAT/FILESIZE.INC) | inc | 6 | not_translated |
| [v4.0/src/CMD/FORMAT/FORCHNG.INC](../reference/v4.0/src/CMD/FORMAT/FORCHNG.INC) | inc | 136 | not_translated |
| [v4.0/src/CMD/FORMAT/FOREQU.INC](../reference/v4.0/src/CMD/FORMAT/FOREQU.INC) | inc | 116 | not_translated |
| [v4.0/src/CMD/FORMAT/FOREXEC.ASM](../reference/v4.0/src/CMD/FORMAT/FOREXEC.ASM) | asm | 637 | not_translated |
| [v4.0/src/CMD/FORMAT/FORINIT.ASM](../reference/v4.0/src/CMD/FORMAT/FORINIT.ASM) | asm | 1255 | not_translated |
| [v4.0/src/CMD/FORMAT/FORLABEL.ASM](../reference/v4.0/src/CMD/FORMAT/FORLABEL.ASM) | asm | 771 | not_translated |
| [v4.0/src/CMD/FORMAT/FORMACRO.INC](../reference/v4.0/src/CMD/FORMAT/FORMACRO.INC) | inc | 102 | not_translated |
| [v4.0/src/CMD/FORMAT/FORMAT.ASM](../reference/v4.0/src/CMD/FORMAT/FORMAT.ASM) | asm | 4509 | not_translated |
| [v4.0/src/CMD/FORMAT/FORMSG.INC](../reference/v4.0/src/CMD/FORMAT/FORMSG.INC) | inc | 862 | not_translated |
| [v4.0/src/CMD/FORMAT/FORPARSE.INC](../reference/v4.0/src/CMD/FORMAT/FORPARSE.INC) | inc | 524 | not_translated |
| [v4.0/src/CMD/FORMAT/FORPROC.ASM](../reference/v4.0/src/CMD/FORMAT/FORPROC.ASM) | asm | 521 | not_translated |
| [v4.0/src/CMD/FORMAT/FORSWTCH.INC](../reference/v4.0/src/CMD/FORMAT/FORSWTCH.INC) | inc | 39 | not_translated |
| [v4.0/src/CMD/FORMAT/MSFOR.ASM](../reference/v4.0/src/CMD/FORMAT/MSFOR.ASM) | asm | 1936 | not_translated |
| [v4.0/src/CMD/GRAFTABL/GRTAB.ASM](../reference/v4.0/src/CMD/GRAFTABL/GRTAB.ASM) | asm | 1218 | not_translated |
| [v4.0/src/CMD/GRAFTABL/GRTABCF.ASM](../reference/v4.0/src/CMD/GRAFTABL/GRTABCF.ASM) | asm | 1196 | not_translated |
| [v4.0/src/CMD/GRAFTABL/GRTABHAN.ASM](../reference/v4.0/src/CMD/GRAFTABL/GRTABHAN.ASM) | asm | 223 | not_translated |
| [v4.0/src/CMD/GRAFTABL/GRTABML.ASM](../reference/v4.0/src/CMD/GRAFTABL/GRTABML.ASM) | asm | 1187 | not_translated |
| [v4.0/src/CMD/GRAFTABL/GRTABMS.INC](../reference/v4.0/src/CMD/GRAFTABL/GRTABMS.INC) | inc | 116 | not_translated |
| [v4.0/src/CMD/GRAFTABL/GRTABNO.ASM](../reference/v4.0/src/CMD/GRAFTABL/GRTABNO.ASM) | asm | 1188 | not_translated |
| [v4.0/src/CMD/GRAFTABL/GRTABP.ASM](../reference/v4.0/src/CMD/GRAFTABL/GRTABP.ASM) | asm | 144 | not_translated |
| [v4.0/src/CMD/GRAFTABL/GRTABPAR.ASM](../reference/v4.0/src/CMD/GRAFTABL/GRTABPAR.ASM) | asm | 538 | not_translated |
| [v4.0/src/CMD/GRAFTABL/GRTABPO.ASM](../reference/v4.0/src/CMD/GRAFTABL/GRTABPO.ASM) | asm | 1316 | not_translated |
| [v4.0/src/CMD/GRAFTABL/GRTABSM.ASM](../reference/v4.0/src/CMD/GRAFTABL/GRTABSM.ASM) | asm | 152 | not_translated |
| [v4.0/src/CMD/GRAFTABL/GRTABUS.ASM](../reference/v4.0/src/CMD/GRAFTABL/GRTABUS.ASM) | asm | 1182 | not_translated |
| [v4.0/src/CMD/GRAPHICS/GRAPHICS.ASM](../reference/v4.0/src/CMD/GRAPHICS/GRAPHICS.ASM) | asm | 77 | not_translated |
| [v4.0/src/CMD/GRAPHICS/GRBWPRT.ASM](../reference/v4.0/src/CMD/GRAPHICS/GRBWPRT.ASM) | asm | 632 | not_translated |
| [v4.0/src/CMD/GRAPHICS/GRCOLPRT.ASM](../reference/v4.0/src/CMD/GRAPHICS/GRCOLPRT.ASM) | asm | 1123 | not_translated |
| [v4.0/src/CMD/GRAPHICS/GRCOMMON.ASM](../reference/v4.0/src/CMD/GRAPHICS/GRCOMMON.ASM) | asm | 859 | not_translated |
| [v4.0/src/CMD/GRAPHICS/GRCPSD.ASM](../reference/v4.0/src/CMD/GRAPHICS/GRCPSD.ASM) | asm | 110 | not_translated |
| [v4.0/src/CMD/GRAPHICS/GRCTRL.ASM](../reference/v4.0/src/CMD/GRAPHICS/GRCTRL.ASM) | asm | 2163 | not_translated |
| [v4.0/src/CMD/GRAPHICS/GRINST.ASM](../reference/v4.0/src/CMD/GRAPHICS/GRINST.ASM) | asm | 974 | not_translated |
| [v4.0/src/CMD/GRAPHICS/GRINT2FH.ASM](../reference/v4.0/src/CMD/GRAPHICS/GRINT2FH.ASM) | asm | 147 | not_translated |
| [v4.0/src/CMD/GRAPHICS/GRLOAD.ASM](../reference/v4.0/src/CMD/GRAPHICS/GRLOAD.ASM) | asm | 841 | not_translated |
| [v4.0/src/CMD/GRAPHICS/GRLOAD2.ASM](../reference/v4.0/src/CMD/GRAPHICS/GRLOAD2.ASM) | asm | 911 | not_translated |
| [v4.0/src/CMD/GRAPHICS/GRLOAD3.ASM](../reference/v4.0/src/CMD/GRAPHICS/GRLOAD3.ASM) | asm | 730 | not_translated |
| [v4.0/src/CMD/GRAPHICS/GRPARMS.ASM](../reference/v4.0/src/CMD/GRAPHICS/GRPARMS.ASM) | asm | 718 | not_translated |
| [v4.0/src/CMD/GRAPHICS/GRPARSE.ASM](../reference/v4.0/src/CMD/GRAPHICS/GRPARSE.ASM) | asm | 88 | not_translated |
| [v4.0/src/CMD/GRAPHICS/GRPATTRN.ASM](../reference/v4.0/src/CMD/GRAPHICS/GRPATTRN.ASM) | asm | 218 | not_translated |
| [v4.0/src/CMD/IFSFUNC/IFSDEV.ASM](../reference/v4.0/src/CMD/IFSFUNC/IFSDEV.ASM) | asm | 1268 | not_translated |
| [v4.0/src/CMD/IFSFUNC/IFSDIR.ASM](../reference/v4.0/src/CMD/IFSFUNC/IFSDIR.ASM) | asm | 314 | not_translated |
| [v4.0/src/CMD/IFSFUNC/IFSERROR.ASM](../reference/v4.0/src/CMD/IFSFUNC/IFSERROR.ASM) | asm | 638 | not_translated |
| [v4.0/src/CMD/IFSFUNC/IFSFDOS.ASM](../reference/v4.0/src/CMD/IFSFUNC/IFSFDOS.ASM) | asm | 477 | not_translated |
| [v4.0/src/CMD/IFSFUNC/IFSFILE.ASM](../reference/v4.0/src/CMD/IFSFUNC/IFSFILE.ASM) | asm | 1779 | not_translated |
| [v4.0/src/CMD/IFSFUNC/IFSFLINK.ASM](../reference/v4.0/src/CMD/IFSFUNC/IFSFLINK.ASM) | asm | 203 | not_translated |
| [v4.0/src/CMD/IFSFUNC/IFSFSYM.INC](../reference/v4.0/src/CMD/IFSFUNC/IFSFSYM.INC) | inc | 163 | not_translated |
| [v4.0/src/CMD/IFSFUNC/IFSHAND.ASM](../reference/v4.0/src/CMD/IFSFUNC/IFSHAND.ASM) | asm | 904 | not_translated |
| [v4.0/src/CMD/IFSFUNC/IFSINIT.ASM](../reference/v4.0/src/CMD/IFSFUNC/IFSINIT.ASM) | asm | 1374 | not_translated |
| [v4.0/src/CMD/IFSFUNC/IFSPARSE.INC](../reference/v4.0/src/CMD/IFSFUNC/IFSPARSE.INC) | inc | 93 | not_translated |
| [v4.0/src/CMD/IFSFUNC/IFSSESS.ASM](../reference/v4.0/src/CMD/IFSFUNC/IFSSESS.ASM) | asm | 2188 | not_translated |
| [v4.0/src/CMD/IFSFUNC/IFSUTIL.ASM](../reference/v4.0/src/CMD/IFSFUNC/IFSUTIL.ASM) | asm | 2740 | not_translated |
| [v4.0/src/CMD/JOIN/JOIN.C](../reference/v4.0/src/CMD/JOIN/JOIN.C) | c | 670 | not_translated |
| [v4.0/src/CMD/JOIN/JOINPARS.H](../reference/v4.0/src/CMD/JOIN/JOINPARS.H) | h | 157 | not_translated |
| [v4.0/src/CMD/JOIN/_MSGRET.ASM](../reference/v4.0/src/CMD/JOIN/_MSGRET.ASM) | asm | 250 | not_translated |
| [v4.0/src/CMD/JOIN/_PARSE.ASM](../reference/v4.0/src/CMD/JOIN/_PARSE.ASM) | asm | 121 | not_translated |
| [v4.0/src/CMD/KEYB/COMMSUBS.ASM](../reference/v4.0/src/CMD/KEYB/COMMSUBS.ASM) | asm | 604 | not_translated |
| [v4.0/src/CMD/KEYB/COMMSUBS.INC](../reference/v4.0/src/CMD/KEYB/COMMSUBS.INC) | inc | 31 | not_translated |
| [v4.0/src/CMD/KEYB/KBMSG.INC](../reference/v4.0/src/CMD/KEYB/KBMSG.INC) | inc | 21 | not_translated |
| [v4.0/src/CMD/KEYB/KEYB.ASM](../reference/v4.0/src/CMD/KEYB/KEYB.ASM) | asm | 72 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBCMD.ASM](../reference/v4.0/src/CMD/KEYB/KEYBCMD.ASM) | asm | 2072 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBCMD.INC](../reference/v4.0/src/CMD/KEYB/KEYBCMD.INC) | inc | 53 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBCPSD.ASM](../reference/v4.0/src/CMD/KEYB/KEYBCPSD.ASM) | asm | 129 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBCPSD.INC](../reference/v4.0/src/CMD/KEYB/KEYBCPSD.INC) | inc | 30 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBDCL.INC](../reference/v4.0/src/CMD/KEYB/KEYBDCL.INC) | inc | 51 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBEQU.INC](../reference/v4.0/src/CMD/KEYB/KEYBEQU.INC) | inc | 32 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBI2F.ASM](../reference/v4.0/src/CMD/KEYB/KEYBI2F.ASM) | asm | 226 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBI2F.INC](../reference/v4.0/src/CMD/KEYB/KEYBI2F.INC) | inc | 27 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBI48.ASM](../reference/v4.0/src/CMD/KEYB/KEYBI48.ASM) | asm | 175 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBI48.INC](../reference/v4.0/src/CMD/KEYB/KEYBI48.INC) | inc | 28 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBI9.ASM](../reference/v4.0/src/CMD/KEYB/KEYBI9.ASM) | asm | 673 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBI9.INC](../reference/v4.0/src/CMD/KEYB/KEYBI9.INC) | inc | 30 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBI9C.ASM](../reference/v4.0/src/CMD/KEYB/KEYBI9C.ASM) | asm | 1895 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBI9C.INC](../reference/v4.0/src/CMD/KEYB/KEYBI9C.INC) | inc | 42 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBMAC.INC](../reference/v4.0/src/CMD/KEYB/KEYBMAC.INC) | inc | 320 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBMSG.INC](../reference/v4.0/src/CMD/KEYB/KEYBMSG.INC) | inc | 45 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBSHAR.INC](../reference/v4.0/src/CMD/KEYB/KEYBSHAR.INC) | inc | 308 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBSYS.INC](../reference/v4.0/src/CMD/KEYB/KEYBSYS.INC) | inc | 138 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBTBBL.ASM](../reference/v4.0/src/CMD/KEYB/KEYBTBBL.ASM) | asm | 855 | not_translated |
| [v4.0/src/CMD/KEYB/KEYBTBBL.INC](../reference/v4.0/src/CMD/KEYB/KEYBTBBL.INC) | inc | 34 | not_translated |
| [v4.0/src/CMD/KEYB/PARSER.ASM](../reference/v4.0/src/CMD/KEYB/PARSER.ASM) | asm | 518 | not_translated |
| [v4.0/src/CMD/LABEL/LABEL.ASM](../reference/v4.0/src/CMD/LABEL/LABEL.ASM) | asm | 1351 | not_translated |
| [v4.0/src/CMD/LABEL/LABELM.ASM](../reference/v4.0/src/CMD/LABEL/LABELM.ASM) | asm | 92 | not_translated |
| [v4.0/src/CMD/MEM/MEM.C](../reference/v4.0/src/CMD/MEM/MEM.C) | c | 1675 | not_translated |
| [v4.0/src/CMD/MEM/MSGDEF.H](../reference/v4.0/src/CMD/MEM/MSGDEF.H) | h | 94 | not_translated |
| [v4.0/src/CMD/MEM/PARSE.H](../reference/v4.0/src/CMD/MEM/PARSE.H) | h | 145 | not_translated |
| [v4.0/src/CMD/MEM/_MSGRET.ASM](../reference/v4.0/src/CMD/MEM/_MSGRET.ASM) | asm | 264 | not_translated |
| [v4.0/src/CMD/MEM/_PARSE.ASM](../reference/v4.0/src/CMD/MEM/_PARSE.ASM) | asm | 166 | not_translated |
| [v4.0/src/CMD/MODE/DISPLAY.ASM](../reference/v4.0/src/CMD/MODE/DISPLAY.ASM) | asm | 182 | not_translated |
| [v4.0/src/CMD/MODE/INVOKE.ASM](../reference/v4.0/src/CMD/MODE/INVOKE.ASM) | asm | 1550 | not_translated |
| [v4.0/src/CMD/MODE/MAIN.ASM](../reference/v4.0/src/CMD/MODE/MAIN.ASM) | asm | 169 | not_translated |
| [v4.0/src/CMD/MODE/MODECOM.ASM](../reference/v4.0/src/CMD/MODE/MODECOM.ASM) | asm | 593 | not_translated |
| [v4.0/src/CMD/MODE/MODECP.ASM](../reference/v4.0/src/CMD/MODE/MODECP.ASM) | asm | 1279 | not_translated |
| [v4.0/src/CMD/MODE/MODECPEQ.INC](../reference/v4.0/src/CMD/MODE/MODECPEQ.INC) | inc | 366 | not_translated |
| [v4.0/src/CMD/MODE/MODECPMS.INC](../reference/v4.0/src/CMD/MODE/MODECPMS.INC) | inc | 158 | not_translated |
| [v4.0/src/CMD/MODE/MODECPRO.INC](../reference/v4.0/src/CMD/MODE/MODECPRO.INC) | inc | 89 | not_translated |
| [v4.0/src/CMD/MODE/MODEDEFS.INC](../reference/v4.0/src/CMD/MODE/MODEDEFS.INC) | inc | 750 | not_translated |
| [v4.0/src/CMD/MODE/MODEECHO.ASM](../reference/v4.0/src/CMD/MODE/MODEECHO.ASM) | asm | 483 | not_translated |
| [v4.0/src/CMD/MODE/MODELENG.ASM](../reference/v4.0/src/CMD/MODE/MODELENG.ASM) | asm | 291 | not_translated |
| [v4.0/src/CMD/MODE/MODEMES.ASM](../reference/v4.0/src/CMD/MODE/MODEMES.ASM) | asm | 146 | not_translated |
| [v4.0/src/CMD/MODE/MODEPARS.ASM](../reference/v4.0/src/CMD/MODE/MODEPARS.ASM) | asm | 4625 | not_translated |
| [v4.0/src/CMD/MODE/MODEPRIN.ASM](../reference/v4.0/src/CMD/MODE/MODEPRIN.ASM) | asm | 1096 | not_translated |
| [v4.0/src/CMD/MODE/MODEQUAT.INC](../reference/v4.0/src/CMD/MODE/MODEQUAT.INC) | inc | 49 | not_translated |
| [v4.0/src/CMD/MODE/MODESCRN.ASM](../reference/v4.0/src/CMD/MODE/MODESCRN.ASM) | asm | 564 | not_translated |
| [v4.0/src/CMD/MODE/MODESUBS.INC](../reference/v4.0/src/CMD/MODE/MODESUBS.INC) | inc | 599 | not_translated |
| [v4.0/src/CMD/MODE/MODEVID.ASM](../reference/v4.0/src/CMD/MODE/MODEVID.ASM) | asm | 615 | not_translated |
| [v4.0/src/CMD/MODE/PARSHELL.ASM](../reference/v4.0/src/CMD/MODE/PARSHELL.ASM) | asm | 30 | not_translated |
| [v4.0/src/CMD/MODE/RESCODE.ASM](../reference/v4.0/src/CMD/MODE/RESCODE.ASM) | asm | 719 | not_translated |
| [v4.0/src/CMD/MODE/SCRNTAB.ASM](../reference/v4.0/src/CMD/MODE/SCRNTAB.ASM) | asm | 121 | not_translated |
| [v4.0/src/CMD/MODE/TYPAMAT.ASM](../reference/v4.0/src/CMD/MODE/TYPAMAT.ASM) | asm | 263 | not_translated |
| [v4.0/src/CMD/MORE/MORE.ASM](../reference/v4.0/src/CMD/MORE/MORE.ASM) | asm | 276 | not_translated |
| [v4.0/src/CMD/MORE/MORE.INC](../reference/v4.0/src/CMD/MORE/MORE.INC) | inc | 29 | not_translated |
| [v4.0/src/CMD/NLSFUNC/DOESMAC.INC](../reference/v4.0/src/CMD/NLSFUNC/DOESMAC.INC) | inc | 40 | not_translated |
| [v4.0/src/CMD/NLSFUNC/FUNCDBCS.INC](../reference/v4.0/src/CMD/NLSFUNC/FUNCDBCS.INC) | inc | 18 | not_translated |
| [v4.0/src/CMD/NLSFUNC/FUNCPARM.INC](../reference/v4.0/src/CMD/NLSFUNC/FUNCPARM.INC) | inc | 33 | not_translated |
| [v4.0/src/CMD/NLSFUNC/MSG2NLS.INC](../reference/v4.0/src/CMD/NLSFUNC/MSG2NLS.INC) | inc | 94 | not_translated |
| [v4.0/src/CMD/NLSFUNC/NLSFUNC.ASM](../reference/v4.0/src/CMD/NLSFUNC/NLSFUNC.ASM) | asm | 1812 | not_translated |
| [v4.0/src/CMD/NLSFUNC/NLSPARM.ASM](../reference/v4.0/src/CMD/NLSFUNC/NLSPARM.ASM) | asm | 148 | not_translated |
| [v4.0/src/CMD/PRINT/PRIDEFS.ASM](../reference/v4.0/src/CMD/PRINT/PRIDEFS.ASM) | asm | 211 | not_translated |
| [v4.0/src/CMD/PRINT/PRIDEFS.INC](../reference/v4.0/src/CMD/PRINT/PRIDEFS.INC) | inc | 538 | not_translated |
| [v4.0/src/CMD/PRINT/PRINT_R.ASM](../reference/v4.0/src/CMD/PRINT/PRINT_R.ASM) | asm | 3432 | not_translated |
| [v4.0/src/CMD/PRINT/PRINT_RM.ASM](../reference/v4.0/src/CMD/PRINT/PRINT_RM.ASM) | asm | 49 | not_translated |
| [v4.0/src/CMD/PRINT/PRINT_T.ASM](../reference/v4.0/src/CMD/PRINT/PRINT_T.ASM) | asm | 3504 | not_translated |
| [v4.0/src/CMD/PRINT/PRINT_TM.ASM](../reference/v4.0/src/CMD/PRINT/PRINT_TM.ASM) | asm | 84 | not_translated |
| [v4.0/src/CMD/RECOVER/PATHMAC.INC](../reference/v4.0/src/CMD/RECOVER/PATHMAC.INC) | inc | 32 | not_translated |
| [v4.0/src/CMD/RECOVER/RECCHNG.INC](../reference/v4.0/src/CMD/RECOVER/RECCHNG.INC) | inc | 162 | not_translated |
| [v4.0/src/CMD/RECOVER/RECDATA.INC](../reference/v4.0/src/CMD/RECOVER/RECDATA.INC) | inc | 168 | not_translated |
| [v4.0/src/CMD/RECOVER/RECDISP.ASM](../reference/v4.0/src/CMD/RECOVER/RECDISP.ASM) | asm | 160 | not_translated |
| [v4.0/src/CMD/RECOVER/RECEQU.INC](../reference/v4.0/src/CMD/RECOVER/RECEQU.INC) | inc | 175 | not_translated |
| [v4.0/src/CMD/RECOVER/RECINIT.ASM](../reference/v4.0/src/CMD/RECOVER/RECINIT.ASM) | asm | 1028 | not_translated |
| [v4.0/src/CMD/RECOVER/RECMACRO.INC](../reference/v4.0/src/CMD/RECOVER/RECMACRO.INC) | inc | 95 | not_translated |
| [v4.0/src/CMD/RECOVER/RECMSG.INC](../reference/v4.0/src/CMD/RECOVER/RECMSG.INC) | inc | 308 | not_translated |
| [v4.0/src/CMD/RECOVER/RECOVER.ASM](../reference/v4.0/src/CMD/RECOVER/RECOVER.ASM) | asm | 1615 | not_translated |
| [v4.0/src/CMD/RECOVER/RECPARSE.INC](../reference/v4.0/src/CMD/RECOVER/RECPARSE.INC) | inc | 178 | not_translated |
| [v4.0/src/CMD/RECOVER/RECPROC.ASM](../reference/v4.0/src/CMD/RECOVER/RECPROC.ASM) | asm | 531 | not_translated |
| [v4.0/src/CMD/RECOVER/RECPUB.INC](../reference/v4.0/src/CMD/RECOVER/RECPUB.INC) | inc | 47 | not_translated |
| [v4.0/src/CMD/RECOVER/RECSEG.INC](../reference/v4.0/src/CMD/RECOVER/RECSEG.INC) | inc | 21 | not_translated |
| [v4.0/src/CMD/REPLACE/REPLACE.C](../reference/v4.0/src/CMD/REPLACE/REPLACE.C) | c | 2048 | not_translated |
| [v4.0/src/CMD/REPLACE/REPLACEP.H](../reference/v4.0/src/CMD/REPLACE/REPLACEP.H) | h | 162 | not_translated |
| [v4.0/src/CMD/REPLACE/_MSGRET.ASM](../reference/v4.0/src/CMD/REPLACE/_MSGRET.ASM) | asm | 251 | not_translated |
| [v4.0/src/CMD/REPLACE/_PARSE.ASM](../reference/v4.0/src/CMD/REPLACE/_PARSE.ASM) | asm | 119 | not_translated |
| [v4.0/src/CMD/REPLACE/_REPLACE.ASM](../reference/v4.0/src/CMD/REPLACE/_REPLACE.ASM) | asm | 142 | not_translated |
| [v4.0/src/CMD/RESTORE/DIRECT.H](../reference/v4.0/src/CMD/RESTORE/DIRECT.H) | h | 27 | not_translated |
| [v4.0/src/CMD/RESTORE/MESSAGES.INC](../reference/v4.0/src/CMD/RESTORE/MESSAGES.INC) | inc | 1 | not_translated |
| [v4.0/src/CMD/RESTORE/RESTORE.C](../reference/v4.0/src/CMD/RESTORE/RESTORE.C) | c | 241 | not_translated |
| [v4.0/src/CMD/RESTORE/RESTPARS.C](../reference/v4.0/src/CMD/RESTORE/RESTPARS.C) | c | 1003 | not_translated |
| [v4.0/src/CMD/RESTORE/RESTPARS.H](../reference/v4.0/src/CMD/RESTORE/RESTPARS.H) | h | 120 | not_translated |
| [v4.0/src/CMD/RESTORE/RT.H](../reference/v4.0/src/CMD/RESTORE/RT.H) | h | 273 | not_translated |
| [v4.0/src/CMD/RESTORE/RT1.H](../reference/v4.0/src/CMD/RESTORE/RT1.H) | h | 274 | not_translated |
| [v4.0/src/CMD/RESTORE/RT2.H](../reference/v4.0/src/CMD/RESTORE/RT2.H) | h | 137 | not_translated |
| [v4.0/src/CMD/RESTORE/RTDO.C](../reference/v4.0/src/CMD/RESTORE/RTDO.C) | c | 377 | not_translated |
| [v4.0/src/CMD/RESTORE/RTDO1.C](../reference/v4.0/src/CMD/RESTORE/RTDO1.C) | c | 420 | not_translated |
| [v4.0/src/CMD/RESTORE/RTFILE.C](../reference/v4.0/src/CMD/RESTORE/RTFILE.C) | c | 598 | not_translated |
| [v4.0/src/CMD/RESTORE/RTFILE1.C](../reference/v4.0/src/CMD/RESTORE/RTFILE1.C) | c | 457 | not_translated |
| [v4.0/src/CMD/RESTORE/RTNEW.C](../reference/v4.0/src/CMD/RESTORE/RTNEW.C) | c | 245 | not_translated |
| [v4.0/src/CMD/RESTORE/RTNEW1.C](../reference/v4.0/src/CMD/RESTORE/RTNEW1.C) | c | 628 | not_translated |
| [v4.0/src/CMD/RESTORE/RTOLD.C](../reference/v4.0/src/CMD/RESTORE/RTOLD.C) | c | 267 | not_translated |
| [v4.0/src/CMD/RESTORE/RTOLD1.C](../reference/v4.0/src/CMD/RESTORE/RTOLD1.C) | c | 658 | not_translated |
| [v4.0/src/CMD/RESTORE/RTT.C](../reference/v4.0/src/CMD/RESTORE/RTT.C) | c | 410 | not_translated |
| [v4.0/src/CMD/RESTORE/RTT1.C](../reference/v4.0/src/CMD/RESTORE/RTT1.C) | c | 79 | not_translated |
| [v4.0/src/CMD/RESTORE/RTT2.C](../reference/v4.0/src/CMD/RESTORE/RTT2.C) | c | 488 | not_translated |
| [v4.0/src/CMD/RESTORE/RTT3.C](../reference/v4.0/src/CMD/RESTORE/RTT3.C) | c | 620 | not_translated |
| [v4.0/src/CMD/RESTORE/WRWDEFS.H](../reference/v4.0/src/CMD/RESTORE/WRWDEFS.H) | h | 77 | not_translated |
| [v4.0/src/CMD/RESTORE/_MSGRET.ASM](../reference/v4.0/src/CMD/RESTORE/_MSGRET.ASM) | asm | 242 | not_translated |
| [v4.0/src/CMD/RESTORE/_PARSE.ASM](../reference/v4.0/src/CMD/RESTORE/_PARSE.ASM) | asm | 115 | not_translated |
| [v4.0/src/CMD/SHARE/GSHARE.ASM](../reference/v4.0/src/CMD/SHARE/GSHARE.ASM) | asm | 2405 | not_translated |
| [v4.0/src/CMD/SHARE/GSHARE2.ASM](../reference/v4.0/src/CMD/SHARE/GSHARE2.ASM) | asm | 2979 | not_translated |
| [v4.0/src/CMD/SHARE/SHAREHDR.INC](../reference/v4.0/src/CMD/SHARE/SHAREHDR.INC) | inc | 73 | not_translated |
| [v4.0/src/CMD/SHARE/SHARELNK.ASM](../reference/v4.0/src/CMD/SHARE/SHARELNK.ASM) | asm | 214 | not_translated |
| [v4.0/src/CMD/SHARE/SHARESR.ASM](../reference/v4.0/src/CMD/SHARE/SHARESR.ASM) | asm | 73 | not_translated |
| [v4.0/src/CMD/SORT/SORT.ASM](../reference/v4.0/src/CMD/SORT/SORT.ASM) | asm | 1001 | not_translated |
| [v4.0/src/CMD/SORT/SORTMES.ASM](../reference/v4.0/src/CMD/SORT/SORTMES.ASM) | asm | 85 | not_translated |
| [v4.0/src/CMD/SUBST/SUBST.C](../reference/v4.0/src/CMD/SUBST/SUBST.C) | c | 720 | not_translated |
| [v4.0/src/CMD/SUBST/SUBSTPAR.H](../reference/v4.0/src/CMD/SUBST/SUBSTPAR.H) | h | 156 | not_translated |
| [v4.0/src/CMD/SUBST/_MSGRET.ASM](../reference/v4.0/src/CMD/SUBST/_MSGRET.ASM) | asm | 250 | not_translated |
| [v4.0/src/CMD/SUBST/_PARSE.ASM](../reference/v4.0/src/CMD/SUBST/_PARSE.ASM) | asm | 121 | not_translated |
| [v4.0/src/CMD/SYS/SYS1.ASM](../reference/v4.0/src/CMD/SYS/SYS1.ASM) | asm | 3710 | not_translated |
| [v4.0/src/CMD/SYS/SYS2.ASM](../reference/v4.0/src/CMD/SYS/SYS2.ASM) | asm | 2622 | not_translated |
| [v4.0/src/CMD/SYS/SYSHDR.INC](../reference/v4.0/src/CMD/SYS/SYSHDR.INC) | inc | 428 | not_translated |
| [v4.0/src/CMD/SYS/SYSSR.ASM](../reference/v4.0/src/CMD/SYS/SYSSR.ASM) | asm | 82 | not_translated |
| [v4.0/src/CMD/TREE/TREE.ASM](../reference/v4.0/src/CMD/TREE/TREE.ASM) | asm | 1736 | not_translated |
| [v4.0/src/CMD/TREE/TREEMS.INC](../reference/v4.0/src/CMD/TREE/TREEMS.INC) | inc | 70 | not_translated |
| [v4.0/src/CMD/TREE/TREEPAR.ASM](../reference/v4.0/src/CMD/TREE/TREEPAR.ASM) | asm | 510 | not_translated |
| [v4.0/src/CMD/TREE/TREEQU.INC](../reference/v4.0/src/CMD/TREE/TREEQU.INC) | inc | 229 | not_translated |
| [v4.0/src/CMD/TREE/TREESYSM.ASM](../reference/v4.0/src/CMD/TREE/TREESYSM.ASM) | asm | 151 | not_translated |
| [v4.0/src/CMD/TREE/TREESYSP.ASM](../reference/v4.0/src/CMD/TREE/TREESYSP.ASM) | asm | 127 | not_translated |
| [v4.0/src/CMD/XCOPY/XCOPY.ASM](../reference/v4.0/src/CMD/XCOPY/XCOPY.ASM) | asm | 3866 | not_translated |
| [v4.0/src/CMD/XCOPY/XCOPYPAR.ASM](../reference/v4.0/src/CMD/XCOPY/XCOPYPAR.ASM) | asm | 488 | not_translated |
| [v4.0/src/CMD/XCOPY/XCPYINIT.ASM](../reference/v4.0/src/CMD/XCOPY/XCPYINIT.ASM) | asm | 1890 | not_translated |
| [v4.0/src/DEV/ANSI/ANSI.ASM](../reference/v4.0/src/DEV/ANSI/ANSI.ASM) | asm | 1170 | not_translated |
| [v4.0/src/DEV/ANSI/ANSI.INC](../reference/v4.0/src/DEV/ANSI/ANSI.INC) | inc | 313 | not_translated |
| [v4.0/src/DEV/ANSI/ANSIINIT.ASM](../reference/v4.0/src/DEV/ANSI/ANSIINIT.ASM) | asm | 414 | not_translated |
| [v4.0/src/DEV/ANSI/ANSIVID.INC](../reference/v4.0/src/DEV/ANSI/ANSIVID.INC) | inc | 54 | not_translated |
| [v4.0/src/DEV/ANSI/IOCTL.ASM](../reference/v4.0/src/DEV/ANSI/IOCTL.ASM) | asm | 1127 | not_translated |
| [v4.0/src/DEV/ANSI/PARSER.ASM](../reference/v4.0/src/DEV/ANSI/PARSER.ASM) | asm | 326 | not_translated |
| [v4.0/src/DEV/COUNTRY/MKCNTRY.ASM](../reference/v4.0/src/DEV/COUNTRY/MKCNTRY.ASM) | asm | 2378 | not_translated |
| [v4.0/src/DEV/COUNTRY/MKCNTRY.INC](../reference/v4.0/src/DEV/COUNTRY/MKCNTRY.INC) | inc | 141 | not_translated |
| [v4.0/src/DEV/DISPLAY/CPS-FUNC.INC](../reference/v4.0/src/DEV/DISPLAY/CPS-FUNC.INC) | inc | 728 | not_translated |
| [v4.0/src/DEV/DISPLAY/DEF-EQU.INC](../reference/v4.0/src/DEV/DISPLAY/DEF-EQU.INC) | inc | 291 | not_translated |
| [v4.0/src/DEV/DISPLAY/DISPLAY.ASM](../reference/v4.0/src/DEV/DISPLAY/DISPLAY.ASM) | asm | 389 | not_translated |
| [v4.0/src/DEV/DISPLAY/DISPMES.ASM](../reference/v4.0/src/DEV/DISPLAY/DISPMES.ASM) | asm | 17 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/437-8X14.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/437-8X14.ASM) | asm | 270 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/437-8X16.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/437-8X16.ASM) | asm | 270 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/437-8X8.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/437-8X8.ASM) | asm | 271 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/437-CPI.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/437-CPI.ASM) | asm | 49 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/850-8X14.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/850-8X14.ASM) | asm | 271 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/850-8X16.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/850-8X16.ASM) | asm | 271 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/850-8X8.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/850-8X8.ASM) | asm | 271 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/850-CPI.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/850-CPI.ASM) | asm | 49 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/860-8X14.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/860-8X14.ASM) | asm | 271 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/860-8X16.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/860-8X16.ASM) | asm | 271 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/860-8X8.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/860-8X8.ASM) | asm | 271 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/860-CPI.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/860-CPI.ASM) | asm | 49 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/863-8X14.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/863-8X14.ASM) | asm | 271 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/863-8X16.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/863-8X16.ASM) | asm | 271 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/863-8X8.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/863-8X8.ASM) | asm | 271 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/863-CPI.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/863-CPI.ASM) | asm | 49 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/865-8X14.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/865-8X14.ASM) | asm | 271 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/865-8X16.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/865-8X16.ASM) | asm | 271 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/865-8X8.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/865-8X8.ASM) | asm | 271 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/865-CPI.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/865-CPI.ASM) | asm | 49 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/COPYRIGH.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/COPYRIGH.ASM) | asm | 12 | not_translated |
| [v4.0/src/DEV/DISPLAY/EGA/CPI-HEAD.ASM](../reference/v4.0/src/DEV/DISPLAY/EGA/CPI-HEAD.ASM) | asm | 15 | not_translated |
| [v4.0/src/DEV/DISPLAY/F-PARSER.INC](../reference/v4.0/src/DEV/DISPLAY/F-PARSER.INC) | inc | 2041 | not_translated |
| [v4.0/src/DEV/DISPLAY/INIT.ASM](../reference/v4.0/src/DEV/DISPLAY/INIT.ASM) | asm | 966 | not_translated |
| [v4.0/src/DEV/DISPLAY/INT10COM.INC](../reference/v4.0/src/DEV/DISPLAY/INT10COM.INC) | inc | 1030 | not_translated |
| [v4.0/src/DEV/DISPLAY/INT2FCOM.INC](../reference/v4.0/src/DEV/DISPLAY/INT2FCOM.INC) | inc | 324 | not_translated |
| [v4.0/src/DEV/DISPLAY/LCD/437-CPI.ASM](../reference/v4.0/src/DEV/DISPLAY/LCD/437-CPI.ASM) | asm | 290 | not_translated |
| [v4.0/src/DEV/DISPLAY/LCD/850-CPI.ASM](../reference/v4.0/src/DEV/DISPLAY/LCD/850-CPI.ASM) | asm | 290 | not_translated |
| [v4.0/src/DEV/DISPLAY/LCD/860-CPI.ASM](../reference/v4.0/src/DEV/DISPLAY/LCD/860-CPI.ASM) | asm | 290 | not_translated |
| [v4.0/src/DEV/DISPLAY/LCD/863-CPI.ASM](../reference/v4.0/src/DEV/DISPLAY/LCD/863-CPI.ASM) | asm | 291 | not_translated |
| [v4.0/src/DEV/DISPLAY/LCD/865-CPI.ASM](../reference/v4.0/src/DEV/DISPLAY/LCD/865-CPI.ASM) | asm | 293 | not_translated |
| [v4.0/src/DEV/DISPLAY/LCD/COPYRIGH.ASM](../reference/v4.0/src/DEV/DISPLAY/LCD/COPYRIGH.ASM) | asm | 11 | not_translated |
| [v4.0/src/DEV/DISPLAY/LCD/FONT-R3.ASM](../reference/v4.0/src/DEV/DISPLAY/LCD/FONT-R3.ASM) | asm | 15 | not_translated |
| [v4.0/src/DEV/DISPLAY/MACROS.INC](../reference/v4.0/src/DEV/DISPLAY/MACROS.INC) | inc | 29 | not_translated |
| [v4.0/src/DEV/DISPLAY/PARSER.ASM](../reference/v4.0/src/DEV/DISPLAY/PARSER.ASM) | asm | 810 | not_translated |
| [v4.0/src/DEV/DISPLAY/TABLES.INC](../reference/v4.0/src/DEV/DISPLAY/TABLES.INC) | inc | 369 | not_translated |
| [v4.0/src/DEV/DISPLAY/WRITE.INC](../reference/v4.0/src/DEV/DISPLAY/WRITE.INC) | inc | 373 | not_translated |
| [v4.0/src/DEV/DRIVER/DRIVER.ASM](../reference/v4.0/src/DEV/DRIVER/DRIVER.ASM) | asm | 1297 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDF.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDF.ASM) | asm | 394 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFBE.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFBE.ASM) | asm | 1834 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFCF.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFCF.ASM) | asm | 1770 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFDK.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFDK.ASM) | asm | 1632 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFEOF.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFEOF.ASM) | asm | 33 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFFR.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFFR.ASM) | asm | 1530 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFFR120.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFFR120.ASM) | asm | 2308 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFFR189.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFFR189.ASM) | asm | 1530 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFGE.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFGE.ASM) | asm | 1334 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFIT.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFIT.ASM) | asm | 795 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFIT141.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFIT141.ASM) | asm | 795 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFIT142.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFIT142.ASM) | asm | 865 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFLA.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFLA.ASM) | asm | 1323 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFNL.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFNL.ASM) | asm | 1516 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFNO.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFNO.ASM) | asm | 1608 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFNOW.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFNOW.ASM) | asm | 508 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFPO.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFPO.ASM) | asm | 1608 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFSF.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFSF.ASM) | asm | 1628 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFSG.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFSG.ASM) | asm | 1630 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFSP.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFSP.ASM) | asm | 1374 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFSU.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFSU.ASM) | asm | 1681 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFSV.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFSV.ASM) | asm | 1688 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFUK.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFUK.ASM) | asm | 656 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFUK166.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFUK166.ASM) | asm | 656 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KDFUK168.ASM](../reference/v4.0/src/DEV/KEYBOARD/KDFUK168.ASM) | asm | 849 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KEYBMAC.INC](../reference/v4.0/src/DEV/KEYBOARD/KEYBMAC.INC) | inc | 324 | not_translated |
| [v4.0/src/DEV/KEYBOARD/KEYBSHAR.INC](../reference/v4.0/src/DEV/KEYBOARD/KEYBSHAR.INC) | inc | 305 | not_translated |
| [v4.0/src/DEV/PRINTER/4201/4201.INC](../reference/v4.0/src/DEV/PRINTER/4201/4201.INC) | inc | 802 | not_translated |
| [v4.0/src/DEV/PRINTER/4208/4208-CPY.ASM](../reference/v4.0/src/DEV/PRINTER/4208/4208-CPY.ASM) | asm | 10 | not_translated |
| [v4.0/src/DEV/PRINTER/4208/4208.INC](../reference/v4.0/src/DEV/PRINTER/4208/4208.INC) | inc | 91 | not_translated |
| [v4.0/src/DEV/PRINTER/5202/5202.ASM](../reference/v4.0/src/DEV/PRINTER/5202/5202.ASM) | asm | 109 | not_translated |
| [v4.0/src/DEV/PRINTER/CPSFONT.ASM](../reference/v4.0/src/DEV/PRINTER/CPSFONT.ASM) | asm | 1962 | not_translated |
| [v4.0/src/DEV/PRINTER/CPSFONT3.ASM](../reference/v4.0/src/DEV/PRINTER/CPSFONT3.ASM) | asm | 1962 | not_translated |
| [v4.0/src/DEV/PRINTER/CPSPEQU.INC](../reference/v4.0/src/DEV/PRINTER/CPSPEQU.INC) | inc | 578 | not_translated |
| [v4.0/src/DEV/PRINTER/CPSPI.ASM](../reference/v4.0/src/DEV/PRINTER/CPSPI.ASM) | asm | 2423 | not_translated |
| [v4.0/src/DEV/PRINTER/CPSPI07.ASM](../reference/v4.0/src/DEV/PRINTER/CPSPI07.ASM) | asm | 1417 | not_translated |
| [v4.0/src/DEV/PRINTER/CPSPM10.ASM](../reference/v4.0/src/DEV/PRINTER/CPSPM10.ASM) | asm | 3852 | not_translated |
| [v4.0/src/DEV/PRINTER/PARSE4E.ASM](../reference/v4.0/src/DEV/PRINTER/PARSE4E.ASM) | asm | 1107 | not_translated |
| [v4.0/src/DEV/PRINTER/PARSER.ASM](../reference/v4.0/src/DEV/PRINTER/PARSER.ASM) | asm | 669 | not_translated |
| [v4.0/src/DEV/PRINTER/PRTINT2F.ASM](../reference/v4.0/src/DEV/PRINTER/PRTINT2F.ASM) | asm | 451 | not_translated |
| [v4.0/src/DEV/PRINTER/PTRMSG.ASM](../reference/v4.0/src/DEV/PRINTER/PTRMSG.ASM) | asm | 41 | not_translated |
| [v4.0/src/DEV/RAMDRIVE/ABOVE.INC](../reference/v4.0/src/DEV/RAMDRIVE/ABOVE.INC) | inc | 62 | not_translated |
| [v4.0/src/DEV/RAMDRIVE/AB_MACRO.INC](../reference/v4.0/src/DEV/RAMDRIVE/AB_MACRO.INC) | inc | 181 | not_translated |
| [v4.0/src/DEV/RAMDRIVE/DEVSYM.INC](../reference/v4.0/src/DEV/RAMDRIVE/DEVSYM.INC) | inc | 129 | not_translated |
| [v4.0/src/DEV/RAMDRIVE/DIRENT.INC](../reference/v4.0/src/DEV/RAMDRIVE/DIRENT.INC) | inc | 57 | not_translated |
| [v4.0/src/DEV/RAMDRIVE/EMM.INC](../reference/v4.0/src/DEV/RAMDRIVE/EMM.INC) | inc | 224 | not_translated |
| [v4.0/src/DEV/RAMDRIVE/LOADALL.INC](../reference/v4.0/src/DEV/RAMDRIVE/LOADALL.INC) | inc | 36 | not_translated |
| [v4.0/src/DEV/RAMDRIVE/MESSAGES.ASM](../reference/v4.0/src/DEV/RAMDRIVE/MESSAGES.ASM) | asm | 83 | not_translated |
| [v4.0/src/DEV/RAMDRIVE/MI.INC](../reference/v4.0/src/DEV/RAMDRIVE/MI.INC) | inc | 19 | not_translated |
| [v4.0/src/DEV/RAMDRIVE/RAMDRIVE.ASM](../reference/v4.0/src/DEV/RAMDRIVE/RAMDRIVE.ASM) | asm | 6218 | not_translated |
| [v4.0/src/DEV/RAMDRIVE/SYSCALL.INC](../reference/v4.0/src/DEV/RAMDRIVE/SYSCALL.INC) | inc | 147 | not_translated |
| [v4.0/src/DEV/SMARTDRV/ABOVE.ASM](../reference/v4.0/src/DEV/SMARTDRV/ABOVE.ASM) | asm | 63 | not_translated |
| [v4.0/src/DEV/SMARTDRV/AB_MACRO.ASM](../reference/v4.0/src/DEV/SMARTDRV/AB_MACRO.ASM) | asm | 181 | not_translated |
| [v4.0/src/DEV/SMARTDRV/CMACROS.INC](../reference/v4.0/src/DEV/SMARTDRV/CMACROS.INC) | inc | 933 | not_translated |
| [v4.0/src/DEV/SMARTDRV/DEVSYM.ASM](../reference/v4.0/src/DEV/SMARTDRV/DEVSYM.ASM) | asm | 129 | not_translated |
| [v4.0/src/DEV/SMARTDRV/DIRENT.ASM](../reference/v4.0/src/DEV/SMARTDRV/DIRENT.ASM) | asm | 57 | not_translated |
| [v4.0/src/DEV/SMARTDRV/EMM.ASM](../reference/v4.0/src/DEV/SMARTDRV/EMM.ASM) | asm | 224 | not_translated |
| [v4.0/src/DEV/SMARTDRV/FL13.ASM](../reference/v4.0/src/DEV/SMARTDRV/FL13.ASM) | asm | 149 | not_translated |
| [v4.0/src/DEV/SMARTDRV/FLMES.ASM](../reference/v4.0/src/DEV/SMARTDRV/FLMES.ASM) | asm | 87 | not_translated |
| [v4.0/src/DEV/SMARTDRV/FLUSH13.C](../reference/v4.0/src/DEV/SMARTDRV/FLUSH13.C) | c | 687 | not_translated |
| [v4.0/src/DEV/SMARTDRV/LOADALL.ASM](../reference/v4.0/src/DEV/SMARTDRV/LOADALL.ASM) | asm | 36 | not_translated |
| [v4.0/src/DEV/SMARTDRV/MI.ASM](../reference/v4.0/src/DEV/SMARTDRV/MI.ASM) | asm | 19 | not_translated |
| [v4.0/src/DEV/SMARTDRV/SMARTDRV.ASM](../reference/v4.0/src/DEV/SMARTDRV/SMARTDRV.ASM) | asm | 7587 | not_translated |
| [v4.0/src/DEV/SMARTDRV/SYSCALL.ASM](../reference/v4.0/src/DEV/SMARTDRV/SYSCALL.ASM) | asm | 147 | not_translated |
| [v4.0/src/DEV/VDISK/VDISK.ASM](../reference/v4.0/src/DEV/VDISK/VDISK.ASM) | asm | 2222 | not_translated |
| [v4.0/src/DEV/VDISK/VDISK.INC](../reference/v4.0/src/DEV/VDISK/VDISK.INC) | inc | 28 | not_translated |
| [v4.0/src/DEV/VDISK/VDISKMSG.ASM](../reference/v4.0/src/DEV/VDISK/VDISKMSG.ASM) | asm | 28 | not_translated |
| [v4.0/src/DEV/VDISK/VDISKSYS.ASM](../reference/v4.0/src/DEV/VDISK/VDISKSYS.ASM) | asm | 3024 | not_translated |
| [v4.0/src/DEV/VDISK/VDISKSYS.INC](../reference/v4.0/src/DEV/VDISK/VDISKSYS.INC) | inc | 38 | not_translated |
| [v4.0/src/DEV/XMA2EMS/DIAGS.ASM](../reference/v4.0/src/DEV/XMA2EMS/DIAGS.ASM) | asm | 1910 | not_translated |
| [v4.0/src/DEV/XMA2EMS/EMSINIT.INC](../reference/v4.0/src/DEV/XMA2EMS/EMSINIT.INC) | inc | 887 | not_translated |
| [v4.0/src/DEV/XMA2EMS/GENIOCTL.INC](../reference/v4.0/src/DEV/XMA2EMS/GENIOCTL.INC) | inc | 167 | not_translated |
| [v4.0/src/DEV/XMA2EMS/I13HOOK.INC](../reference/v4.0/src/DEV/XMA2EMS/I13HOOK.INC) | inc | 450 | not_translated |
| [v4.0/src/DEV/XMA2EMS/LIM40.INC](../reference/v4.0/src/DEV/XMA2EMS/LIM40.INC) | inc | 1793 | not_translated |
| [v4.0/src/DEV/XMA2EMS/LIM40B.INC](../reference/v4.0/src/DEV/XMA2EMS/LIM40B.INC) | inc | 3469 | not_translated |
| [v4.0/src/DEV/XMA2EMS/PARMPARS.INC](../reference/v4.0/src/DEV/XMA2EMS/PARMPARS.INC) | inc | 531 | not_translated |
| [v4.0/src/DEV/XMA2EMS/PS2_5060.INC](../reference/v4.0/src/DEV/XMA2EMS/PS2_5060.INC) | inc | 735 | not_translated |
| [v4.0/src/DEV/XMA2EMS/ROMSCAN.INC](../reference/v4.0/src/DEV/XMA2EMS/ROMSCAN.INC) | inc | 420 | not_translated |
| [v4.0/src/DEV/XMA2EMS/XMA1DIAG.INC](../reference/v4.0/src/DEV/XMA2EMS/XMA1DIAG.INC) | inc | 1871 | not_translated |
| [v4.0/src/DEV/XMA2EMS/XMA2EMS.ASM](../reference/v4.0/src/DEV/XMA2EMS/XMA2EMS.ASM) | asm | 2592 | not_translated |
| [v4.0/src/DEV/XMA2EMS/XMA2EMSP.INC](../reference/v4.0/src/DEV/XMA2EMS/XMA2EMSP.INC) | inc | 243 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEACC.INC](../reference/v4.0/src/DEV/XMAEM/INDEACC.INC) | inc | 219 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEDAT.INC](../reference/v4.0/src/DEV/XMAEM/INDEDAT.INC) | inc | 536 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEDMA.ASM](../reference/v4.0/src/DEV/XMAEM/INDEDMA.ASM) | asm | 661 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEEMU.ASM](../reference/v4.0/src/DEV/XMAEM/INDEEMU.ASM) | asm | 654 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEEXC.ASM](../reference/v4.0/src/DEV/XMAEM/INDEEXC.ASM) | asm | 800 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEGDT.ASM](../reference/v4.0/src/DEV/XMAEM/INDEGDT.ASM) | asm | 380 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEI15.ASM](../reference/v4.0/src/DEV/XMAEM/INDEI15.ASM) | asm | 504 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEIDT.ASM](../reference/v4.0/src/DEV/XMAEM/INDEIDT.ASM) | asm | 513 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEINI.ASM](../reference/v4.0/src/DEV/XMAEM/INDEINI.ASM) | asm | 1474 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEMAUS.ASM](../reference/v4.0/src/DEV/XMAEM/INDEMAUS.ASM) | asm | 74 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEMSG.ASM](../reference/v4.0/src/DEV/XMAEM/INDEMSG.ASM) | asm | 97 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEMSUS.ASM](../reference/v4.0/src/DEV/XMAEM/INDEMSUS.ASM) | asm | 77 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEMSUS.INC](../reference/v4.0/src/DEV/XMAEM/INDEMSUS.INC) | inc | 65 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEPAT.ASM](../reference/v4.0/src/DEV/XMAEM/INDEPAT.ASM) | asm | 75 | not_translated |
| [v4.0/src/DEV/XMAEM/INDEXMA.ASM](../reference/v4.0/src/DEV/XMAEM/INDEXMA.ASM) | asm | 782 | not_translated |
| [v4.0/src/DOS/ABORT.ASM](../reference/v4.0/src/DOS/ABORT.ASM) | asm | 138 | not_translated |
| [v4.0/src/DOS/ALLOC.ASM](../reference/v4.0/src/DOS/ALLOC.ASM) | asm | 393 | not_translated |
| [v4.0/src/DOS/BUF.ASM](../reference/v4.0/src/DOS/BUF.ASM) | asm | 982 | not_translated |
| [v4.0/src/DOS/CLOSE.ASM](../reference/v4.0/src/DOS/CLOSE.ASM) | asm | 433 | not_translated |
| [v4.0/src/DOS/CPMIO.ASM](../reference/v4.0/src/DOS/CPMIO.ASM) | asm | 449 | not_translated |
| [v4.0/src/DOS/CPMIO2.ASM](../reference/v4.0/src/DOS/CPMIO2.ASM) | asm | 355 | not_translated |
| [v4.0/src/DOS/CREATE.ASM](../reference/v4.0/src/DOS/CREATE.ASM) | asm | 380 | not_translated |
| [v4.0/src/DOS/CRIT.ASM](../reference/v4.0/src/DOS/CRIT.ASM) | asm | 93 | not_translated |
| [v4.0/src/DOS/CTRLC.ASM](../reference/v4.0/src/DOS/CTRLC.ASM) | asm | 771 | not_translated |
| [v4.0/src/DOS/DELETE.ASM](../reference/v4.0/src/DOS/DELETE.ASM) | asm | 778 | not_translated |
| [v4.0/src/DOS/DEV.ASM](../reference/v4.0/src/DOS/DEV.ASM) | asm | 841 | not_translated |
| [v4.0/src/DOS/DINFO.ASM](../reference/v4.0/src/DOS/DINFO.ASM) | asm | 132 | not_translated |
| [v4.0/src/DOS/DIR.ASM](../reference/v4.0/src/DOS/DIR.ASM) | asm | 446 | not_translated |
| [v4.0/src/DOS/DIR2.ASM](../reference/v4.0/src/DOS/DIR2.ASM) | asm | 1354 | not_translated |
| [v4.0/src/DOS/DIRCALL.ASM](../reference/v4.0/src/DOS/DIRCALL.ASM) | asm | 416 | not_translated |
| [v4.0/src/DOS/DISK.ASM](../reference/v4.0/src/DOS/DISK.ASM) | asm | 1073 | not_translated |
| [v4.0/src/DOS/DISK2.ASM](../reference/v4.0/src/DOS/DISK2.ASM) | asm | 784 | not_translated |
| [v4.0/src/DOS/DISK3.ASM](../reference/v4.0/src/DOS/DISK3.ASM) | asm | 662 | not_translated |
| [v4.0/src/DOS/DISP.ASM](../reference/v4.0/src/DOS/DISP.ASM) | asm | 580 | not_translated |
| [v4.0/src/DOS/DISPATCH.ASM](../reference/v4.0/src/DOS/DISPATCH.ASM) | asm | 215 | not_translated |
| [v4.0/src/DOS/DOSMES.ASM](../reference/v4.0/src/DOS/DOSMES.ASM) | asm | 443 | not_translated |
| [v4.0/src/DOS/DOSPRINT.ASM](../reference/v4.0/src/DOS/DOSPRINT.ASM) | asm | 37 | not_translated |
| [v4.0/src/DOS/DUP.ASM](../reference/v4.0/src/DOS/DUP.ASM) | asm | 69 | not_translated |
| [v4.0/src/DOS/EXEC.ASM](../reference/v4.0/src/DOS/EXEC.ASM) | asm | 931 | not_translated |
| [v4.0/src/DOS/EXTATTR.ASM](../reference/v4.0/src/DOS/EXTATTR.ASM) | asm | 680 | not_translated |
| [v4.0/src/DOS/FAT.ASM](../reference/v4.0/src/DOS/FAT.ASM) | asm | 696 | not_translated |
| [v4.0/src/DOS/FCB.ASM](../reference/v4.0/src/DOS/FCB.ASM) | asm | 505 | not_translated |
| [v4.0/src/DOS/FCBIO.ASM](../reference/v4.0/src/DOS/FCBIO.ASM) | asm | 1186 | not_translated |
| [v4.0/src/DOS/FCBIO2.ASM](../reference/v4.0/src/DOS/FCBIO2.ASM) | asm | 721 | not_translated |
| [v4.0/src/DOS/FILE.ASM](../reference/v4.0/src/DOS/FILE.ASM) | asm | 909 | not_translated |
| [v4.0/src/DOS/FINFO.ASM](../reference/v4.0/src/DOS/FINFO.ASM) | asm | 287 | not_translated |
| [v4.0/src/DOS/GETSET.ASM](../reference/v4.0/src/DOS/GETSET.ASM) | asm | 913 | not_translated |
| [v4.0/src/DOS/HANDLE.ASM](../reference/v4.0/src/DOS/HANDLE.ASM) | asm | 893 | not_translated |
| [v4.0/src/DOS/HIGHSW.ASM](../reference/v4.0/src/DOS/HIGHSW.ASM) | asm | 30 | not_translated |
| [v4.0/src/DOS/IFS.ASM](../reference/v4.0/src/DOS/IFS.ASM) | asm | 524 | not_translated |
| [v4.0/src/DOS/IOCTL.ASM](../reference/v4.0/src/DOS/IOCTL.ASM) | asm | 746 | not_translated |
| [v4.0/src/DOS/ISEARCH.ASM](../reference/v4.0/src/DOS/ISEARCH.ASM) | asm | 365 | not_translated |
| [v4.0/src/DOS/KSTRIN.ASM](../reference/v4.0/src/DOS/KSTRIN.ASM) | asm | 691 | not_translated |
| [v4.0/src/DOS/LOCK.ASM](../reference/v4.0/src/DOS/LOCK.ASM) | asm | 373 | not_translated |
| [v4.0/src/DOS/MACRO.ASM](../reference/v4.0/src/DOS/MACRO.ASM) | asm | 446 | not_translated |
| [v4.0/src/DOS/MACRO2.ASM](../reference/v4.0/src/DOS/MACRO2.ASM) | asm | 1282 | not_translated |
| [v4.0/src/DOS/MISC.ASM](../reference/v4.0/src/DOS/MISC.ASM) | asm | 573 | not_translated |
| [v4.0/src/DOS/MISC2.ASM](../reference/v4.0/src/DOS/MISC2.ASM) | asm | 595 | not_translated |
| [v4.0/src/DOS/MKNODE.ASM](../reference/v4.0/src/DOS/MKNODE.ASM) | asm | 890 | not_translated |
| [v4.0/src/DOS/MSCODE.ASM](../reference/v4.0/src/DOS/MSCODE.ASM) | asm | 17 | not_translated |
| [v4.0/src/DOS/MSCONST.ASM](../reference/v4.0/src/DOS/MSCONST.ASM) | asm | 153 | not_translated |
| [v4.0/src/DOS/MSCPMIO.ASM](../reference/v4.0/src/DOS/MSCPMIO.ASM) | asm | 17 | not_translated |
| [v4.0/src/DOS/MSCTRLC.ASM](../reference/v4.0/src/DOS/MSCTRLC.ASM) | asm | 16 | not_translated |
| [v4.0/src/DOS/MSDISP.ASM](../reference/v4.0/src/DOS/MSDISP.ASM) | asm | 17 | not_translated |
| [v4.0/src/DOS/MSHALO.ASM](../reference/v4.0/src/DOS/MSHALO.ASM) | asm | 248 | not_translated |
| [v4.0/src/DOS/MSINIT.ASM](../reference/v4.0/src/DOS/MSINIT.ASM) | asm | 668 | not_translated |
| [v4.0/src/DOS/MSIOCTL.ASM](../reference/v4.0/src/DOS/MSIOCTL.ASM) | asm | 5 | not_translated |
| [v4.0/src/DOS/MSPROC.ASM](../reference/v4.0/src/DOS/MSPROC.ASM) | asm | 16 | not_translated |
| [v4.0/src/DOS/MSSW.ASM](../reference/v4.0/src/DOS/MSSW.ASM) | asm | 27 | not_translated |
| [v4.0/src/DOS/MS_CODE.ASM](../reference/v4.0/src/DOS/MS_CODE.ASM) | asm | 614 | not_translated |
| [v4.0/src/DOS/MS_TABLE.ASM](../reference/v4.0/src/DOS/MS_TABLE.ASM) | asm | 710 | not_translated |
| [v4.0/src/DOS/NSTDOS.ASM](../reference/v4.0/src/DOS/NSTDOS.ASM) | asm | 8 | not_translated |
| [v4.0/src/DOS/OPEN.ASM](../reference/v4.0/src/DOS/OPEN.ASM) | asm | 570 | not_translated |
| [v4.0/src/DOS/PARSE.ASM](../reference/v4.0/src/DOS/PARSE.ASM) | asm | 448 | not_translated |
| [v4.0/src/DOS/PATH.ASM](../reference/v4.0/src/DOS/PATH.ASM) | asm | 363 | not_translated |
| [v4.0/src/DOS/PRINT.ASM](../reference/v4.0/src/DOS/PRINT.ASM) | asm | 200 | not_translated |
| [v4.0/src/DOS/PROC.ASM](../reference/v4.0/src/DOS/PROC.ASM) | asm | 188 | not_translated |
| [v4.0/src/DOS/RENAME.ASM](../reference/v4.0/src/DOS/RENAME.ASM) | asm | 435 | not_translated |
| [v4.0/src/DOS/ROM.ASM](../reference/v4.0/src/DOS/ROM.ASM) | asm | 938 | not_translated |
| [v4.0/src/DOS/SEARCH.ASM](../reference/v4.0/src/DOS/SEARCH.ASM) | asm | 347 | not_translated |
| [v4.0/src/DOS/SEGCHECK.ASM](../reference/v4.0/src/DOS/SEGCHECK.ASM) | asm | 289 | not_translated |
| [v4.0/src/DOS/SHARE.ASM](../reference/v4.0/src/DOS/SHARE.ASM) | asm | 152 | not_translated |
| [v4.0/src/DOS/SHRPRINT.ASM](../reference/v4.0/src/DOS/SHRPRINT.ASM) | asm | 36 | not_translated |
| [v4.0/src/DOS/SRVCALL.ASM](../reference/v4.0/src/DOS/SRVCALL.ASM) | asm | 307 | not_translated |
| [v4.0/src/DOS/STDASW.ASM](../reference/v4.0/src/DOS/STDASW.ASM) | asm | 30 | not_translated |
| [v4.0/src/DOS/STDCODE.ASM](../reference/v4.0/src/DOS/STDCODE.ASM) | asm | 17 | not_translated |
| [v4.0/src/DOS/STDCPMIO.ASM](../reference/v4.0/src/DOS/STDCPMIO.ASM) | asm | 17 | not_translated |
| [v4.0/src/DOS/STDCTRLC.ASM](../reference/v4.0/src/DOS/STDCTRLC.ASM) | asm | 16 | not_translated |
| [v4.0/src/DOS/STDDATA.ASM](../reference/v4.0/src/DOS/STDDATA.ASM) | asm | 34 | not_translated |
| [v4.0/src/DOS/STDDISP.ASM](../reference/v4.0/src/DOS/STDDISP.ASM) | asm | 17 | not_translated |
| [v4.0/src/DOS/STDDOSME.ASM](../reference/v4.0/src/DOS/STDDOSME.ASM) | asm | 20 | not_translated |
| [v4.0/src/DOS/STDIOCTL.ASM](../reference/v4.0/src/DOS/STDIOCTL.ASM) | asm | 5 | not_translated |
| [v4.0/src/DOS/STDPROC.ASM](../reference/v4.0/src/DOS/STDPROC.ASM) | asm | 16 | not_translated |
| [v4.0/src/DOS/STDSW.ASM](../reference/v4.0/src/DOS/STDSW.ASM) | asm | 33 | not_translated |
| [v4.0/src/DOS/STDTABLE.ASM](../reference/v4.0/src/DOS/STDTABLE.ASM) | asm | 17 | not_translated |
| [v4.0/src/DOS/STRIN.ASM](../reference/v4.0/src/DOS/STRIN.ASM) | asm | 403 | not_translated |
| [v4.0/src/DOS/TIME.ASM](../reference/v4.0/src/DOS/TIME.ASM) | asm | 380 | not_translated |
| [v4.0/src/DOS/UTIL.ASM](../reference/v4.0/src/DOS/UTIL.ASM) | asm | 233 | not_translated |
| [v4.0/src/H/BASEMID.H](../reference/v4.0/src/H/BASEMID.H) | h | 247 | not_translated |
| [v4.0/src/H/CDS.H](../reference/v4.0/src/H/CDS.H) | h | 25 | not_translated |
| [v4.0/src/H/COMSUB.H](../reference/v4.0/src/H/COMSUB.H) | h | 167 | not_translated |
| [v4.0/src/H/COPYRIGH.H](../reference/v4.0/src/H/COPYRIGH.H) | h | 4 | not_translated |
| [v4.0/src/H/DOSCALLS.H](../reference/v4.0/src/H/DOSCALLS.H) | h | 553 | not_translated |
| [v4.0/src/H/DPB.H](../reference/v4.0/src/H/DPB.H) | h | 30 | not_translated |
| [v4.0/src/H/ERROR.H](../reference/v4.0/src/H/ERROR.H) | h | 295 | not_translated |
| [v4.0/src/H/INTERNAT.H](../reference/v4.0/src/H/INTERNAT.H) | h | 31 | not_translated |
| [v4.0/src/H/JOINTYPE.H](../reference/v4.0/src/H/JOINTYPE.H) | h | 88 | not_translated |
| [v4.0/src/H/MSGRET.H](../reference/v4.0/src/H/MSGRET.H) | h | 67 | not_translated |
| [v4.0/src/H/SUBCALLS.H](../reference/v4.0/src/H/SUBCALLS.H) | h | 463 | not_translated |
| [v4.0/src/H/SYSVAR.H](../reference/v4.0/src/H/SYSVAR.H) | h | 30 | not_translated |
| [v4.0/src/H/TYPES.H](../reference/v4.0/src/H/TYPES.H) | h | 88 | not_translated |
| [v4.0/src/H/UTILMID.H](../reference/v4.0/src/H/UTILMID.H) | h | 507 | not_translated |
| [v4.0/src/H/UTILMID2.H](../reference/v4.0/src/H/UTILMID2.H) | h | 209 | not_translated |
| [v4.0/src/H/UTILMID3.H](../reference/v4.0/src/H/UTILMID3.H) | h | 101 | not_translated |
| [v4.0/src/H/UTLERROR.H](../reference/v4.0/src/H/UTLERROR.H) | h | 93 | not_translated |
| [v4.0/src/H/VERSION.H](../reference/v4.0/src/H/VERSION.H) | h | 7 | not_translated |
| [v4.0/src/H/VERSIONC.H](../reference/v4.0/src/H/VERSIONC.H) | h | 16 | not_translated |
| [v4.0/src/INC/ARENA.INC](../reference/v4.0/src/INC/ARENA.INC) | inc | 30 | not_translated |
| [v4.0/src/INC/BOOT.INC](../reference/v4.0/src/INC/BOOT.INC) | inc | 65 | not_translated |
| [v4.0/src/INC/BOOT11.INC](../reference/v4.0/src/INC/BOOT11.INC) | inc | 65 | not_translated |
| [v4.0/src/INC/BOOTFORM.INC](../reference/v4.0/src/INC/BOOTFORM.INC) | inc | 48 | not_translated |
| [v4.0/src/INC/BPB.INC](../reference/v4.0/src/INC/BPB.INC) | inc | 41 | not_translated |
| [v4.0/src/INC/BUFFER.INC](../reference/v4.0/src/INC/BUFFER.INC) | inc | 78 | not_translated |
| [v4.0/src/INC/BUGTYP.ASM](../reference/v4.0/src/INC/BUGTYP.ASM) | asm | 50 | not_translated |
| [v4.0/src/INC/CASRN.INC](../reference/v4.0/src/INC/CASRN.INC) | inc | 59 | not_translated |
| [v4.0/src/INC/CASVAR.INC](../reference/v4.0/src/INC/CASVAR.INC) | inc | 89 | not_translated |
| [v4.0/src/INC/CDS.C](../reference/v4.0/src/INC/CDS.C) | c | 132 | not_translated |
| [v4.0/src/INC/CMACROS.INC](../reference/v4.0/src/INC/CMACROS.INC) | inc | 933 | not_translated |
| [v4.0/src/INC/CONST2.ASM](../reference/v4.0/src/INC/CONST2.ASM) | asm | 212 | not_translated |
| [v4.0/src/INC/COPYRIGH.INC](../reference/v4.0/src/INC/COPYRIGH.INC) | inc | 3 | not_translated |
| [v4.0/src/INC/CPMFCB.INC](../reference/v4.0/src/INC/CPMFCB.INC) | inc | 125 | not_translated |
| [v4.0/src/INC/CPUTYPE.INC](../reference/v4.0/src/INC/CPUTYPE.INC) | inc | 40 | not_translated |
| [v4.0/src/INC/CURDIR.INC](../reference/v4.0/src/INC/CURDIR.INC) | inc | 46 | not_translated |
| [v4.0/src/INC/DEVSYM.INC](../reference/v4.0/src/INC/DEVSYM.INC) | inc | 208 | not_translated |
| [v4.0/src/INC/DIRENT.INC](../reference/v4.0/src/INC/DIRENT.INC) | inc | 62 | not_translated |
| [v4.0/src/INC/DIVMES.ASM](../reference/v4.0/src/INC/DIVMES.ASM) | asm | 11 | not_translated |
| [v4.0/src/INC/DOSCNTRY.INC](../reference/v4.0/src/INC/DOSCNTRY.INC) | inc | 115 | not_translated |
| [v4.0/src/INC/DOSMAC.INC](../reference/v4.0/src/INC/DOSMAC.INC) | inc | 631 | not_translated |
| [v4.0/src/INC/DOSSEG.ASM](../reference/v4.0/src/INC/DOSSEG.ASM) | asm | 26 | not_translated |
| [v4.0/src/INC/DOSSYM.INC](../reference/v4.0/src/INC/DOSSYM.INC) | inc | 157 | not_translated |
| [v4.0/src/INC/DPB.C](../reference/v4.0/src/INC/DPB.C) | c | 38 | not_translated |
| [v4.0/src/INC/DPB.INC](../reference/v4.0/src/INC/DPB.INC) | inc | 35 | not_translated |
| [v4.0/src/INC/DPL.ASM](../reference/v4.0/src/INC/DPL.ASM) | asm | 16 | not_translated |
| [v4.0/src/INC/DSEG.INC](../reference/v4.0/src/INC/DSEG.INC) | inc | 209 | not_translated |
| [v4.0/src/INC/EA.INC](../reference/v4.0/src/INC/EA.INC) | inc | 72 | not_translated |
| [v4.0/src/INC/ERROR.INC](../reference/v4.0/src/INC/ERROR.INC) | inc | 156 | not_translated |
| [v4.0/src/INC/ERRTST.C](../reference/v4.0/src/INC/ERRTST.C) | c | 310 | not_translated |
| [v4.0/src/INC/EXE.INC](../reference/v4.0/src/INC/EXE.INC) | inc | 79 | not_translated |
| [v4.0/src/INC/FASTOPEN.INC](../reference/v4.0/src/INC/FASTOPEN.INC) | inc | 149 | not_translated |
| [v4.0/src/INC/FASTSEEK.INC](../reference/v4.0/src/INC/FASTSEEK.INC) | inc | 23 | not_translated |
| [v4.0/src/INC/FASTXXXX.INC](../reference/v4.0/src/INC/FASTXXXX.INC) | inc | 13 | not_translated |
| [v4.0/src/INC/FILEMODE.INC](../reference/v4.0/src/INC/FILEMODE.INC) | inc | 56 | not_translated |
| [v4.0/src/INC/FIND.INC](../reference/v4.0/src/INC/FIND.INC) | inc | 26 | not_translated |
| [v4.0/src/INC/IFSSYM.INC](../reference/v4.0/src/INC/IFSSYM.INC) | inc | 671 | not_translated |
| [v4.0/src/INC/INTNAT.INC](../reference/v4.0/src/INC/INTNAT.INC) | inc | 40 | not_translated |
| [v4.0/src/INC/IOCTL.INC](../reference/v4.0/src/INC/IOCTL.INC) | inc | 212 | not_translated |
| [v4.0/src/INC/KSTRING.C](../reference/v4.0/src/INC/KSTRING.C) | c | 119 | not_translated |
| [v4.0/src/INC/LOCK.INC](../reference/v4.0/src/INC/LOCK.INC) | inc | 24 | not_translated |
| [v4.0/src/INC/MFT.INC](../reference/v4.0/src/INC/MFT.INC) | inc | 176 | not_translated |
| [v4.0/src/INC/MI.INC](../reference/v4.0/src/INC/MI.INC) | inc | 20 | not_translated |
| [v4.0/src/INC/MSBDS.INC](../reference/v4.0/src/INC/MSBDS.INC) | inc | 129 | not_translated |
| [v4.0/src/INC/MSDATA.ASM](../reference/v4.0/src/INC/MSDATA.ASM) | asm | 34 | not_translated |
| [v4.0/src/INC/MSDATA2.ASM](../reference/v4.0/src/INC/MSDATA2.ASM) | asm | 34 | not_translated |
| [v4.0/src/INC/MSDOSME.ASM](../reference/v4.0/src/INC/MSDOSME.ASM) | asm | 17 | not_translated |
| [v4.0/src/INC/MSGDCL.INC](../reference/v4.0/src/INC/MSGDCL.INC) | inc | 108 | not_translated |
| [v4.0/src/INC/MSGHAN.INC](../reference/v4.0/src/INC/MSGHAN.INC) | inc | 154 | not_translated |
| [v4.0/src/INC/MSGSERV.ASM](../reference/v4.0/src/INC/MSGSERV.ASM) | asm | 3133 | not_translated |
| [v4.0/src/INC/MSHALO.ASM](../reference/v4.0/src/INC/MSHALO.ASM) | asm | 248 | not_translated |
| [v4.0/src/INC/MSHEAD.ASM](../reference/v4.0/src/INC/MSHEAD.ASM) | asm | 135 | not_translated |
| [v4.0/src/INC/MSTABLE.ASM](../reference/v4.0/src/INC/MSTABLE.ASM) | asm | 17 | not_translated |
| [v4.0/src/INC/MS_DATA.ASM](../reference/v4.0/src/INC/MS_DATA.ASM) | asm | 194 | not_translated |
| [v4.0/src/INC/MULT.INC](../reference/v4.0/src/INC/MULT.INC) | inc | 185 | not_translated |
| [v4.0/src/INC/NIBDOS.ASM](../reference/v4.0/src/INC/NIBDOS.ASM) | asm | 8 | not_translated |
| [v4.0/src/INC/PARSE.ASM](../reference/v4.0/src/INC/PARSE.ASM) | asm | 2975 | not_translated |
| [v4.0/src/INC/PATHMAC.INC](../reference/v4.0/src/INC/PATHMAC.INC) | inc | 45 | not_translated |
| [v4.0/src/INC/PCGEQU.INC](../reference/v4.0/src/INC/PCGEQU.INC) | inc | 185 | not_translated |
| [v4.0/src/INC/PCINPUT.INC](../reference/v4.0/src/INC/PCINPUT.INC) | inc | 3795 | not_translated |
| [v4.0/src/INC/PDB.INC](../reference/v4.0/src/INC/PDB.INC) | inc | 51 | not_translated |
| [v4.0/src/INC/POSTEQU.INC](../reference/v4.0/src/INC/POSTEQU.INC) | inc | 263 | not_translated |
| [v4.0/src/INC/PRINTF.ASM](../reference/v4.0/src/INC/PRINTF.ASM) | asm | 415 | not_translated |
| [v4.0/src/INC/PSDATA.INC](../reference/v4.0/src/INC/PSDATA.INC) | inc | 505 | not_translated |
| [v4.0/src/INC/REDIRSYM.INC](../reference/v4.0/src/INC/REDIRSYM.INC) | inc | 175 | not_translated |
| [v4.0/src/INC/SF.INC](../reference/v4.0/src/INC/SF.INC) | inc | 169 | not_translated |
| [v4.0/src/INC/SHELLRD.INC](../reference/v4.0/src/INC/SHELLRD.INC) | inc | 5112 | not_translated |
| [v4.0/src/INC/SMDOSSYM.INC](../reference/v4.0/src/INC/SMDOSSYM.INC) | inc | 157 | not_translated |
| [v4.0/src/INC/SMIFSSYM.INC](../reference/v4.0/src/INC/SMIFSSYM.INC) | inc | 63 | not_translated |
| [v4.0/src/INC/STRING.C](../reference/v4.0/src/INC/STRING.C) | c | 95 | not_translated |
| [v4.0/src/INC/STRUC.INC](../reference/v4.0/src/INC/STRUC.INC) | inc | 703 | not_translated |
| [v4.0/src/INC/SYSCALL.INC](../reference/v4.0/src/INC/SYSCALL.INC) | inc | 166 | not_translated |
| [v4.0/src/INC/SYSMSG.INC](../reference/v4.0/src/INC/SYSMSG.INC) | inc | 444 | not_translated |
| [v4.0/src/INC/SYSVAR.C](../reference/v4.0/src/INC/SYSVAR.C) | c | 49 | not_translated |
| [v4.0/src/INC/SYSVAR.INC](../reference/v4.0/src/INC/SYSVAR.INC) | inc | 77 | not_translated |
| [v4.0/src/INC/VECTOR.INC](../reference/v4.0/src/INC/VECTOR.INC) | inc | 73 | not_translated |
| [v4.0/src/INC/VERSION.INC](../reference/v4.0/src/INC/VERSION.INC) | inc | 107 | not_translated |
| [v4.0/src/INC/VERSIONA.INC](../reference/v4.0/src/INC/VERSIONA.INC) | inc | 19 | not_translated |
| [v4.0/src/INC/YESNO.ASM](../reference/v4.0/src/INC/YESNO.ASM) | asm | 4 | not_translated |
| [v4.0/src/INC/_PARSE.ASM](../reference/v4.0/src/INC/_PARSE.ASM) | asm | 117 | not_translated |
| [v4.0/src/MAPPER/BEEP.ASM](../reference/v4.0/src/MAPPER/BEEP.ASM) | asm | 98 | not_translated |
| [v4.0/src/MAPPER/CHARIN.ASM](../reference/v4.0/src/MAPPER/CHARIN.ASM) | asm | 113 | not_translated |
| [v4.0/src/MAPPER/CWAIT.ASM](../reference/v4.0/src/MAPPER/CWAIT.ASM) | asm | 91 | not_translated |
| [v4.0/src/MAPPER/D_GCTRCD.ASM](../reference/v4.0/src/MAPPER/D_GCTRCD.ASM) | asm | 47 | not_translated |
| [v4.0/src/MAPPER/ERROR.ASM](../reference/v4.0/src/MAPPER/ERROR.ASM) | asm | 150 | not_translated |
| [v4.0/src/MAPPER/ERROR.INC](../reference/v4.0/src/MAPPER/ERROR.INC) | inc | 285 | not_translated |
| [v4.0/src/MAPPER/EXECPGM.ASM](../reference/v4.0/src/MAPPER/EXECPGM.ASM) | asm | 118 | not_translated |
| [v4.0/src/MAPPER/FIND.INC](../reference/v4.0/src/MAPPER/FIND.INC) | inc | 37 | not_translated |
| [v4.0/src/MAPPER/FLUSHBUF.ASM](../reference/v4.0/src/MAPPER/FLUSHBUF.ASM) | asm | 58 | not_translated |
| [v4.0/src/MAPPER/F_CLOSE.ASM](../reference/v4.0/src/MAPPER/F_CLOSE.ASM) | asm | 105 | not_translated |
| [v4.0/src/MAPPER/F_FIRST.ASM](../reference/v4.0/src/MAPPER/F_FIRST.ASM) | asm | 355 | not_translated |
| [v4.0/src/MAPPER/F_NEXT.ASM](../reference/v4.0/src/MAPPER/F_NEXT.ASM) | asm | 272 | not_translated |
| [v4.0/src/MAPPER/GETCNTRY.ASM](../reference/v4.0/src/MAPPER/GETCNTRY.ASM) | asm | 136 | not_translated |
| [v4.0/src/MAPPER/GETMSG.ASM](../reference/v4.0/src/MAPPER/GETMSG.ASM) | asm | 587 | not_translated |
| [v4.0/src/MAPPER/GMACHMOD.ASM](../reference/v4.0/src/MAPPER/GMACHMOD.ASM) | asm | 41 | not_translated |
| [v4.0/src/MAPPER/KBD.INC](../reference/v4.0/src/MAPPER/KBD.INC) | inc | 35 | not_translated |
| [v4.0/src/MAPPER/KBDGSTAT.ASM](../reference/v4.0/src/MAPPER/KBDGSTAT.ASM) | asm | 77 | not_translated |
| [v4.0/src/MAPPER/KBDSSTAT.ASM](../reference/v4.0/src/MAPPER/KBDSSTAT.ASM) | asm | 130 | not_translated |
| [v4.0/src/MAPPER/MACROS.INC](../reference/v4.0/src/MAPPER/MACROS.INC) | inc | 46 | expanded_macro_dependency |
| [v4.0/src/MAPPER/MAPPER.INC](../reference/v4.0/src/MAPPER/MAPPER.INC) | inc | 23 | not_translated |
| [v4.0/src/MAPPER/MESSAGES.INC](../reference/v4.0/src/MAPPER/MESSAGES.INC) | inc | 89 | not_translated |
| [v4.0/src/MAPPER/MSC.INC](../reference/v4.0/src/MAPPER/MSC.INC) | inc | 16 | not_translated |
| [v4.0/src/MAPPER/NLSAPI.INC](../reference/v4.0/src/MAPPER/NLSAPI.INC) | inc | 18 | not_translated |
| [v4.0/src/MAPPER/OLDGETCN.ASM](../reference/v4.0/src/MAPPER/OLDGETCN.ASM) | asm | 133 | not_translated |
| [v4.0/src/MAPPER/OPEN.ASM](../reference/v4.0/src/MAPPER/OPEN.ASM) | asm | 290 | not_translated |
| [v4.0/src/MAPPER/QCURDIR.ASM](../reference/v4.0/src/MAPPER/QCURDIR.ASM) | asm | 105 | not_translated |
| [v4.0/src/MAPPER/QCURDSK.ASM](../reference/v4.0/src/MAPPER/QCURDSK.ASM) | asm | 149 | not_translated |
| [v4.0/src/MAPPER/QFILEINF.ASM](../reference/v4.0/src/MAPPER/QFILEINF.ASM) | asm | 147 | not_translated |
| [v4.0/src/MAPPER/QFSINFO.ASM](../reference/v4.0/src/MAPPER/QFSINFO.ASM) | asm | 137 | not_translated |
| [v4.0/src/MAPPER/SCNTRY.ASM](../reference/v4.0/src/MAPPER/SCNTRY.ASM) | asm | 58 | not_translated |
| [v4.0/src/MAPPER/SETFSINF.ASM](../reference/v4.0/src/MAPPER/SETFSINF.ASM) | asm | 106 | not_translated |
| [v4.0/src/MAPPER/SETINT24.ASM](../reference/v4.0/src/MAPPER/SETINT24.ASM) | asm | 94 | not_translated |
| [v4.0/src/MAPPER/SIGHAND.ASM](../reference/v4.0/src/MAPPER/SIGHAND.ASM) | asm | 205 | not_translated |
| [v4.0/src/MEMM/EMM/EMM.H](../reference/v4.0/src/MEMM/EMM/EMM.H) | h | 224 | not_translated |
| [v4.0/src/MEMM/EMM/EMM40.C](../reference/v4.0/src/MEMM/EMM/EMM40.C) | c | 569 | not_translated |
| [v4.0/src/MEMM/EMM/EMMDATA.ASM](../reference/v4.0/src/MEMM/EMM/EMMDATA.ASM) | asm | 466 | not_translated |
| [v4.0/src/MEMM/EMM/EMMDEF.INC](../reference/v4.0/src/MEMM/EMM/EMMDEF.INC) | inc | 330 | not_translated |
| [v4.0/src/MEMM/EMM/EMMDISP.ASM](../reference/v4.0/src/MEMM/EMM/EMMDISP.ASM) | asm | 234 | not_translated |
| [v4.0/src/MEMM/EMM/EMMFUNCT.C](../reference/v4.0/src/MEMM/EMM/EMMFUNCT.C) | c | 660 | not_translated |
| [v4.0/src/MEMM/EMM/EMMINC.ASM](../reference/v4.0/src/MEMM/EMM/EMMINC.ASM) | asm | 56 | not_translated |
| [v4.0/src/MEMM/EMM/EMMP.ASM](../reference/v4.0/src/MEMM/EMM/EMMP.ASM) | asm | 2976 | not_translated |
| [v4.0/src/MEMM/EMM/EMMSUP.ASM](../reference/v4.0/src/MEMM/EMM/EMMSUP.ASM) | asm | 652 | not_translated |
| [v4.0/src/MEMM/MEMM/A20TRAP.ASM](../reference/v4.0/src/MEMM/MEMM/A20TRAP.ASM) | asm | 528 | not_translated |
| [v4.0/src/MEMM/MEMM/ALLOCMEM.ASM](../reference/v4.0/src/MEMM/MEMM/ALLOCMEM.ASM) | asm | 565 | not_translated |
| [v4.0/src/MEMM/MEMM/DESC.INC](../reference/v4.0/src/MEMM/MEMM/DESC.INC) | inc | 277 | not_translated |
| [v4.0/src/MEMM/MEMM/DISP.INC](../reference/v4.0/src/MEMM/MEMM/DISP.INC) | inc | 50 | not_translated |
| [v4.0/src/MEMM/MEMM/DMA.INC](../reference/v4.0/src/MEMM/MEMM/DMA.INC) | inc | 234 | not_translated |
| [v4.0/src/MEMM/MEMM/EKBD.ASM](../reference/v4.0/src/MEMM/MEMM/EKBD.ASM) | asm | 140 | not_translated |
| [v4.0/src/MEMM/MEMM/ELIM.INC](../reference/v4.0/src/MEMM/MEMM/ELIM.INC) | inc | 128 | not_translated |
| [v4.0/src/MEMM/MEMM/ELIMFUNC.ASM](../reference/v4.0/src/MEMM/MEMM/ELIMFUNC.ASM) | asm | 268 | not_translated |
| [v4.0/src/MEMM/MEMM/ELIMTRAP.ASM](../reference/v4.0/src/MEMM/MEMM/ELIMTRAP.ASM) | asm | 1560 | not_translated |
| [v4.0/src/MEMM/MEMM/EM286LL.ASM](../reference/v4.0/src/MEMM/MEMM/EM286LL.ASM) | asm | 361 | not_translated |
| [v4.0/src/MEMM/MEMM/EM386LL.ASM](../reference/v4.0/src/MEMM/MEMM/EM386LL.ASM) | asm | 398 | not_translated |
| [v4.0/src/MEMM/MEMM/EMM.ASM](../reference/v4.0/src/MEMM/MEMM/EMM.ASM) | asm | 522 | not_translated |
| [v4.0/src/MEMM/MEMM/EMM386.INC](../reference/v4.0/src/MEMM/MEMM/EMM386.INC) | inc | 50 | not_translated |
| [v4.0/src/MEMM/MEMM/EMMINIT.ASM](../reference/v4.0/src/MEMM/MEMM/EMMINIT.ASM) | asm | 679 | not_translated |
| [v4.0/src/MEMM/MEMM/EMMMES.ASM](../reference/v4.0/src/MEMM/MEMM/EMMMES.ASM) | asm | 209 | not_translated |
| [v4.0/src/MEMM/MEMM/ERRHNDLR.ASM](../reference/v4.0/src/MEMM/MEMM/ERRHNDLR.ASM) | asm | 385 | not_translated |
| [v4.0/src/MEMM/MEMM/EXTPOOL.ASM](../reference/v4.0/src/MEMM/MEMM/EXTPOOL.ASM) | asm | 188 | not_translated |
| [v4.0/src/MEMM/MEMM/I286.ASM](../reference/v4.0/src/MEMM/MEMM/I286.ASM) | asm | 313 | not_translated |
| [v4.0/src/MEMM/MEMM/I386.ASM](../reference/v4.0/src/MEMM/MEMM/I386.ASM) | asm | 241 | not_translated |
| [v4.0/src/MEMM/MEMM/INIT.ASM](../reference/v4.0/src/MEMM/MEMM/INIT.ASM) | asm | 1121 | not_translated |
| [v4.0/src/MEMM/MEMM/INITDEB.ASM](../reference/v4.0/src/MEMM/MEMM/INITDEB.ASM) | asm | 181 | not_translated |
| [v4.0/src/MEMM/MEMM/INITEPG.ASM](../reference/v4.0/src/MEMM/MEMM/INITEPG.ASM) | asm | 378 | not_translated |
| [v4.0/src/MEMM/MEMM/INITTAB.ASM](../reference/v4.0/src/MEMM/MEMM/INITTAB.ASM) | asm | 633 | not_translated |
| [v4.0/src/MEMM/MEMM/INSTR386.INC](../reference/v4.0/src/MEMM/MEMM/INSTR386.INC) | inc | 564 | not_translated |
| [v4.0/src/MEMM/MEMM/IOTRAP.ASM](../reference/v4.0/src/MEMM/MEMM/IOTRAP.ASM) | asm | 165 | not_translated |
| [v4.0/src/MEMM/MEMM/KBD.ASM](../reference/v4.0/src/MEMM/MEMM/KBD.ASM) | asm | 1608 | not_translated |
| [v4.0/src/MEMM/MEMM/KBD.INC](../reference/v4.0/src/MEMM/MEMM/KBD.INC) | inc | 100 | not_translated |
| [v4.0/src/MEMM/MEMM/LOADALL.INC](../reference/v4.0/src/MEMM/MEMM/LOADALL.INC) | inc | 174 | not_translated |
| [v4.0/src/MEMM/MEMM/MACH_ID.INC](../reference/v4.0/src/MEMM/MEMM/MACH_ID.INC) | inc | 39 | not_translated |
| [v4.0/src/MEMM/MEMM/MAPDMA.ASM](../reference/v4.0/src/MEMM/MEMM/MAPDMA.ASM) | asm | 633 | not_translated |
| [v4.0/src/MEMM/MEMM/MAPDMA.C](../reference/v4.0/src/MEMM/MEMM/MAPDMA.C) | c | 425 | not_translated |
| [v4.0/src/MEMM/MEMM/MAPLIN.ASM](../reference/v4.0/src/MEMM/MEMM/MAPLIN.ASM) | asm | 172 | not_translated |
| [v4.0/src/MEMM/MEMM/MEMM386.ASM](../reference/v4.0/src/MEMM/MEMM/MEMM386.ASM) | asm | 561 | not_translated |
| [v4.0/src/MEMM/MEMM/MEMMCOM.ASM](../reference/v4.0/src/MEMM/MEMM/MEMMCOM.ASM) | asm | 243 | not_translated |
| [v4.0/src/MEMM/MEMM/MEMMINC.ASM](../reference/v4.0/src/MEMM/MEMM/MEMMINC.ASM) | asm | 81 | not_translated |
| [v4.0/src/MEMM/MEMM/MEMMONF.ASM](../reference/v4.0/src/MEMM/MEMM/MEMMONF.ASM) | asm | 338 | not_translated |
| [v4.0/src/MEMM/MEMM/MEMM_MSG.INC](../reference/v4.0/src/MEMM/MEMM/MEMM_MSG.INC) | inc | 44 | not_translated |
| [v4.0/src/MEMM/MEMM/MOVEB.ASM](../reference/v4.0/src/MEMM/MEMM/MOVEB.ASM) | asm | 534 | not_translated |
| [v4.0/src/MEMM/MEMM/M_STATE.ASM](../reference/v4.0/src/MEMM/MEMM/M_STATE.ASM) | asm | 114 | not_translated |
| [v4.0/src/MEMM/MEMM/OEMDEP.INC](../reference/v4.0/src/MEMM/MEMM/OEMDEP.INC) | inc | 112 | not_translated |
| [v4.0/src/MEMM/MEMM/OEMPROC.ASM](../reference/v4.0/src/MEMM/MEMM/OEMPROC.ASM) | asm | 1120 | not_translated |
| [v4.0/src/MEMM/MEMM/PAGE.INC](../reference/v4.0/src/MEMM/MEMM/PAGE.INC) | inc | 53 | not_translated |
| [v4.0/src/MEMM/MEMM/PPAGE.ASM](../reference/v4.0/src/MEMM/MEMM/PPAGE.ASM) | asm | 510 | not_translated |
| [v4.0/src/MEMM/MEMM/PRINT.ASM](../reference/v4.0/src/MEMM/MEMM/PRINT.ASM) | asm | 424 | not_translated |
| [v4.0/src/MEMM/MEMM/RETREAL.ASM](../reference/v4.0/src/MEMM/MEMM/RETREAL.ASM) | asm | 228 | not_translated |
| [v4.0/src/MEMM/MEMM/ROM_SRCH.ASM](../reference/v4.0/src/MEMM/MEMM/ROM_SRCH.ASM) | asm | 318 | not_translated |
| [v4.0/src/MEMM/MEMM/RRTRAP.ASM](../reference/v4.0/src/MEMM/MEMM/RRTRAP.ASM) | asm | 437 | not_translated |
| [v4.0/src/MEMM/MEMM/SHIPHI.ASM](../reference/v4.0/src/MEMM/MEMM/SHIPHI.ASM) | asm | 326 | not_translated |
| [v4.0/src/MEMM/MEMM/TABDEF.ASM](../reference/v4.0/src/MEMM/MEMM/TABDEF.ASM) | asm | 357 | not_translated |
| [v4.0/src/MEMM/MEMM/TRAPDEF.ASM](../reference/v4.0/src/MEMM/MEMM/TRAPDEF.ASM) | asm | 469 | not_translated |
| [v4.0/src/MEMM/MEMM/UTIL.ASM](../reference/v4.0/src/MEMM/MEMM/UTIL.ASM) | asm | 336 | not_translated |
| [v4.0/src/MEMM/MEMM/VDMINIT.ASM](../reference/v4.0/src/MEMM/MEMM/VDMINIT.ASM) | asm | 526 | not_translated |
| [v4.0/src/MEMM/MEMM/VDMSEG.INC](../reference/v4.0/src/MEMM/MEMM/VDMSEG.INC) | inc | 168 | not_translated |
| [v4.0/src/MEMM/MEMM/VDMSEL.INC](../reference/v4.0/src/MEMM/MEMM/VDMSEL.INC) | inc | 101 | not_translated |
| [v4.0/src/MEMM/MEMM/VM386.INC](../reference/v4.0/src/MEMM/MEMM/VM386.INC) | inc | 207 | not_translated |
| [v4.0/src/MEMM/MEMM/VMINIT.ASM](../reference/v4.0/src/MEMM/MEMM/VMINIT.ASM) | asm | 293 | not_translated |
| [v4.0/src/MEMM/MEMM/VMINST.ASM](../reference/v4.0/src/MEMM/MEMM/VMINST.ASM) | asm | 2135 | not_translated |
| [v4.0/src/MEMM/MEMM/VMTRAP.ASM](../reference/v4.0/src/MEMM/MEMM/VMTRAP.ASM) | asm | 791 | not_translated |
| [v4.0/src/SELECT/ASM2C.ASM](../reference/v4.0/src/SELECT/ASM2C.ASM) | asm | 192 | not_translated |
| [v4.0/src/SELECT/BRIDGE.ASM](../reference/v4.0/src/SELECT/BRIDGE.ASM) | asm | 26 | not_translated |
| [v4.0/src/SELECT/CASERVIC.ASM](../reference/v4.0/src/SELECT/CASERVIC.ASM) | asm | 2645 | not_translated |
| [v4.0/src/SELECT/CASEXTRN.INC](../reference/v4.0/src/SELECT/CASEXTRN.INC) | inc | 39 | not_translated |
| [v4.0/src/SELECT/CASTRUC.INC](../reference/v4.0/src/SELECT/CASTRUC.INC) | inc | 240 | not_translated |
| [v4.0/src/SELECT/CKDISP.ASM](../reference/v4.0/src/SELECT/CKDISP.ASM) | asm | 152 | not_translated |
| [v4.0/src/SELECT/COLORS.ASM](../reference/v4.0/src/SELECT/COLORS.ASM) | asm | 84 | not_translated |
| [v4.0/src/SELECT/DOSFILES.INC](../reference/v4.0/src/SELECT/DOSFILES.INC) | inc | 21 | not_translated |
| [v4.0/src/SELECT/EXT.INC](../reference/v4.0/src/SELECT/EXT.INC) | inc | 577 | not_translated |
| [v4.0/src/SELECT/EXTERN.H](../reference/v4.0/src/SELECT/EXTERN.H) | h | 76 | not_translated |
| [v4.0/src/SELECT/GEN_COMS.ASM](../reference/v4.0/src/SELECT/GEN_COMS.ASM) | asm | 864 | not_translated |
| [v4.0/src/SELECT/GET_HELP.ASM](../reference/v4.0/src/SELECT/GET_HELP.ASM) | asm | 234 | not_translated |
| [v4.0/src/SELECT/GET_STAT.C](../reference/v4.0/src/SELECT/GET_STAT.C) | c | 1217 | not_translated |
| [v4.0/src/SELECT/GET_STAT.H](../reference/v4.0/src/SELECT/GET_STAT.H) | h | 192 | not_translated |
| [v4.0/src/SELECT/GLOBAL.C](../reference/v4.0/src/SELECT/GLOBAL.C) | c | 79 | not_translated |
| [v4.0/src/SELECT/INITMEM.ASM](../reference/v4.0/src/SELECT/INITMEM.ASM) | asm | 395 | not_translated |
| [v4.0/src/SELECT/INPUT.ASM](../reference/v4.0/src/SELECT/INPUT.ASM) | asm | 1738 | not_translated |
| [v4.0/src/SELECT/INT13.C](../reference/v4.0/src/SELECT/INT13.C) | c | 194 | not_translated |
| [v4.0/src/SELECT/INTVEC.ASM](../reference/v4.0/src/SELECT/INTVEC.ASM) | asm | 547 | not_translated |
| [v4.0/src/SELECT/MACROS.INC](../reference/v4.0/src/SELECT/MACROS.INC) | inc | 541 | not_translated |
| [v4.0/src/SELECT/MACROS2.INC](../reference/v4.0/src/SELECT/MACROS2.INC) | inc | 438 | not_translated |
| [v4.0/src/SELECT/MACROS3.INC](../reference/v4.0/src/SELECT/MACROS3.INC) | inc | 382 | not_translated |
| [v4.0/src/SELECT/MACROS4.INC](../reference/v4.0/src/SELECT/MACROS4.INC) | inc | 346 | not_translated |
| [v4.0/src/SELECT/MACROS5.INC](../reference/v4.0/src/SELECT/MACROS5.INC) | inc | 539 | not_translated |
| [v4.0/src/SELECT/MACROS6.INC](../reference/v4.0/src/SELECT/MACROS6.INC) | inc | 720 | not_translated |
| [v4.0/src/SELECT/MACROS7.INC](../reference/v4.0/src/SELECT/MACROS7.INC) | inc | 126 | not_translated |
| [v4.0/src/SELECT/MACROS8.INC](../reference/v4.0/src/SELECT/MACROS8.INC) | inc | 48 | not_translated |
| [v4.0/src/SELECT/MAC_EQU.INC](../reference/v4.0/src/SELECT/MAC_EQU.INC) | inc | 88 | not_translated |
| [v4.0/src/SELECT/MOD_COPY.ASM](../reference/v4.0/src/SELECT/MOD_COPY.ASM) | asm | 2485 | not_translated |
| [v4.0/src/SELECT/MPARSE.ASM](../reference/v4.0/src/SELECT/MPARSE.ASM) | asm | 39 | not_translated |
| [v4.0/src/SELECT/PAN-LIST.INC](../reference/v4.0/src/SELECT/PAN-LIST.INC) | inc | 192 | not_translated |
| [v4.0/src/SELECT/PANELS.ASM](../reference/v4.0/src/SELECT/PANELS.ASM) | asm | 4374 | not_translated |
| [v4.0/src/SELECT/PCEQUATE.INC](../reference/v4.0/src/SELECT/PCEQUATE.INC) | inc | 452 | not_translated |
| [v4.0/src/SELECT/PCGEQU.INC](../reference/v4.0/src/SELECT/PCGEQU.INC) | inc | 170 | not_translated |
| [v4.0/src/SELECT/PRN_DEF.ASM](../reference/v4.0/src/SELECT/PRN_DEF.ASM) | asm | 1557 | not_translated |
| [v4.0/src/SELECT/ROUTINE2.ASM](../reference/v4.0/src/SELECT/ROUTINE2.ASM) | asm | 1370 | not_translated |
| [v4.0/src/SELECT/ROUTINES.ASM](../reference/v4.0/src/SELECT/ROUTINES.ASM) | asm | 1627 | not_translated |
| [v4.0/src/SELECT/ROUT_EXT.INC](../reference/v4.0/src/SELECT/ROUT_EXT.INC) | inc | 75 | not_translated |
| [v4.0/src/SELECT/SCN_PARM.ASM](../reference/v4.0/src/SELECT/SCN_PARM.ASM) | asm | 300 | not_translated |
| [v4.0/src/SELECT/SCROLL.ASM](../reference/v4.0/src/SELECT/SCROLL.ASM) | asm | 3356 | not_translated |
| [v4.0/src/SELECT/SEL-PAN.ASM](../reference/v4.0/src/SELECT/SEL-PAN.ASM) | asm | 49 | not_translated |
| [v4.0/src/SELECT/SEL-PAN.INC](../reference/v4.0/src/SELECT/SEL-PAN.INC) | inc | 307 | not_translated |
| [v4.0/src/SELECT/SELCHILD.ASM](../reference/v4.0/src/SELECT/SELCHILD.ASM) | asm | 63 | not_translated |
| [v4.0/src/SELECT/SELECT.INC](../reference/v4.0/src/SELECT/SELECT.INC) | inc | 55 | not_translated |
| [v4.0/src/SELECT/SELECT0.ASM](../reference/v4.0/src/SELECT/SELECT0.ASM) | asm | 373 | not_translated |
| [v4.0/src/SELECT/SELECT1.ASM](../reference/v4.0/src/SELECT/SELECT1.ASM) | asm | 448 | not_translated |
| [v4.0/src/SELECT/SELECT2.ASM](../reference/v4.0/src/SELECT/SELECT2.ASM) | asm | 590 | not_translated |
| [v4.0/src/SELECT/SELECT2A.ASM](../reference/v4.0/src/SELECT/SELECT2A.ASM) | asm | 382 | not_translated |
| [v4.0/src/SELECT/SELECT3.ASM](../reference/v4.0/src/SELECT/SELECT3.ASM) | asm | 287 | not_translated |
| [v4.0/src/SELECT/SELECT4.ASM](../reference/v4.0/src/SELECT/SELECT4.ASM) | asm | 439 | not_translated |
| [v4.0/src/SELECT/SELECT5.ASM](../reference/v4.0/src/SELECT/SELECT5.ASM) | asm | 576 | not_translated |
| [v4.0/src/SELECT/SELECT5A.ASM](../reference/v4.0/src/SELECT/SELECT5A.ASM) | asm | 187 | not_translated |
| [v4.0/src/SELECT/SELECT6.ASM](../reference/v4.0/src/SELECT/SELECT6.ASM) | asm | 562 | not_translated |
| [v4.0/src/SELECT/SELECT7.ASM](../reference/v4.0/src/SELECT/SELECT7.ASM) | asm | 464 | not_translated |
| [v4.0/src/SELECT/SELECT8.ASM](../reference/v4.0/src/SELECT/SELECT8.ASM) | asm | 453 | not_translated |
| [v4.0/src/SELECT/SELECT9.ASM](../reference/v4.0/src/SELECT/SELECT9.ASM) | asm | 518 | not_translated |
| [v4.0/src/SELECT/SELQUIT.ASM](../reference/v4.0/src/SELECT/SELQUIT.ASM) | asm | 314 | not_translated |
| [v4.0/src/SELECT/SELSERV.ASM](../reference/v4.0/src/SELECT/SELSERV.ASM) | asm | 109 | not_translated |
| [v4.0/src/SELECT/SEL_FILE.INC](../reference/v4.0/src/SELECT/SEL_FILE.INC) | inc | 578 | not_translated |
| [v4.0/src/SELECT/SERVICES.ASM](../reference/v4.0/src/SELECT/SERVICES.ASM) | asm | 900 | not_translated |
| [v4.0/src/SELECT/SSTUB.ASM](../reference/v4.0/src/SELECT/SSTUB.ASM) | asm | 210 | not_translated |
| [v4.0/src/SELECT/S_DISPLY.ASM](../reference/v4.0/src/SELECT/S_DISPLY.ASM) | asm | 113 | not_translated |
| [v4.0/src/SELECT/VAR.ASM](../reference/v4.0/src/SELECT/VAR.ASM) | asm | 947 | not_translated |
| [v4.0/src/SELECT/VARSTRUC.INC](../reference/v4.0/src/SELECT/VARSTRUC.INC) | inc | 66 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/ASSERT.H](../reference/v4.0/src/TOOLS/BLD/INC/ASSERT.H) | h | 36 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/BIOS.H](../reference/v4.0/src/TOOLS/BLD/INC/BIOS.H) | h | 182 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/CONIO.H](../reference/v4.0/src/TOOLS/BLD/INC/CONIO.H) | h | 35 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/CTYPE.H](../reference/v4.0/src/TOOLS/BLD/INC/CTYPE.H) | h | 67 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/DIRECT.H](../reference/v4.0/src/TOOLS/BLD/INC/DIRECT.H) | h | 25 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/DOS.H](../reference/v4.0/src/TOOLS/BLD/INC/DOS.H) | h | 221 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/ERRNO.H](../reference/v4.0/src/TOOLS/BLD/INC/ERRNO.H) | h | 51 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/FCNTL.H](../reference/v4.0/src/TOOLS/BLD/INC/FCNTL.H) | h | 36 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/FLOAT.H](../reference/v4.0/src/TOOLS/BLD/INC/FLOAT.H) | h | 138 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/GRAPH.H](../reference/v4.0/src/TOOLS/BLD/INC/GRAPH.H) | h | 217 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/IO.H](../reference/v4.0/src/TOOLS/BLD/INC/IO.H) | h | 44 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/LIMITS.H](../reference/v4.0/src/TOOLS/BLD/INC/LIMITS.H) | h | 33 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/MALLOC.H](../reference/v4.0/src/TOOLS/BLD/INC/MALLOC.H) | h | 125 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/MATH.H](../reference/v4.0/src/TOOLS/BLD/INC/MATH.H) | h | 113 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/MEMORY.H](../reference/v4.0/src/TOOLS/BLD/INC/MEMORY.H) | h | 35 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/PROCESS.H](../reference/v4.0/src/TOOLS/BLD/INC/PROCESS.H) | h | 66 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/SEARCH.H](../reference/v4.0/src/TOOLS/BLD/INC/SEARCH.H) | h | 32 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/SETJMP.H](../reference/v4.0/src/TOOLS/BLD/INC/SETJMP.H) | h | 36 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/SHARE.H](../reference/v4.0/src/TOOLS/BLD/INC/SHARE.H) | h | 16 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/SIGNAL.H](../reference/v4.0/src/TOOLS/BLD/INC/SIGNAL.H) | h | 60 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/STDARG.H](../reference/v4.0/src/TOOLS/BLD/INC/STDARG.H) | h | 21 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/STDDEF.H](../reference/v4.0/src/TOOLS/BLD/INC/STDDEF.H) | h | 49 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/STDIO.H](../reference/v4.0/src/TOOLS/BLD/INC/STDIO.H) | h | 166 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/STDLIB.H](../reference/v4.0/src/TOOLS/BLD/INC/STDLIB.H) | h | 152 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/STRING.H](../reference/v4.0/src/TOOLS/BLD/INC/STRING.H) | h | 62 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/SYS/LOCKING.H](../reference/v4.0/src/TOOLS/BLD/INC/SYS/LOCKING.H) | h | 17 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/SYS/STAT.H](../reference/v4.0/src/TOOLS/BLD/INC/SYS/STAT.H) | h | 57 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/SYS/TIMEB.H](../reference/v4.0/src/TOOLS/BLD/INC/SYS/TIMEB.H) | h | 40 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/SYS/TYPES.H](../reference/v4.0/src/TOOLS/BLD/INC/SYS/TYPES.H) | h | 32 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/SYS/UTIME.H](../reference/v4.0/src/TOOLS/BLD/INC/SYS/UTIME.H) | h | 41 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/TIME.H](../reference/v4.0/src/TOOLS/BLD/INC/TIME.H) | h | 76 | not_translated |
| [v4.0/src/TOOLS/BLD/INC/VARARGS.H](../reference/v4.0/src/TOOLS/BLD/INC/VARARGS.H) | h | 22 | not_translated |

## Additional source inputs

The older inventory only counted ASM/INC/C/H. These macro, declaration, assembly configuration and message inputs must also be handled when porting their owning modules. In particular, SYSMAC.LIB contains source macros rather than binary objects, and USA.INF is assembled through PANEL.INF.

| Original source input | Kind | Status |
| --- | --- | --- |
| [v4.0/src/BIOS/MSBIO.SKL](../reference/v4.0/src/BIOS/MSBIO.SKL) | skl | reference_only |
| [v4.0/src/BOOT/BOOT.SKL](../reference/v4.0/src/BOOT/BOOT.SKL) | skl | reference_only |
| [v4.0/src/CMD/APPEND/APPEND.SKL](../reference/v4.0/src/CMD/APPEND/APPEND.SKL) | skl | reference_only |
| [v4.0/src/CMD/APPEND/SYSMAC.LIB](../reference/v4.0/src/CMD/APPEND/SYSMAC.LIB) | lib | reference_only |
| [v4.0/src/CMD/ASSIGN/ASSIGN.SKL](../reference/v4.0/src/CMD/ASSIGN/ASSIGN.SKL) | skl | reference_only |
| [v4.0/src/CMD/ATTRIB/ATTRIB.SKL](../reference/v4.0/src/CMD/ATTRIB/ATTRIB.SKL) | skl | reference_only |
| [v4.0/src/CMD/BACKUP/BACKUP.SKL](../reference/v4.0/src/CMD/BACKUP/BACKUP.SKL) | skl | reference_only |
| [v4.0/src/CMD/CHKDSK/CHKDSK.SKL](../reference/v4.0/src/CMD/CHKDSK/CHKDSK.SKL) | skl | reference_only |
| [v4.0/src/CMD/COMMAND/COMMAND.SKL](../reference/v4.0/src/CMD/COMMAND/COMMAND.SKL) | skl | reference_only |
| [v4.0/src/CMD/COMMAND/RESMSG.EQU](../reference/v4.0/src/CMD/COMMAND/RESMSG.EQU) | equ | reference_only |
| [v4.0/src/CMD/COMP/COMP.SKL](../reference/v4.0/src/CMD/COMP/COMP.SKL) | skl | reference_only |
| [v4.0/src/CMD/DEBUG/DEBUG.SKL](../reference/v4.0/src/CMD/DEBUG/DEBUG.SKL) | skl | reference_only |
| [v4.0/src/CMD/DEBUG/SYSVER.FAL](../reference/v4.0/src/CMD/DEBUG/SYSVER.FAL) | fal | reference_only |
| [v4.0/src/CMD/DEBUG/SYSVER.TRU](../reference/v4.0/src/CMD/DEBUG/SYSVER.TRU) | tru | reference_only |
| [v4.0/src/CMD/DISKCOMP/DISKCOMP.EQU](../reference/v4.0/src/CMD/DISKCOMP/DISKCOMP.EQU) | equ | reference_only |
| [v4.0/src/CMD/DISKCOMP/DISKCOMP.SKL](../reference/v4.0/src/CMD/DISKCOMP/DISKCOMP.SKL) | skl | reference_only |
| [v4.0/src/CMD/DISKCOPY/DISKCOPY.EQU](../reference/v4.0/src/CMD/DISKCOPY/DISKCOPY.EQU) | equ | reference_only |
| [v4.0/src/CMD/DISKCOPY/DISKCOPY.SKL](../reference/v4.0/src/CMD/DISKCOPY/DISKCOPY.SKL) | skl | reference_only |
| [v4.0/src/CMD/EDLIN/EDLIN.SKL](../reference/v4.0/src/CMD/EDLIN/EDLIN.SKL) | skl | reference_only |
| [v4.0/src/CMD/EXE2BIN/EXE2BIN.SKL](../reference/v4.0/src/CMD/EXE2BIN/EXE2BIN.SKL) | skl | reference_only |
| [v4.0/src/CMD/FASTOPEN/FASTOPEN.SKL](../reference/v4.0/src/CMD/FASTOPEN/FASTOPEN.SKL) | skl | reference_only |
| [v4.0/src/CMD/FDISK/FDISK.MSG](../reference/v4.0/src/CMD/FDISK/FDISK.MSG) | msg | reference_only |
| [v4.0/src/CMD/FDISK/FDISK.SKL](../reference/v4.0/src/CMD/FDISK/FDISK.SKL) | skl | reference_only |
| [v4.0/src/CMD/FDISK/FDISK5.SKL](../reference/v4.0/src/CMD/FDISK/FDISK5.SKL) | skl | reference_only |
| [v4.0/src/CMD/FILESYS/FILESYS.SKL](../reference/v4.0/src/CMD/FILESYS/FILESYS.SKL) | skl | reference_only |
| [v4.0/src/CMD/FIND/FIND.SKL](../reference/v4.0/src/CMD/FIND/FIND.SKL) | skl | reference_only |
| [v4.0/src/CMD/FORMAT/FORMAT.SKL](../reference/v4.0/src/CMD/FORMAT/FORMAT.SKL) | skl | reference_only |
| [v4.0/src/CMD/GRAFTABL/GRAFTABL.SKL](../reference/v4.0/src/CMD/GRAFTABL/GRAFTABL.SKL) | skl | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRAPHICS.SKL](../reference/v4.0/src/CMD/GRAPHICS/GRAPHICS.SKL) | skl | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRBWPRT.EXT](../reference/v4.0/src/CMD/GRAPHICS/GRBWPRT.EXT) | ext | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRCOLPRT.EXT](../reference/v4.0/src/CMD/GRAPHICS/GRCOLPRT.EXT) | ext | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRCOMMON.EXT](../reference/v4.0/src/CMD/GRAPHICS/GRCOMMON.EXT) | ext | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRCPSD.EXT](../reference/v4.0/src/CMD/GRAPHICS/GRCPSD.EXT) | ext | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRCTRL.EXT](../reference/v4.0/src/CMD/GRAPHICS/GRCTRL.EXT) | ext | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRCTRL.STR](../reference/v4.0/src/CMD/GRAPHICS/GRCTRL.STR) | str | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRINST.EXT](../reference/v4.0/src/CMD/GRAPHICS/GRINST.EXT) | ext | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRINT2FH.EXT](../reference/v4.0/src/CMD/GRAPHICS/GRINT2FH.EXT) | ext | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRLOAD.EXT](../reference/v4.0/src/CMD/GRAPHICS/GRLOAD.EXT) | ext | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRLOAD2.EXT](../reference/v4.0/src/CMD/GRAPHICS/GRLOAD2.EXT) | ext | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRLOAD3.EXT](../reference/v4.0/src/CMD/GRAPHICS/GRLOAD3.EXT) | ext | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRMSG.EQU](../reference/v4.0/src/CMD/GRAPHICS/GRMSG.EQU) | equ | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRPARMS.EXT](../reference/v4.0/src/CMD/GRAPHICS/GRPARMS.EXT) | ext | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRPARSE.EXT](../reference/v4.0/src/CMD/GRAPHICS/GRPARSE.EXT) | ext | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRPATTRN.EXT](../reference/v4.0/src/CMD/GRAPHICS/GRPATTRN.EXT) | ext | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRPATTRN.STR](../reference/v4.0/src/CMD/GRAPHICS/GRPATTRN.STR) | str | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRPRINT.EXT](../reference/v4.0/src/CMD/GRAPHICS/GRPRINT.EXT) | ext | reference_only |
| [v4.0/src/CMD/GRAPHICS/GRSHAR.STR](../reference/v4.0/src/CMD/GRAPHICS/GRSHAR.STR) | str | reference_only |
| [v4.0/src/CMD/IFSFUNC/IFSFUNC.SKL](../reference/v4.0/src/CMD/IFSFUNC/IFSFUNC.SKL) | skl | reference_only |
| [v4.0/src/CMD/JOIN/JOIN.SKL](../reference/v4.0/src/CMD/JOIN/JOIN.SKL) | skl | reference_only |
| [v4.0/src/CMD/KEYB/KEYB.SKL](../reference/v4.0/src/CMD/KEYB/KEYB.SKL) | skl | reference_only |
| [v4.0/src/CMD/LABEL/LABL.SKL](../reference/v4.0/src/CMD/LABEL/LABL.SKL) | skl | reference_only |
| [v4.0/src/CMD/MEM/MEM.SKL](../reference/v4.0/src/CMD/MEM/MEM.SKL) | skl | reference_only |
| [v4.0/src/CMD/MODE/COMMON.STC](../reference/v4.0/src/CMD/MODE/COMMON.STC) | stc | reference_only |
| [v4.0/src/CMD/MODE/MODE.SKL](../reference/v4.0/src/CMD/MODE/MODE.SKL) | skl | reference_only |
| [v4.0/src/CMD/MORE/MORE.SKL](../reference/v4.0/src/CMD/MORE/MORE.SKL) | skl | reference_only |
| [v4.0/src/CMD/NLSFUNC/NLSFUNC.SKL](../reference/v4.0/src/CMD/NLSFUNC/NLSFUNC.SKL) | skl | reference_only |
| [v4.0/src/CMD/PRINT/PRINT.SKL](../reference/v4.0/src/CMD/PRINT/PRINT.SKL) | skl | reference_only |
| [v4.0/src/CMD/RECOVER/RECOVER.SKL](../reference/v4.0/src/CMD/RECOVER/RECOVER.SKL) | skl | reference_only |
| [v4.0/src/CMD/REPLACE/REPLACE.SKL](../reference/v4.0/src/CMD/REPLACE/REPLACE.SKL) | skl | reference_only |
| [v4.0/src/CMD/RESTORE/RESTORE.SKL](../reference/v4.0/src/CMD/RESTORE/RESTORE.SKL) | skl | reference_only |
| [v4.0/src/CMD/SHARE/SHARE.SKL](../reference/v4.0/src/CMD/SHARE/SHARE.SKL) | skl | reference_only |
| [v4.0/src/CMD/SORT/SORT.SKL](../reference/v4.0/src/CMD/SORT/SORT.SKL) | skl | reference_only |
| [v4.0/src/CMD/SUBST/SUBST.SKL](../reference/v4.0/src/CMD/SUBST/SUBST.SKL) | skl | reference_only |
| [v4.0/src/CMD/SYS/SYS.SKL](../reference/v4.0/src/CMD/SYS/SYS.SKL) | skl | reference_only |
| [v4.0/src/CMD/TREE/TREE.SKL](../reference/v4.0/src/CMD/TREE/TREE.SKL) | skl | reference_only |
| [v4.0/src/CMD/XCOPY/DOS.EQU](../reference/v4.0/src/CMD/XCOPY/DOS.EQU) | equ | reference_only |
| [v4.0/src/CMD/XCOPY/XCOPY.EQU](../reference/v4.0/src/CMD/XCOPY/XCOPY.EQU) | equ | reference_only |
| [v4.0/src/CMD/XCOPY/XCOPY.SKL](../reference/v4.0/src/CMD/XCOPY/XCOPY.SKL) | skl | reference_only |
| [v4.0/src/CMD/XCOPY/XINITMSG.EQU](../reference/v4.0/src/CMD/XCOPY/XINITMSG.EQU) | equ | reference_only |
| [v4.0/src/CMD/XCOPY/XMAINMSG.EQU](../reference/v4.0/src/CMD/XCOPY/XMAINMSG.EQU) | equ | reference_only |
| [v4.0/src/DEV/ANSI/ANSI.SKL](../reference/v4.0/src/DEV/ANSI/ANSI.SKL) | skl | reference_only |
| [v4.0/src/DEV/DISPLAY/DISPLAY.SKL](../reference/v4.0/src/DEV/DISPLAY/DISPLAY.SKL) | skl | reference_only |
| [v4.0/src/DEV/DRIVER/DRIVER.SKL](../reference/v4.0/src/DEV/DRIVER/DRIVER.SKL) | skl | reference_only |
| [v4.0/src/DEV/PRINTER/PRINTER.SKL](../reference/v4.0/src/DEV/PRINTER/PRINTER.SKL) | skl | reference_only |
| [v4.0/src/DEV/VDISK/VDISK.SKL](../reference/v4.0/src/DEV/VDISK/VDISK.SKL) | skl | reference_only |
| [v4.0/src/DEV/XMA2EMS/EMS_US.MSG](../reference/v4.0/src/DEV/XMA2EMS/EMS_US.MSG) | msg | reference_only |
| [v4.0/src/DEV/XMA2EMS/XMA2EMS.LC](../reference/v4.0/src/DEV/XMA2EMS/XMA2EMS.LC) | lc | reference_only |
| [v4.0/src/DEV/XMA2EMS/XMA2EMS.SKL](../reference/v4.0/src/DEV/XMA2EMS/XMA2EMS.SKL) | skl | reference_only |
| [v4.0/src/DEV/XMAEM/INDEDES.MAC](../reference/v4.0/src/DEV/XMAEM/INDEDES.MAC) | mac | reference_only |
| [v4.0/src/DEV/XMAEM/INDEINS.MAC](../reference/v4.0/src/DEV/XMAEM/INDEINS.MAC) | mac | reference_only |
| [v4.0/src/DEV/XMAEM/INDEOVP.MAC](../reference/v4.0/src/DEV/XMAEM/INDEOVP.MAC) | mac | reference_only |
| [v4.0/src/DEV/XMAEM/XMAEM.LC](../reference/v4.0/src/DEV/XMAEM/XMAEM.LC) | lc | reference_only |
| [v4.0/src/DEV/XMAEM/XMAEM.SKL](../reference/v4.0/src/DEV/XMAEM/XMAEM.SKL) | skl | reference_only |
| [v4.0/src/DOS/MSDOS.SKL](../reference/v4.0/src/DOS/MSDOS.SKL) | skl | reference_only |
| [v4.0/src/INC/DBCS.OFF](../reference/v4.0/src/INC/DBCS.OFF) | off | reference_only |
| [v4.0/src/INC/DBCS.ON](../reference/v4.0/src/INC/DBCS.ON) | on | reference_only |
| [v4.0/src/INC/DBCS.SW](../reference/v4.0/src/INC/DBCS.SW) | sw | reference_only |
| [v4.0/src/INC/MACRO.DEF](../reference/v4.0/src/INC/MACRO.DEF) | def | reference_only |
| [v4.0/src/INC/SHELLRD.EQU](../reference/v4.0/src/INC/SHELLRD.EQU) | equ | reference_only |
| [v4.0/src/MEMM/MEMM/ASCII_SM.EQU](../reference/v4.0/src/MEMM/MEMM/ASCII_SM.EQU) | equ | reference_only |
| [v4.0/src/MEMM/MEMM/DRIVER.EQU](../reference/v4.0/src/MEMM/MEMM/DRIVER.EQU) | equ | reference_only |
| [v4.0/src/MEMM/MEMM/DRIVER.STR](../reference/v4.0/src/MEMM/MEMM/DRIVER.STR) | str | reference_only |
| [v4.0/src/MEMM/MEMM/PIC_DEF.EQU](../reference/v4.0/src/MEMM/MEMM/PIC_DEF.EQU) | equ | reference_only |
| [v4.0/src/MEMM/MEMM/ROMSTRUC.EQU](../reference/v4.0/src/MEMM/MEMM/ROMSTRUC.EQU) | equ | reference_only |
| [v4.0/src/MEMM/MEMM/ROMXBIOS.EQU](../reference/v4.0/src/MEMM/MEMM/ROMXBIOS.EQU) | equ | reference_only |
| [v4.0/src/MESSAGES/USA-MS.MSG](../reference/v4.0/src/MESSAGES/USA-MS.MSG) | msg | reference_only |
| [v4.0/src/SELECT/DATA.MAC](../reference/v4.0/src/SELECT/DATA.MAC) | mac | reference_only |
| [v4.0/src/SELECT/DOS.EQU](../reference/v4.0/src/SELECT/DOS.EQU) | equ | reference_only |
| [v4.0/src/SELECT/PANEL.MAC](../reference/v4.0/src/SELECT/PANEL.MAC) | mac | reference_only |
| [v4.0/src/SELECT/SELECT.SKL](../reference/v4.0/src/SELECT/SELECT.SKL) | skl | reference_only |
| [v4.0/src/SELECT/USA.INF](../reference/v4.0/src/SELECT/USA.INF) | inf | reference_only |
| [v4.0/src/SELECT/XCOPY.EQU](../reference/v4.0/src/SELECT/XCOPY.EQU) | equ | reference_only |
| [v4.0/src/SELECT/XMAINMSG.EQU](../reference/v4.0/src/SELECT/XMAINMSG.EQU) | equ | reference_only |

## Runtime and help resources

These printer profiles, help sources, code-page data and menu data are separate from executable C++ translation. Preserve their formats or regenerate them for the ported consumers.

| Original resource | Kind | Role |
| --- | --- | --- |
| [v4.0/src/CMD/GRAPHICS/GRAPHICS.PRO](../reference/v4.0/src/CMD/GRAPHICS/GRAPHICS.PRO) | pro | GRAPHICS printer profile interpreted at runtime |
| [v4.0/src/DEV/PRINTER/4201/4201.CPI](../reference/v4.0/src/DEV/PRINTER/4201/4201.CPI) | cpi | Compiled printer code-page data |
| [v4.0/src/DEV/PRINTER/4208/4208.CPI](../reference/v4.0/src/DEV/PRINTER/4208/4208.CPI) | cpi | Compiled printer code-page data |
| [v4.0/src/INC/SHELL.MEU](../reference/v4.0/src/INC/SHELL.MEU) | meu | Compiled DOS Shell menu data |
| [v4.0/src/SELECT/SELECT.PRT](../reference/v4.0/src/SELECT/SELECT.PRT) | prt | SELECT printer profile interpreted at runtime |
| [v4.0/src/SELECT/USA.PRT](../reference/v4.0/src/SELECT/USA.PRT) | prt | SELECT printer profile interpreted at runtime |
| [v4.0/src/SELECT/USA.TXT](../reference/v4.0/src/SELECT/USA.TXT) | txt | SELECT help source converted by ASC2HLP |

The JSON report also accounts for every build/resource file and supplied binary. Binary components without corresponding source require source recovery or a separately scoped reimplementation; they are not counted as completed C++ translations.
