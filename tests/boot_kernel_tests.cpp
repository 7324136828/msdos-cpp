#define MSDOS_KERNEL_HOST_TEST
#include "../boot/kernel.cpp"
#include <array>
#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
std::array<unsigned char,1<<20> ram{};
std::vector<unsigned char> disk;
std::string terminal;
std::deque<u32> keys;
std::string auxiliary,printer;
bool fail_writes=false,corrupt_writes=false;
unsigned launches=0;
struct Mapping {void* pointer;unsigned offset,size;};
std::vector<Mapping> mappings;
unsigned mappingOffset=0x4000;
void require(bool ok,const char* text) {if(!ok) throw std::runtime_error(text);}
unsigned address(unsigned seg,unsigned off) {return ((seg<<4)+(off&0xffff))&0xfffff;}
unsigned char& memory(unsigned seg,unsigned off) {
    if(seg==0x1000) for(const auto& m:mappings) if(off>=m.offset && off<m.offset+m.size)
        return static_cast<unsigned char*>(m.pointer)[off-m.offset];
    return ram[address(seg,off)];
}
void function(unsigned ah,unsigned ax=0) {dos_frame.ax=static_cast<u16>((ah<<8)|(ax&255));}
void default_fcb(unsigned off,const char* name) {
    char buffer[11];require(name83(name,buffer),"test file name");
    for(unsigned i=0;i<37;i++) far_set(0x2000,off+i,0);
    for(unsigned i=0;i<11;i++) far_set(0x2000,off+1+i,buffer[i]);
    dos_frame.ds=0x2000;dos_frame.dx=static_cast<u16>(off);
}
bool command(std::string text) {
    std::vector<char> line(text.begin(),text.end());line.push_back(0);return command_line(line.data());
}
int make_test_file(const char* text,const std::string& bytes) {
    char name[11];require(name83(text,name),"test file name");const int index=create_file(name);require(index>=0,"test file create");
    require(bytes.size()<65536,"test payload fits DTA");
    for(unsigned j=0;j<bytes.size();j++) far_set(0x4000,j,bytes[j]);
    require(write_file(index,0,0x4000,0,static_cast<u16>(bytes.size())) && flush_volume(),"test payload write");return index;
}
}
extern "C" u16 host_offset(const void* p) {
    for(const auto& m:mappings) if(m.pointer==p) return static_cast<u16>(m.offset);
    const unsigned size=p==fat ? sizeof(fat) : p==drive_parameters ? sizeof(drive_parameters) : 512;
    const unsigned off=mappingOffset;mappingOffset+=size;require(mappingOffset<65536,"host mapping capacity");
    mappings.push_back({const_cast<void*>(p),off,size});return static_cast<u16>(off);
}
extern "C" void bios_put(u32 ch) {terminal+=static_cast<char>(ch);console_write(ch);}
extern "C" void bios_video(u32,u32) {}
extern "C" u32 bios_key() {require(!keys.empty(),"unexpected keyboard read");const auto ch=keys.front();keys.pop_front();return ch;}
extern "C" u32 bios_status() {return !keys.empty();}
extern "C" u32 far_get(u32 seg,u32 off) {return memory(seg,off);}
extern "C" void far_set(u32 seg,u32 off,u32 value) {memory(seg,off)=static_cast<u8>(value);}
extern "C" u32 bios_read(u32 lba,u32 seg,u32 off) {
    if(static_cast<std::size_t>(lba+1)*512>disk.size()) return 0;
    for(unsigned i=0;i<512;i++) far_set(seg,off+i,disk[lba*512+i]);
    return 1;
}
extern "C" u32 bios_write(u32 lba,u32 seg,u32 off) {
    if(fail_writes || static_cast<std::size_t>(lba+1)*512>disk.size()) return 0;
    for(unsigned i=0;i<512;i++) disk[lba*512+i]=static_cast<u8>(far_get(seg,off+i));
    if(corrupt_writes) disk[lba*512]^=1;
    return 1;
}
extern "C" u32 bios_clock(u32 date) {return date ? 0x20261006 : 0x14203000;}
extern "C" u32 bios_aux(u32 fn,u32 ch) {if(fn==1) auxiliary+=static_cast<char>(ch);return fn==2 ? 'R' : ch;}
extern "C" void bios_print(u32 ch) {printer+=static_cast<char>(ch);}
extern "C" u32 call5_offset() {return 0x1234;}
extern "C" void install_dos() {}
extern "C" void run_com() {
    launches++;
    require(fw(0x2000,0)==0x20cd && fw(0x2000,2)==0x9000,"PSP installed");
    require(far_get(0x2000,5)==0x9a && fw(0x2000,6)==0x1234 && fw(0x2000,8)==0x1000,"CP/M CALL 5 pointer");
    require(far_get(0x2000,0x100)==0xb4 && far_get(0x2000,0x101)==9,"native COM bytes loaded at PSP:100");
    require(far_get(0x2000,0x80)==10 && far_get(0x2000,0x81)==' ',"DOS command tail layout");
    require(far_get(0x2000,0x5d)=='H' && far_get(0x2000,0x65)=='T',"default FCB from command tail");
}
int main(int argc,char** argv) {
    try {
        require(argc==2,"fixture path required");
        std::ifstream input(argv[1],std::ios::binary);require(static_cast<bool>(input),"fixture open");
        disk.assign(std::istreambuf_iterator<char>(input),{});
        require(load_volume(),"FAT12 volume load");
        char name[11];require(name83("multi.bin",name),"case-insensitive 8.3 names");
        const auto file=find(name);require(file==1,"root lookup");
        require(next_cluster(3)==7 && next_cluster(7)==5,"fragmented FAT chain");
        require(read_file(file,0,0x2000,0x4000,1300),"read full fragmented file");
        for(unsigned i=0;i<1300;i++) require(far_get(0x2000,0x4000+i)==((i*7)&255),"fragmented bytes");
        require(read_file(file,510,0x2000,0x3000,12),"cross-sector partial read");
        for(unsigned i=0;i<12;i++) require(far_get(0x2000,0x3000+i)==(((510+i)*7)&255),"cross-sector data");
        require(!read_file(file,1299,0x2000,0x3000,2),"read beyond EOF rejected");
        require(!read_file(file,0,0x2000,0xffff,2),"DMA wrap rejected");
        require(!name83("TOOLONGNAME.TXT",name) && !name83("../X.TXT",name),"invalid DOS names rejected");
        default_fcb(0x5c,"HELLO.TXT");function(15);require(!dos_dispatch() && (dos_frame.ax&255)==0,"FCB open");
        require(fw(0x2000,0x5c+14)==128,"default record size");
        dta_segment=0x2000;dta_offset=0x8000;
        function(20);dos_dispatch();require((dos_frame.ax&255)==3,"partial FCB record status");
        require(far_get(0x2000,0x8000)=='F' && far_get(0x2000,0x8000+127)==0,"FCB data and padding");
        require(far_get(0x2000,0x5c+32)==1,"FCB next record");
        function(20);dos_dispatch();require((dos_frame.ax&255)==1,"FCB EOF status");
        default_fcb(0x5c,"MULTI.BIN");function(15);dos_dispatch();
        sw(0x2000,0x5c+33,3);dos_frame.cx=2;function(39);dos_dispatch();
        require(dos_frame.cx==2 && (dos_frame.ax&255)==0,"random block read count");
        for(unsigned i=0;i<256;i++) require(far_get(0x2000,0x8000+i)==(((384+i)*7)&255),"random FCB records");
        dta_offset=0xfff0;function(20);dos_dispatch();require((dos_frame.ax&255)==2,"FCB segment-boundary error");
        dta_offset=0x8000;sw(0x2000,0x5c+14,0xffff);sw(0x2000,0x5c+33,0xffff);far_set(0x2000,0x5c+35,0xff);
        function(33);dos_dispatch();require((dos_frame.ax&255)==1,"oversized random record cannot wrap multiplication");
        dos_frame.ds=0x2000;dos_frame.dx=0x7000;
        far_set(0x2000,0x7000,'O');far_set(0x2000,0x7001,'K');far_set(0x2000,0x7002,'$');
        function(9);dos_dispatch();require(terminal.ends_with("OK"),"DOS string output");
        keys.push_back('x');function(1);dos_dispatch();require((dos_frame.ax&255)=='x' && terminal.ends_with("OKx"),"DOS input with echo");
        dos_frame.dx=255;function(6);dos_dispatch();require((dos_frame.flags&0x40)!=0,"direct console zero flag");
        function(3);dos_dispatch();require((dos_frame.ax&255)=='R',"auxiliary input");
        dos_frame.dx='S';function(4);dos_dispatch();require(auxiliary=="S","auxiliary output");
        dos_frame.dx='P';function(5);dos_dispatch();require(printer=="P","printer output");
        default_fcb(0x5c,"NEW.DAT");function(22);dos_dispatch();require((dos_frame.ax&255)==0,"FCB create");
        dta_offset=0x8000;
        for(unsigned i=0;i<128;i++) far_set(0x2000,0x8000+i,i^0xa5);
        for(unsigned r=0;r<3;r++) {function(21);dos_dispatch();require((dos_frame.ax&255)==0,"sequential FCB write");}
        require(far_get(0x2000,0x5c+32)==3 && fw(0x2000,0x5c+16)==384,"write advances FCB and file size");
        sw(0x2000,0x5c+33,10);function(34);dos_dispatch();require((dos_frame.ax&255)==0,"random FCB write with gap");
        require(far_get(0x2000,0x5c+32)==10 && fw(0x2000,0x5c+33)==10,"single random write sets current record without increment");
        require(fw(0x2000,0x5c+16)==1408,"random write extends file");
        function(16);dos_dispatch();require((dos_frame.ax&255)==0,"FCB close persists");
        require(std::equal(disk.begin()+65*512,disk.begin()+74*512,disk.begin()+74*512),"both FAT copies updated");
        require(load_volume(),"reload written volume");require(name83("NEW.DAT",name),"new name");const int created=find(name);
        require(created>=0 && longword(root+created*32+28)==1408,"directory size survives reload");
        require(read_file(created,0,0x2000,0x4000,1408),"read persisted allocation chain");
        for(unsigned i=0;i<1408;i++) require(far_get(0x2000,0x4000+i)==(i<384 || i>=1280 ? ((i%128)^0xa5) : 0),"persisted bytes and zero-filled gap");
        function(35);dos_dispatch();require((dos_frame.ax&255)==0 && fw(0x2000,0x5c+33)==11,"FCB size rounded to records");
        far_set(0x2000,0x5c+32,7);function(36);dos_dispatch();require(fw(0x2000,0x5c+33)==7,"set random record");
        const u16 before=free_clusters();sw(0x2000,0x5c+33,2);dos_frame.cx=0;function(40);dos_dispatch();
        require((dos_frame.ax&255)==0 && fw(0x2000,0x5c+16)==256 && free_clusters()==before+2,"zero-count random write truncates and releases clusters");
        sw(0x2000,0x5c+14,1);sw(0x2000,0x5c+33,0);far_set(0x2000,0x5c+35,0);far_set(0x2000,0x5c+36,1);
        function(33);dos_dispatch();require((dos_frame.ax&255)==1,"fourth random-record byte used for small records");
        default_fcb(0x5c,"NEW.DAT");char target[11];require(name83("RENAMED.DAT",target),"rename target");
        for(unsigned j=0;j<11;j++) far_set(0x2000,0x5c+17+j,target[j]);
        function(23);dos_dispatch();require((dos_frame.ax&255)==0 && find(target)==created && find(name)<0,"FCB rename");
        default_fcb(0x5c,"RENAMED.DAT");function(19);dos_dispatch();require((dos_frame.ax&255)==0,"FCB delete");
        require(load_volume() && find(target)<0,"deletion survives reload");
        // Search results are drive + the 32-byte directory entry, and state is per FCB.
        default_fcb(0x5c,"HELLO.TXT");function(17);dos_dispatch();
        require((dos_frame.ax&255)==0 && far_get(0x2000,0x8000)==1 && far_get(0x2000,0x8001)=='H',"FCB search result layout");
        function(18);dos_dispatch();require((dos_frame.ax&255)==255,"FCB search exhaustion");
        function(18);dos_dispatch();require((dos_frame.ax&255)==255,"FCB exhausted search does not restart");
        default_fcb(0x1000,"MULTI.BIN");
        for(unsigned i=0;i<7;i++) far_set(0x2000,0xff9+i,i==0 ? 255 : 0);
        dos_frame.dx=0xff9;function(17);dos_dispatch();require((dos_frame.ax&255)==0 && far_get(0x2000,0x8000)==255 && far_get(0x2000,0x8007)==1 && far_get(0x2000,0x8008)=='M',"extended FCB search result");
        // Parsing preserves requested defaults, expands wildcards, and returns DS:SI at delimiter.
        const std::string specification=" , a:foo*.b? end";
        for(unsigned i=0;i<=specification.size();i++) far_set(0x2000,0x7000+i,i<specification.size() ? specification[i] : 0);
        dos_frame.ds=dos_frame.es=0x2000;dos_frame.si=0x7000;dos_frame.di=0x5c;function(41,1);dos_dispatch();
        require((dos_frame.ax&255)==1 && far_get(0x2000,0x5c)==1 && far_get(0x2000,0x60)=='?' && far_get(0x2000,dos_frame.si)==' ',"DOS wildcard parser and input position");
        // Exhaustion must not publish a longer file or mutate the original FAT.
        std::array<u8,sizeof(fat)> saved_fat;std::copy(std::begin(fat),std::end(fat),saved_fat.begin());
        const u16 original_dirty=dirty_fat;
        for(u16 n=2;n<cluster_limit;n++) if(!next_cluster(n)) pack_cluster(n,0xff7);
        require(!write_file(file,2000,0x2000,0x8000,128) && longword(root+file*32+28)==1300,"full disk rejects extension before mutation");
        std::copy(saved_fat.begin(),saved_fat.end(),std::begin(fat));dirty_fat=original_dirty;
        pack_cluster(5,3);require(!read_file(file,0,0x2000,0x4000,1) && !delete_file(file),"cyclic FAT chain rejected for reads and deletion");
        std::copy(saved_fat.begin(),saved_fat.end(),std::begin(fat));dirty_fat=original_dirty;
        require(name83("FAIL.DAT",name),"failure name");const int failed=create_file(name);require(failed>=0,"failure fixture create");
        fail_writes=true;require(!write_file(failed,0,0x2000,0x8000,128) && word(root+failed*32+26)==0 && longword(root+failed*32+28)==0,"BIOS write failure does not allocate or extend");
        fail_writes=false;require(flush_volume(),"retry metadata after I/O failure");
        verify_writes=true;corrupt_writes=true;require(!write_file(failed,0,0x2000,0x8000,128),"verify detects media corruption");
        corrupt_writes=false;verify_writes=false;
        // RTC seed, date validation, weekday calculation and BIOS midnight rollover.
        init_clock();function(42);dos_dispatch();require(dos_frame.cx==2026 && dos_frame.dx==0x0a06 && (dos_frame.ax&255)==2,"RTC date and weekday");
        function(44);dos_dispatch();require(dos_frame.cx==0x0e14 && dos_frame.dx==0x1e00,"RTC time");
        dos_frame.cx=2023;dos_frame.dx=0x021d;function(43);dos_dispatch();require((dos_frame.ax&255)==255,"invalid leap date rejected");
        dos_frame.cx=2024;dos_frame.dx=0x021d;function(43);dos_dispatch();require((dos_frame.ax&255)==0,"valid leap date accepted");
        sw(0,0x46c,static_cast<u16>(ticks_per_day-10));sw(0,0x46e,static_cast<u16>((ticks_per_day-10)>>16));
        clock_anchor=ticks(); // Moving the fixture clock is not elapsed session time.
        require(set_time(23,59,59,80),"midnight fixture time");sw(0,0x46c,10);sw(0,0x46e,0);
        function(42);dos_dispatch();require(dos_frame.cx==2024 && dos_frame.dx==0x0301,"date advances across BIOS tick wrap");
        require(tick_hundredths(ticks_per_day-1)==hundredths_per_day-6,"exact tick scaling without 64-bit helpers");
        terminal.clear();require(command("copy A:multi.bin copied.bin"),"COPY command dispatch");
        require(terminal.find("1 File(s) copied")!=std::string::npos,"COPY reports successful file count");
        require(name83("COPIED.BIN",name),"copied name");const int copied=find(name);
        require(copied>=0 && read_file(copied,0,0x2000,0x4000,1300),"shell copy reads fragmented source and writes full target");
        for(unsigned i=0;i<1300;i++) require(far_get(0x2000,0x4000+i)==((i*7)&255),"shell copy exact bytes");
        const auto before_copy=disk;terminal.clear();command("COPY COPIED.BIN COPIED.BIN");
        require(disk==before_copy && terminal.find("overlaps a source")!=std::string::npos,"shell self-copy rejected before truncation");
        command("REN COPIED.BIN MOVED.BIN");require(name83("MOVED.BIN",name) && find(name)==copied,"shell rename");
        command("DEL MOVED.BIN");require(find(name)<0,"shell delete");
        terminal.clear();command("TYPE HELLO.TXT");require(terminal.find("FCB read works.")!=std::string::npos,"TYPE reads through kernel storage");
        terminal.clear();command("DIR *.TXT /W");require(terminal.find("HELLO.TXT")!=std::string::npos && terminal.find("MULTI.BIN")==std::string::npos,"DIR wildcard filter");
        const std::string script="REM batch regression\r\nCOPY %1 %2\r\nTYPE %2\r\nREN %2 FINAL.TXT\r\n";
        make_test_file("SCRIPT.BAT",script);terminal.clear();require(command("SCRIPT HELLO.TXT OUT.TXT"),"BAT dispatch");
        require(name83("FINAL.TXT",name) && find(name)>=0 && terminal.find("FCB read works.")!=std::string::npos,"batch parameter expansion and command execution");
        make_test_file("LOOP.BAT","LOOP\r\n");terminal.clear();command("LOOP");
        require(batch_depth==0 && terminal.find("nesting limit")!=std::string::npos,"batch recursion bounded and unwound");
        terminal.clear();command("DATE 2024-02-29");command("TIME 12:34:56");
        require(terminal.find("2024-02-29")!=std::string::npos && terminal.find("12:34:56")!=std::string::npos,"shell date/time");
        command("VERIFY ON");require(verify_writes,"shell VERIFY ON");command("VERIFY OFF");require(!verify_writes,"shell VERIFY OFF");
        // DOS output is byte-preserving; CRLF must not gain a second CR.
        terminal.clear();dos_frame.ds=0x2000;dos_frame.dx=0x7000;
        far_set(0x2000,0x7000,'\r');far_set(0x2000,0x7001,'\n');far_set(0x2000,0x7002,'$');function(9);dos_dispatch();
        require(terminal=="\r\n","INT 21h console preserves CRLF");
        default_fcb(0x5c,"NUL");function(22);dos_dispatch();require((dos_frame.ax&255)==0,"NUL creation opens a device");
        function(20);dos_dispatch();require((dos_frame.ax&255)==1,"NUL read EOF");
        function(21);dos_dispatch();require((dos_frame.ax&255)==0,"NUL write succeeds");
        require(name83("NUL",name) && find(name)<0,"device opens do not create files");
        default_fcb(0x5c,"CON");function(15);dos_dispatch();require((dos_frame.ax&255)==0,"CON FCB open");
        console_position=console_length=0;keys={'H','i','\r',26,'\r'};dta_offset=0x8000;
        function(20);dos_dispatch();require((dos_frame.ax&255)==3 && far_get(0x2000,0x8000)=='H' && far_get(0x2000,0x8004)==26 && far_get(0x2000,0x807f)==0,"CON records, CRLF and control-Z padding");
        function(20);dos_dispatch();require((dos_frame.ax&255)==1 && keys.empty(),"CON EOF persists in FCB");
        default_fcb(0x5c,"LPT1");function(15);dos_dispatch();printer.clear();
        far_set(0x2000,0x8000,'O');far_set(0x2000,0x8001,'K');far_set(0x2000,0x8002,26);
        function(21);dos_dispatch();require((dos_frame.ax&255)==0 && printer=="OK","printer device aliases and control-Z termination");
        // Absolute sector entry points use the same hardware boundary as the kernel.
        dos_frame.ax=0;dos_frame.ds=0x2000;dos_frame.bx=0x8000;dos_frame.cx=1;dos_frame.dx=65;
        raw_dispatch(0);require(!(dos_frame.flags&1) && far_get(0x2000,0x8000)==0xf0,"absolute FAT sector read");
        dos_frame.ax=1;raw_dispatch(0);require((dos_frame.flags&1) && dos_frame.ax==0x8002,"absolute read rejects unknown drive");
        dos_frame.ax=0;dos_frame.bx=0xff00;raw_dispatch(0);require(dos_frame.flags&1,"absolute sector DMA wrap rejected");
        dos_frame.ax=0;dos_frame.bx=0x8000;dos_frame.dx=2879;
        for(unsigned i=0;i<512;i++) far_set(0x2000,0x8000+i,i^0x5a);
        raw_dispatch(1);require(!(dos_frame.flags&1) && disk[2879*512]==0x5a,"absolute sector write");
        fail_writes=true;dos_frame.ax=0;raw_dispatch(1);require(dos_frame.flags&1,"absolute BIOS failure propagated");fail_writes=false;
        function(27);dos_dispatch();require((dos_frame.ax&255)==1 && dos_frame.cx==512 && dos_frame.dx==cluster_count && dos_frame.ds==0x1000 && far_get(dos_frame.ds,dos_frame.bx)==0xf0,"FAT pointer and geometry query");
        function(31);dos_dispatch();const u16 dpb=dos_frame.bx;
        require(fw(dos_frame.ds,static_cast<u16>(dpb+2))==512 && fw(dos_frame.ds,static_cast<u16>(dpb+6))==65 && fw(dos_frame.ds,static_cast<u16>(dpb+11))==97,"original drive parameter block layout");
        dos_frame.dx=2;function(28);dos_dispatch();require((dos_frame.ax&255)==255,"FAT query rejects unknown drive");
        for(unsigned fn:{24U,29U,30U,32U}) {function(fn,255);dos_dispatch();require((dos_frame.ax&255)==0,"original CP/M compatibility no-op returns zero");}
        function(0xfe);require(dos_dispatch()==1,"unsupported DOS call terminates explicitly");
        require(execute("TEST"," HELLO.TXT") && launches==1,"COM launch path");
        require(!execute("MISSING",""),"missing command rejected");
        require(!execute("HELLO.TXT","") && launches==1,"data files cannot be launched as COM programs");
        // Exercise the actual VGA renderer and editor rather than a UI mock.
        console_init();
        require(far_get(0xb800,4)=='>' && far_get(0xb800,5)==0x1b,"terminal header and accent");
        require(far_get(0xb800,(24*80+2)*2)=='A',"terminal status bar");
        for(unsigned n=0;n<60;n++) print("scrolling output\n");
        require(far_get(0xb800,4)=='>' && far_get(0xb800,(24*80+2)*2)=='A',"scrolling preserves chrome");
        command("CLS");require(console_row==2 && console_column==0 && far_get(0xb800,2*80*2)==' ',"CLS clears only terminal content");
        char edited[128];shell_history_count=0;shell_prompt();keys={'D','R',0x4b00,'I',0x4f00,'\r'};shell_input(edited,128);
        require(equal(edited,"DIR"),"insert and cursor navigation");
        keys={'T','Y','P','E',' ','H','E',9,'\r'};shell_input(edited,128);
        require(equal(edited,"TYPE HELLO.TXT"),"Tab completes filenames from FAT12");
        keys={'d','i',9,'\r'};shell_input(edited,128);require(equal(edited,"DIR"),"Tab completes commands case-insensitively");
        keys={'x',0x4800,0x4800,0x5000,0x5000,'\r'};shell_input(edited,128);require(equal(edited,"x"),"history restores the draft");
        keys={0x4800,'\r'};shell_input(edited,128);require(equal(edited,"x"),"history suppresses consecutive duplicates");
        require(shell_history_count==4,"history contains four distinct consecutive commands");
        keys={0x3b00};shell_input(edited,128);require(equal(edited,"HELP"),"F1 invokes help");
        keys={'a','b',0x4700,0x5300,'\r'};shell_input(edited,128);require(equal(edited,"b"),"Home and Delete edit the command");
        keys={'a',12,'b',27,'V','E','R','\r'};shell_input(edited,128);require(equal(edited,"VER"),"Ctrl+L preserves input and Esc clears it");
        keys.clear();for(unsigned n=0;n<150;n++) keys.push_back('X');keys.push_back('\r');shell_input(edited,128);
        require(edited[126]=='X' && !edited[127],"editor bounds long wrapped input");
        console_clear();console_row=22;console_column=75;print("A:\\> ");keys={'a','b','c','\b','\r'};shell_input(edited,128);
        require(equal(edited,"ab") && far_get(0xb800,4)=='>' && far_get(0xb800,(24*80+2)*2)=='A',"wrapped input editing preserves fixed bars");
        std::cout<<"Passed: kernel FAT12 reads/writes/persistence, FCB services, parser, clock, console, PSP, COM loading\n";
        return 0;
    }catch(const std::exception& e){std::cerr<<"FAILED: "<<e.what()<<'\n';return 1;}
}
