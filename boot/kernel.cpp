// Initial DOS 1.25 compatibility kernel. Original algorithms are cited where ported.
// This does not yet replace the entire original MSDOS.ASM or COMMAND.ASM.
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
extern "C" {
void bios_put(u32);
void bios_video(u32 operation,u32 value);
u32 bios_key();
u32 bios_status();
u32 bios_read(u32 lba, u32 segment, u32 offset);
u32 bios_write(u32 lba, u32 segment, u32 offset);
u32 bios_clock(u32 date);
u32 bios_aux(u32 function,u32 value);
void bios_print(u32 value);
u32 far_get(u32 segment, u32 offset);
void far_set(u32 segment, u32 offset, u32 value);
void install_dos();
void run_com();
u32 call5_offset();
struct Frame { u16 di,si,bp,sp,bx,dx,cx,ax,es,ds,ip,cs,flags; };
Frame dos_frame;
}
static_assert(sizeof(Frame) == 26);
static u8 sector[512], fat[4608], root[7168];
static u16 dta_segment = 0x2000, dta_offset = 0x80;
static constexpr u16 reserved = 65, root_start = 83, data_start = 97;
#ifdef MSDOS_KERNEL_HOST_TEST
extern "C" u16 host_offset(const void*);
#endif

#include "console.hpp"
static void put(char c) { if (c == '\n') bios_put('\r'); bios_put(static_cast<u8>(c)); }
static void print(const char* s) { while (*s) put(*s++); }
static void styled(const char* text,u8 attribute) {console_color(attribute);print(text);console_color(text_normal);}
static u16 word(const u8* p) { return static_cast<u16>(p[0] | (static_cast<u16>(p[1]) << 8)); }
static u32 longword(const u8* p) { return word(p) | (static_cast<u32>(word(p+2)) << 16); }
static u16 fw(u16 seg, u16 off) { return static_cast<u16>(far_get(seg,off) | (far_get(seg,static_cast<u16>(off+1)) << 8)); }
static void sw(u16 seg,u16 off,u16 v) { far_set(seg,off,v); far_set(seg,static_cast<u16>(off+1),v>>8); }
static char upper(char c) { return c >= 'a' && c <= 'z' ? static_cast<char>(c-32) : c; }
static bool equal(const char* a,const char* b) { while (*a && *a == *b) {a++;b++;} return *a == *b; }
static u16 pointer(const void* p) {
#ifdef MSDOS_KERNEL_HOST_TEST
    return host_offset(p);
#else
    return static_cast<u16>(reinterpret_cast<u32>(p));
#endif
}
static bool read_sector(u16 lba) { return bios_read(lba,0x1000,pointer(sector)) != 0; }

