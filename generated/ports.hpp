#pragma once
// Generated declarations for source-lifted routines.
#include "msdos/machine.hpp"
namespace msdos::v125 {
void command_scanoff(Machine& m);
void command_delim(Machine& m);
}
namespace msdos::v125 {
void command_getnum(Machine& m);
void command_indig(Machine& m);
void command_show(Machine& m);
void command_out2(Machine& m);
void command_out(Machine& m);
}
namespace msdos::v125 {
void hex2bin(Machine& m);
void hexchk(Machine& m);
}
namespace msdos::v125 {
void trans(Machine& m);
}
namespace msdos::v125 {
void fat_pack(Machine& m);
}
namespace msdos::v125 {
void getrec(Machine& m);
void setrndrec(Machine& m);
void getrrpos(Machine& m);
void getrrpos1(Machine& m);
}
namespace msdos::v125 {
void setvect(Machine& m);
}
namespace msdos::v200 {
void dsum(Machine& m);
}
namespace msdos::v400 {
void dosgetversion(Machine& m);
}
namespace msdos::v400 {
void dosqverify(Machine& m);
}
namespace msdos::v400 {
void dossetverify(Machine& m);
}
namespace msdos::v400 {
void dosmkdir(Machine& m);
}
namespace msdos::v400 {
void dosrmdir(Machine& m);
}
namespace msdos::v400 {
void doschdir(Machine& m);
}
namespace msdos::v400 {
void dosdelete(Machine& m);
}
namespace msdos::v400 {
void dosclose(Machine& m);
}
namespace msdos::v400 {
void dosread(Machine& m);
}
namespace msdos::v400 {
void doswrite(Machine& m);
}
namespace msdos::v400 {
void doschgfileptr(Machine& m);
}
namespace msdos::v400 {
void dosselectdisk(Machine& m);
}
namespace msdos::v400 {
void dosqfilemode(Machine& m);
}
namespace msdos::v400 {
void dossetfilemode(Machine& m);
}
namespace msdos::v400 {
void dosqhandtype(Machine& m);
}
namespace msdos::v400 {
void dosfreeseg(Machine& m);
}
namespace msdos::v400 {
void dosallocseg(Machine& m);
}
namespace msdos::v400 {
void dosreallocseg(Machine& m);
}
namespace msdos::v400 {
void dosgetdatetime(Machine& m);
}
namespace msdos::v400 {
void dossetdatetime(Machine& m);
}
namespace msdos::v400 {
void dosputmessage(Machine& m);
}
namespace msdos::v400 {
void dosdevioctl(Machine& m);
}
namespace msdos::v400 {
void dosexit(Machine& m);
}
namespace msdos::v400 {
void viosetcurpos(Machine& m);
}
namespace msdos::v400 {
void vioscrollup(Machine& m);
}
namespace msdos::v400 {
void dosgetdbcsev(Machine& m);
}
namespace msdos::v400 {
void doscasemap(Machine& m);
}
namespace msdos::v400 {
void dosdevconfig(Machine& m);
}
namespace msdos::v400 {
void dossetfileinfo(Machine& m);
}
namespace msdos::v400 {
void viowrtcharstratt(Machine& m);
}
