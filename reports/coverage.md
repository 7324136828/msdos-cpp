# Conversion coverage

This is a partial port. Translation counts are not proof of behavioral equivalence.

| Version | Source files | ASM | INC | Lines |
| --- | ---: | ---: | ---: | ---: |
| v1.25 | 7 | 7 | 0 | 13587 |
| v2.0 | 100 | 100 | 0 | 51772 |
| v4.0 | 973 | 584 | 241 | 496622 |
| v4.0-ozzie | 6 | 3 | 3 | 7443 |

| Unit | Source | Instructions | Full source file? |
| --- | --- | ---: | --- |
| command_scan | v1.25/source/COMMAND.ASM:1763 | 13 | no |
| command_numbers | v1.25/source/COMMAND.ASM:1969 | 49 | no |
| hex2bin | v1.25/source/HEX2BIN.ASM:19 | 148 | yes |
| trans | v1.25/source/TRANS.ASM:18 | 776 | yes |
| fat_pack | v1.25/source/MSDOS.ASM:496 | 17 | no |
| fcb_records | v1.25/source/MSDOS.ASM:1454 | 29 | no |
| setvect | v1.25/source/MSDOS.ASM:3343 | 8 | no |
| dsum | v2.0/source/TIME.ASM:218 | 6 | no |
| mapper_getver | v4.0/src/MAPPER/GETVER.ASM:32 | 29 | yes |
| mapper_qverify | v4.0/src/MAPPER/QVERIFY.ASM:37 | 29 | yes |
| mapper_sverify | v4.0/src/MAPPER/SVERIFY.ASM:38 | 31 | yes |
| mapper_mkdir | v4.0/src/MAPPER/MKDIR.ASM:37 | 28 | yes |
| mapper_rmdir | v4.0/src/MAPPER/RMDIR.ASM:37 | 28 | yes |
| mapper_chdir | v4.0/src/MAPPER/CHDIR.ASM:54 | 31 | yes |
| mapper_delete | v4.0/src/MAPPER/DELETE.ASM:52 | 28 | yes |
| mapper_close | v4.0/src/MAPPER/CLOSE.ASM:48 | 31 | yes |
| mapper_read | v4.0/src/MAPPER/READ.ASM:42 | 32 | yes |
| mapper_write | v4.0/src/MAPPER/WRITE.ASM:43 | 32 | yes |
| mapper_lseek | v4.0/src/MAPPER/LSEEK.ASM:43 | 36 | yes |
| mapper_sel_disk | v4.0/src/MAPPER/SEL_DISK.ASM:37 | 28 | yes |
| mapper_qfilemod | v4.0/src/MAPPER/QFILEMOD.ASM:42 | 32 | yes |
| mapper_sfilemod | v4.0/src/MAPPER/SFILEMOD.ASM:42 | 30 | yes |
| mapper_qhandtyp | v4.0/src/MAPPER/QHANDTYP.ASM:51 | 33 | yes |
| mapper_freeseg | v4.0/src/MAPPER/FREESEG.ASM:45 | 30 | yes |
| mapper_allocseg | v4.0/src/MAPPER/ALLOCSEG.ASM:46 | 42 | yes |
| mapper_reallseg | v4.0/src/MAPPER/REALLSEG.ASM:48 | 37 | yes |
| mapper_get_tod | v4.0/src/MAPPER/GET_TOD.ASM:37 | 38 | yes |
| mapper_set_tod | v4.0/src/MAPPER/SET_TOD.ASM:38 | 45 | yes |
| mapper_putmsg | v4.0/src/MAPPER/PUTMSG.ASM:47 | 30 | yes |
| mapper_ioctl | v4.0/src/MAPPER/IOCTL.ASM:45 | 42 | yes |
| mapper_exit | v4.0/src/MAPPER/EXIT.ASM:38 | 30 | yes |
| mapper_scurpos | v4.0/src/MAPPER/SCURPOS.ASM:44 | 55 | yes |
| mapper_scrollup | v4.0/src/MAPPER/SCROLLUP.ASM:54 | 67 | yes |
| mapper_dbcs | v4.0/src/MAPPER/DBCS.ASM:37 | 34 | yes |
| mapper_casemap | v4.0/src/MAPPER/CASEMAP.ASM:40 | 35 | yes |
| mapper_devconfg | v4.0/src/MAPPER/DEVCONFG.ASM:42 | 77 | yes |
| mapper_sfileinf | v4.0/src/MAPPER/SFILEINF.ASM:53 | 31 | yes |
| mapper_wchstra | v4.0/src/MAPPER/WCHSTRA.ASM:51 | 113 | yes |

The complete original kernel, COMMAND, BIOS, most utilities, memory managers, networking, and other versions remain unported.
boot/kernel.cpp manually implements selected DOS 1.25 compatibility services; it is not a complete translation of the original kernel.
Full DOS 4.0 mapper bodies retain external DOS/BIOS interrupt dependencies. See the [file-by-file DOS 4.0 audit](v4-missing.md) for remaining files and validation scope.
The JSON inventory contains source hashes, include edges, procedure locations, and hardware boundaries for every ASM/INC/C/H file.
All generated units use compiled C++ statements, labels, branches and loops; there is no per-instruction tick or program-counter executor.
DOS register/memory/flag semantics and relocated return/table tokens are retained as compatibility data. These units cannot execute self-modifying code or preserve original binary layout.