// v1.25/MSDOS.ASM UNPACK: entry N is at N+N/2; odd entries shift four bits.
static u16 next_cluster(u16 n) {
    const u32 offset = n + n/2;
    if (offset+1 >= sizeof(fat)) return 0xfff;
    return static_cast<u16>((word(fat+offset) >> ((n&1) ? 4 : 0)) & 0xfff);
}
static bool load_volume() {
    for (u16 i=0;i<9;i++) {
        if (!read_sector(static_cast<u16>(reserved+i))) return false;
        for (u16 j=0;j<512;j++) fat[i*512+j]=sector[j];
    }
    for (u16 i=0;i<14;i++) {
        if (!read_sector(static_cast<u16>(root_start+i))) return false;
        for (u16 j=0;j<512;j++) root[i*512+j]=sector[j];
    }
    return true;
}
static bool file_character(char c) {
    return static_cast<u8>(c)>32 && c!='"' && c!='*' && c!='+' && c!=',' && c!='/' &&
        c!=':' && c!=';' && c!='<' && c!='=' && c!='>' && c!='?' && c!='[' && c!='\\' && c!=']' && c!='|';
}
static bool name83(const char* s,char* name) {
    for (u16 i=0;i<11;i++) name[i]=' ';
    u16 n=0;
    while (*s && *s!='.') {
        if (n>=8 || !file_character(*s)) return false;
        name[n++]=upper(*s++);
    }
    if (!n) return false;
    if (*s=='.') { s++;n=8;while (*s) {if(n>=11 || *s=='.' || !file_character(*s)) return false;name[n++]=upper(*s++);} }
    return true;
}
static int find(const char* name,int from=0,bool wildcard=false) {
    for (int i=from;i<224;i++) {
        const u8* entry=root+i*32;
        if (!entry[0]) break;
        if (entry[0]==0xe5 || (entry[11]&0x18)) continue;
        bool match=true;
        for(int j=0;j<11;j++) if (!(wildcard && name[j]=='?') && entry[j]!=static_cast<u8>(name[j])) match=false;
        if(match) return i;
    }
    return -1;
}
static u8 named_device(const char* name) {
    const char* names[]={"CON     ","AUX     ","NUL     ","PRN     ","LST     ","COM1    ","LPT1    "};
    for(u16 i=0;i<7;i++) {
        bool match=true;for(u16 j=0;j<8;j++) if(name[j]!=names[i][j]) match=false;
        if(match) return static_cast<u8>(i==4 || i==6 ? 4 : i==5 ? 2 : i+1);
    }
    return 0;
}
#include "storage.hpp"
#include "clock.hpp"
static bool read_file(int index,u32 position,u16 seg,u16 off,u16 count) {
    if(index<0 || index>=224 || static_cast<u32>(off)+count>65536) return false;
    const u8* entry=root+index*32;
    if(!entry[0] || entry[0]==0xe5 || (entry[11]&0x18) || longword(entry+28)>cluster_count*512UL) return false;
    if(position>longword(entry+28) || count>longword(entry+28)-position) return false;
    if(!count) return true;
    u16 clusters,tail;
    if(!chain_info(word(entry+26),clusters,tail) || (position+count+511)/512>clusters) return false;
    u16 cluster=word(entry+26);
    u32 skip=position/512;
    for(u32 i=0;i<skip;i++) { if(cluster<2 || cluster>=0xff8) return false;cluster=next_cluster(cluster); }
    u16 within=static_cast<u16>(position%512), done=0;
    while(done<count) {
        if(!valid_cluster(cluster) || !read_sector(static_cast<u16>(data_start+cluster-2))) return false;
        while(within<512 && done<count) far_set(seg,static_cast<u16>(off+done++),sector[within++]);
        within=0;cluster=next_cluster(cluster);
    }
    return true;
}
#include "fcb.hpp"
static u8 drive_parameters[20];
static void prepare_psp(u16 segment,u16 memory_end) {
    sw(segment,0,0x20cd);sw(segment,2,memory_end);
    far_set(segment,5,0x9a);sw(segment,6,static_cast<u16>(call5_offset()));sw(segment,8,0x1000);
    for(u16 j=0;j<12;j++) far_set(segment,static_cast<u16>(10+j),far_get(0,static_cast<u16>(0x88+j)));
}
extern "C" u32 dos_dispatch() {
    const u8 function=static_cast<u8>(dos_frame.ax>>8);
    switch(function) {
    case 0: if(!flush_volume()) print("Disk write failed\n");return 1;
    case 1: al(static_cast<u8>(bios_key()));bios_put(static_cast<u8>(dos_frame.ax));break;
    case 2: bios_put(static_cast<u8>(dos_frame.dx));break;
    case 3: al(static_cast<u8>(bios_aux(2,0)));break;
    case 4: bios_aux(1,static_cast<u8>(dos_frame.dx));break;
    case 5: bios_print(static_cast<u8>(dos_frame.dx));break;
    case 6:
        if((dos_frame.dx&255)!=255) bios_put(static_cast<u8>(dos_frame.dx));
        else if(bios_status()) {al(static_cast<u8>(bios_key()));dos_frame.flags&=static_cast<u16>(~0x40);}
        else {al(0);dos_frame.flags|=0x40;}
        break;
    case 7: case 8: al(static_cast<u8>(bios_key()));break;
    case 9:
        for(u32 i=0;i<65536;i++) {const u8 c=static_cast<u8>(far_get(dos_frame.ds,static_cast<u16>(dos_frame.dx+i)));if(c=='$') break;bios_put(c);}
        break;
    case 10: {
        const u16 seg=dos_frame.ds,off=dos_frame.dx;
        const u8 maximum=static_cast<u8>(far_get(seg,off));
        u8 n=0;
        while(maximum) {
            const u8 c=static_cast<u8>(bios_key());
            if(c=='\r' || c=='\n') {far_set(seg,static_cast<u16>(off+2+n),'\r');put('\n');break;}
            if(c=='\b' && n) {n--;print("\b \b");}
            else if(c>=32 && n+1<maximum) {far_set(seg,static_cast<u16>(off+2+n++),c);put(static_cast<char>(c));}
        }
        far_set(seg,static_cast<u16>(off+1),n);break;
    }
    case 11: al(bios_status() ? 255 : 0);break;
    case 12: {
        const u8 next=static_cast<u8>(dos_frame.ax);
        while(bios_status()) bios_key();
        if(next==1 || next==6 || next==7 || next==8 || next==10) {
            dos_frame.ax=static_cast<u16>(next<<8);const u32 result=dos_dispatch();
            dos_frame.ax=static_cast<u16>((12<<8)|(dos_frame.ax&255));return result;
        }
        al(0);break;
    }
    case 13: al(flush_volume() ? 0 : 255);break;
    case 14: al(1);break;
    case 15: open_fcb(false);break;
    case 16: {
        char name[11];const u16 fcb=normal_fcb(dos_frame.ds,dos_frame.dx);
        const bool device=fcb_name(dos_frame.ds,fcb,name) && named_device(name);
        al((device || fcb_find(dos_frame.ds,dos_frame.dx)>=0) && flush_volume() ? 0 : 255);break;
    }
    case 17: search_fcb(true);break;
    case 18: search_fcb(false);break;
    case 19: delete_fcb();break;
    case 20: transfer_fcb(false,false,false);break;
    case 21: transfer_fcb(false,false,true);break;
    case 22: open_fcb(true);break;
    case 23: rename_fcb();break;
    // These CP/M compatibility calls also return zero in DOS 1.25's dispatch.
    case 24: case 29: case 30: case 32: al(0);break;
    case 25: al(0);break;
    case 26: dta_segment=dos_frame.ds;dta_offset=dos_frame.dx;break;
    case 27: case 28:
        if(function==28 && (dos_frame.dx&255)>1) {al(255);break;}
        al(1);dos_frame.bx=pointer(fat);dos_frame.ds=0x1000;dos_frame.cx=512;dos_frame.dx=cluster_count;break;
    case 31:
        for(u16 j=0;j<20;j++) drive_parameters[j]=0;
        store_word(drive_parameters+2,512);store_word(drive_parameters+6,reserved);
        drive_parameters[8]=2;store_word(drive_parameters+9,224);store_word(drive_parameters+11,data_start);
        store_word(drive_parameters+13,cluster_limit-1);drive_parameters[15]=9;
        store_word(drive_parameters+16,root_start);store_word(drive_parameters+18,pointer(fat));
        dos_frame.ds=0x1000;dos_frame.bx=pointer(drive_parameters);break;
    case 33: transfer_fcb(true,false,false);break;
    case 34: transfer_fcb(true,false,true);break;
    case 35: {
        const u16 seg=dos_frame.ds,fcb=normal_fcb(seg,dos_frame.dx);const int index=fcb_find(seg,dos_frame.dx);
        char name[11];const bool device=fcb_name(seg,fcb,name) && named_device(name);
        if(index<0 && !device) {al(255);break;}
        u16 size=fw(seg,static_cast<u16>(fcb+14));if(!size) size=128;
        const u32 bytes=device ? 0 : longword(root+index*32+28);
        set_random_record(seg,fcb,size,bytes/size+(bytes%size ? 1 : 0));al(0);break;
    }
    case 36: {
        const u16 fcb=normal_fcb(dos_frame.ds,dos_frame.dx);
        set_random_record(dos_frame.ds,fcb,fw(dos_frame.ds,static_cast<u16>(fcb+14)),sequential_record(dos_frame.ds,fcb));break;
    }
    case 37: sw(0,static_cast<u16>((dos_frame.ax&255)*4),dos_frame.dx);sw(0,static_cast<u16>((dos_frame.ax&255)*4+2),dos_frame.ds);break;
    case 38: {
        const u16 segment=dos_frame.dx;
        for(u16 j=0;j<256;j++) far_set(segment,j,far_get(dos_frame.cs,j));
        prepare_psp(segment,fw(dos_frame.cs,2));dos_frame.ds=segment;dos_frame.es=segment;break;
    }
    case 39: transfer_fcb(true,true,false);break;
    case 40: transfer_fcb(true,true,true);break;
    case 41: parse_fcb();break;
    case 42: clock_now();dos_frame.cx=clock_year;dos_frame.dx=static_cast<u16>((clock_month<<8)|clock_day);al(weekday());break;
    case 43: al(set_date(dos_frame.cx,static_cast<u8>(dos_frame.dx>>8),static_cast<u8>(dos_frame.dx)) ? 0 : 255);break;
    case 44: {
        const u32 now=clock_now();dos_frame.cx=static_cast<u16>(((now/360000)<<8)|((now/6000)%60));
        dos_frame.dx=static_cast<u16>((((now/100)%60)<<8)|(now%100));al(0);break;
    }
    case 45: al(set_time(static_cast<u8>(dos_frame.cx>>8),static_cast<u8>(dos_frame.cx),static_cast<u8>(dos_frame.dx>>8),static_cast<u8>(dos_frame.dx)) ? 0 : 255);break;
    case 46: verify_writes=(dos_frame.ax&1)!=0;al(static_cast<u8>(dos_frame.ax&1));break;
    case 48: dos_frame.ax=0x1901;break; // 1.25 (the 1.x API predates this query).
    default:
        print("\nUnsupported DOS call AH=");
        const char* digits="0123456789ABCDEF";put(digits[function>>4]);put(digits[function&15]);put('\n');
        return 1;
    }
    return 0;
}
extern "C" u32 raw_dispatch(u32 write) {
    const u32 start=dos_frame.dx,count=dos_frame.cx,offset=dos_frame.bx;
    dos_frame.flags|=1;
    if((dos_frame.ax&255)!=0) {dos_frame.ax=0x8002;return 0;}
    if(start>2880 || count>2880-start || count*512UL+offset>65536) {dos_frame.ax=0x0208;return 0;}
    if(!flush_volume()) {dos_frame.ax=0x0202;return 0;}
    bool success=true;
    for(u32 n=0;n<count;n++) {
        const u32 dest=offset+n*512;
        if(write ? !bios_write(start+n,dos_frame.ds,dest) : !bios_read(start+n,dos_frame.ds,dest)) {success=false;break;}
    }
    if(write && count && start<97 && start+count>reserved) success=load_volume() && success;
    if(success) {dos_frame.flags&=static_cast<u16>(~1);dos_frame.ax=0;}
    else dos_frame.ax=0x0202;
    return 0;
}
static bool execute(const char* command,const char* tail) {
    if(command[0] && command[1]==':') {if(upper(command[0])!='A') return false;command+=2;}
    char name[11];if(!name83(command,name)) return false;
    if(name[8]==' ') {name[8]='C';name[9]='O';name[10]='M';}
    if(name[8]!='C' || name[9]!='O' || name[10]!='M') return false;
    const int index=find(name);if(index<0) return false;
    const u32 size=longword(root+index*32+28);
    if(!size || size>0xfe00) {print("COM program too large\n");return true;}
    for(u32 i=0;i<0x10000;i++) far_set(0x2000,i,0);
    prepare_psp(0x2000,0x9000);
    u16 length=0,source=0;
    if(*tail && *tail!=' ' && *tail!='\t') far_set(0x2000,0x81+length++,' ');
    while(tail[source] && length<126) far_set(0x2000,static_cast<u16>(0x81+length++),tail[source++]);
    far_set(0x2000,0x80,length);far_set(0x2000,static_cast<u16>(0x81+length),'\r');
    // Populate the two default FCBs from whitespace-delimited command arguments.
    for(u16 arg=0;arg<2;arg++) {
        for(u16 j=0;j<11;j++) far_set(0x2000,static_cast<u16>(0x5d+arg*16+j),' ');
        while(*tail==' ' || *tail=='\t') tail++;
        char token[13];u16 n=0;while(*tail && *tail!=' ' && *tail!='\t') {if(n<12) token[n++]=*tail;tail++;}token[n]=0;
        char fcb_name[11];
        if(n && name83(token,fcb_name)) for(u16 j=0;j<11;j++) far_set(0x2000,static_cast<u16>(0x5d+arg*16+j),static_cast<u8>(fcb_name[j]));
    }
    if(!read_file(index,0,0x2000,0x100,static_cast<u16>(size))) {print("Program read failed\n");return true;}
    dta_segment=0x2000;dta_offset=0x80;install_dos();run_com();
    if(!flush_volume()) print("Disk write failed\n");
    for(u16 j=0;j<12;j++) far_set(0,static_cast<u16>(0x88+j),far_get(0x2000,static_cast<u16>(10+j)));
    install_dos();put('\n');return true;
}
#include "shell.hpp"
extern "C" void kernel_main() {
    install_dos();init_clock();console_init();
    styled("MS-DOS-cpp",text_accent);print("  /  ");styled("Your DOS workspace\n\n",text_muted);
    styled("DOS 1.25 compatibility  /  Type HELP to get started.\n\n",text_muted);
    if(!load_volume()) {print("FAT12 disk read failed\n");return;}
    styled("  Ready",text_green);print("  Drive A: mounted. FAT12 disk writes enabled.\n");
    styled("  Start",text_blue);print("  HELP for commands, DIR for files, DEMO README.TXT to explore.\n\n");
    run_batch("AUTOEXEC.BAT","");
    char line[128];
    for(;;) {
        shell_prompt();shell_input(line,sizeof(line));
        command_line(line);
        console_color(text_normal);put('\n');
    }
}
