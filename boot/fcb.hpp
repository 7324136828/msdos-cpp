// DOS 1.25 FCB layout and record-position rules, from MSDOS.ASM FCBLOCK,
// FINSEQ/FINRND/FINBLK, FILESIZE, SRCHFRST/SAVPLCE and MAKEFCB.
static void al(u8 value) {dos_frame.ax=static_cast<u16>((dos_frame.ax&0xff00)|value);}
static u16 normal_fcb(u16 seg,u16 off) {
    return static_cast<u16>(off+(far_get(seg,off)==255 ? 7 : 0));
}
static bool fcb_name(u16 seg,u16 off,char* name,bool wildcard=false) {
    if(far_get(seg,off)>1) return false;
    for(u16 j=0;j<11;j++) {
        const char c=upper(static_cast<char>(far_get(seg,static_cast<u16>(off+1+j))&127));
        if(c!=' ' && !(wildcard && c=='?') && (!file_character(c) || c=='.')) return false;
        name[j]=c;
    }
    return name[0]!=' ';
}
static int fcb_find(u16 seg,u16 off) {
    char name[11];return fcb_name(seg,normal_fcb(seg,off),name) ? find(name) : -1;
}
static void fcb_metadata(u16 seg,u16 fcb,int index) {
    const u8* entry=root+index*32;
    sw(seg,static_cast<u16>(fcb+16),word(entry+28));sw(seg,static_cast<u16>(fcb+18),word(entry+30));
    sw(seg,static_cast<u16>(fcb+20),word(entry+24));sw(seg,static_cast<u16>(fcb+22),word(entry+22));
    far_set(seg,static_cast<u16>(fcb+24),0x40);
    sw(seg,static_cast<u16>(fcb+25),word(entry+26));sw(seg,static_cast<u16>(fcb+27),word(entry+26));
    sw(seg,static_cast<u16>(fcb+29),0);
}
static void open_fcb(bool create) {
    const u16 seg=dos_frame.ds,original=dos_frame.dx,fcb=normal_fcb(seg,original);
    char name[11];if(!fcb_name(seg,fcb,name)) {al(255);return;}
    const u8 device=named_device(name);
    if(device) {
        for(u16 j=12;j<32;j++) far_set(seg,static_cast<u16>(fcb+j),0);
        sw(seg,static_cast<u16>(fcb+14),128);far_set(seg,static_cast<u16>(fcb+24),256-device);
        al(0);return;
    }
    const int index=create ? create_file(name,original==fcb ? 0 : static_cast<u8>(far_get(seg,static_cast<u16>(original+6)))) : find(name);
    if(index<0) {al(255);return;}
    if(create) {stamp_file(index);if(!flush_volume()) {al(255);return;}}
    far_set(seg,fcb,1);sw(seg,static_cast<u16>(fcb+12),0);sw(seg,static_cast<u16>(fcb+14),128);
    fcb_metadata(seg,fcb,index);al(0);
}
static u32 sequential_record(u16 seg,u16 fcb) {
    return fw(seg,static_cast<u16>(fcb+12))*128UL+(far_get(seg,static_cast<u16>(fcb+32))&127);
}
static u32 random_record(u16 seg,u16 fcb,u16 size) {
    u32 value=fw(seg,static_cast<u16>(fcb+33))|(far_get(seg,static_cast<u16>(fcb+35))<<16);
    if(size<64) value|=far_get(seg,static_cast<u16>(fcb+36))<<24;
    return value;
}
static void set_random_record(u16 seg,u16 fcb,u16 size,u32 record) {
    sw(seg,static_cast<u16>(fcb+33),static_cast<u16>(record));far_set(seg,static_cast<u16>(fcb+35),record>>16);
    if(size<64) far_set(seg,static_cast<u16>(fcb+36),record>>24);
}
static void set_sequential_record(u16 seg,u16 fcb,u32 record) {
    sw(seg,static_cast<u16>(fcb+12),static_cast<u16>(record/128));far_set(seg,static_cast<u16>(fcb+32),record%128);
}
static u8 console_buffer[130];
static u16 console_position,console_length;
static int device_read(u8 device) {
    if(device==3 || device==4) return -1;
    if(device==2) return static_cast<u8>(bios_aux(2,0));
    if(console_position==console_length) {
        console_position=console_length=0;
        for(;;) {
            const u8 c=static_cast<u8>(bios_key());
            if(c=='\r' || c=='\n') {console_buffer[console_length++]='\r';console_buffer[console_length++]='\n';put('\n');break;}
            if(c=='\b' && console_length) {console_length--;print("\b \b");}
            else if((c>=32 || c==26) && console_length<128) {console_buffer[console_length++]=c;bios_put(c);}
        }
        if(console_buffer[0]==26) {console_position=console_length;return 26;}
    }
    return console_buffer[console_position++];
}
static void transfer_device(u8 device,u16 seg,u16 fcb,u16 size,u32 record,bool random,bool block,bool write) {
    const u16 requested=block ? dos_frame.cx : 1;u16 transferred=0;bool stopped=false;al(0);
    for(u16 r=0;r<requested;r++) {
        const u32 dest=dta_offset+static_cast<u32>(r)*size;
        if(dest+size>65536) {al(2);break;}
        if(write) {
            for(u16 j=0;j<size && !stopped;j++) {
                const u8 c=static_cast<u8>(far_get(dta_segment,static_cast<u16>(dest+j)));
                if(c==26) stopped=true;
                if(device==2) bios_aux(1,c);
                else if(!stopped && device==1) bios_put(c);
                else if(!stopped && device==4) bios_print(c);
            }
            far_set(seg,static_cast<u16>(fcb+24),256-device);transferred++;
        }else {
            if(!(far_get(seg,static_cast<u16>(fcb+24))&0x40)) {al(1);break;}
            u16 count=0;
            while(count<size) {
                const int c=device_read(device);
                if(c<0) {stopped=true;break;}
                far_set(dta_segment,static_cast<u16>(dest+count++),static_cast<u8>(c));
                if(c==26) {stopped=true;break;}
            }
            if(stopped) far_set(seg,static_cast<u16>(fcb+24),far_get(seg,static_cast<u16>(fcb+24))&~0x40U);
            if(!count) {al(1);break;}
            transferred++;
            if(count<size) {for(u16 j=count;j<size;j++) far_set(dta_segment,static_cast<u16>(dest+j),0);al(3);break;}
        }
    }
    const u32 next=record+((!random || block) ? transferred : 0);
    set_sequential_record(seg,fcb,next);if(random) set_random_record(seg,fcb,size,next);
    if(block) dos_frame.cx=transferred;
}
static void transfer_fcb(bool random,bool block,bool write) {
    const u16 seg=dos_frame.ds,fcb=normal_fcb(seg,dos_frame.dx);
    const int index=fcb_find(seg,dos_frame.dx);
    const u16 size=fw(seg,static_cast<u16>(fcb+14));
    char name[11];const u8 device=fcb_name(seg,fcb,name) ? named_device(name) : 0;
    if((index<0 && !device) || !size) {al(4);if(block) dos_frame.cx=0;return;}
    const u32 record=random ? random_record(seg,fcb,size) : sequential_record(seg,fcb);
    if(device) {transfer_device(device,seg,fcb,size,record,random,block,write);return;}
    const u16 requested=block ? dos_frame.cx : 1;
    u16 transferred=0;al(0);
    if(write && block && !requested) {
        if(record>cluster_count*512UL/size || !resize_file(index,record*size)) al(1);
        else {stamp_file(index);if(!flush_volume()) al(1);fcb_metadata(seg,fcb,index);}
    }
    for(u16 r=0;r<requested;r++) {
        const u32 file_size=longword(root+index*32+28),dest=dta_offset+static_cast<u32>(r)*size;
        // Compare before multiplying to prevent a four-byte RR from wrapping.
        const u32 limit=write ? cluster_count*512UL : file_size;
        if(record>limit/size || r>limit/size-record) {al(1);break;}
        if(dest+size>65536) {al(2);break;}
        const u32 position=(record+r)*size;
        if(write) {
            if(!write_file(index,position,dta_segment,static_cast<u16>(dest),size)) {al(1);break;}
            transferred++;
        }else {
            if(position>=file_size) {al(1);break;}
            const u16 count=static_cast<u16>(file_size-position<size ? file_size-position : size);
            if(!read_file(index,position,dta_segment,static_cast<u16>(dest),count)) {al(1);break;}
            for(u16 j=count;j<size;j++) far_set(dta_segment,static_cast<u16>(dest+j),0);
            transferred++;if(count<size) {al(3);break;}
        }
    }
    if(write && transferred) {stamp_file(index);if(!flush_volume()) al(1);fcb_metadata(seg,fcb,index);}
    // A single random transfer sets the sequential position to the record used.
    // Sequential and random block transfers point to the next record.
    const u32 next=record+((!random || block) ? transferred : 0);
    set_sequential_record(seg,fcb,next);
    if(random) set_random_record(seg,fcb,size,next);
    if(block) dos_frame.cx=transferred;
}
static bool search_match(const char* pattern,int index,u8 attributes) {
    const u8* entry=root+index*32;
    if(!entry[0] || entry[0]==0xe5 || (entry[11]&0x18) || ((entry[11]&6)&~attributes)) return false;
    for(u16 j=0;j<11;j++) if(pattern[j]!='?' && entry[j]!=static_cast<u8>(pattern[j])) return false;
    return true;
}
static void search_fcb(bool first) {
    const u16 seg=dos_frame.ds,original=dos_frame.dx,fcb=normal_fcb(seg,original);
    char pattern[11];if(!fcb_name(seg,fcb,pattern,true)) {al(255);return;}
    const u8 attributes=original==fcb ? 0 : static_cast<u8>(far_get(seg,static_cast<u16>(original+6)));
    if(!first && fw(seg,static_cast<u16>(fcb+16))==0xffff) {al(255);return;}
    const u16 start=first ? 0 : static_cast<u16>(fw(seg,static_cast<u16>(fcb+16))+1);
    int index=start;for(;index<224;index++) if(search_match(pattern,index,attributes)) break;
    if(index>=224) {sw(seg,static_cast<u16>(fcb+16),0xffff);al(255);return;}
    const u16 prefix=original==fcb ? 0 : 7;
    if(static_cast<u32>(dta_offset)+prefix+33>65536) {al(255);return;}
    if(prefix) {
        for(u16 j=0;j<7;j++) far_set(dta_segment,static_cast<u16>(dta_offset+j),j==0 ? 255 : j==6 ? attributes : 0);
    }
    far_set(dta_segment,static_cast<u16>(dta_offset+prefix),1);
    for(u16 j=0;j<32;j++) far_set(dta_segment,static_cast<u16>(dta_offset+prefix+1+j),root[index*32+j]);
    sw(seg,static_cast<u16>(fcb+16),static_cast<u16>(index));al(0);
}
static void delete_fcb() {
    const u16 seg=dos_frame.ds,original=dos_frame.dx,fcb=normal_fcb(seg,original);
    char pattern[11];if(!fcb_name(seg,fcb,pattern,true)) {al(255);return;}
    const u8 attributes=original==fcb ? 0 : static_cast<u8>(far_get(seg,static_cast<u16>(original+6)));
    bool found=false;
    // Preflight all matching chains before changing directory entries.
    for(int index=0;index<224;index++) if(search_match(pattern,index,attributes)) {
        u16 count,tail;if(!writable_entry(index) || !chain_info(word(root+index*32+26),count,tail)) {al(255);return;}
        found=true;
    }
    if(!found) {al(255);return;}
    for(int index=0;index<224;index++) if(search_match(pattern,index,attributes)) delete_file(index);
    al(flush_volume() ? 0 : 255);
}
static void rename_fcb() {
    const u16 seg=dos_frame.ds,fcb=normal_fcb(seg,dos_frame.dx);
    char from[11],to[11];
    if(!fcb_name(seg,fcb,from,true) || !fcb_name(seg,static_cast<u16>(fcb+16),to,true)) {al(255);return;}
    bool found=false;
    for(int index=0;index<224;index++) if(search_match(from,index,0)) {
        char target[11];for(u16 j=0;j<11;j++) target[j]=to[j]=='?' ? static_cast<char>(root[index*32+j]) : to[j];
        const int exists=find(target);
        if(!writable_entry(index) || (exists>=0 && exists!=index)) {al(255);return;}
        for(int other=0;other<index;other++) if(search_match(from,other,0)) {
            bool same=true;for(u16 j=0;j<11;j++) if(target[j]!=(to[j]=='?' ? static_cast<char>(root[other*32+j]) : to[j])) same=false;
            if(same) {al(255);return;}
        }
        found=true;
    }
    if(!found) {al(255);return;}
    // Mark matches first: changing a name must not change the wildcard selection.
    u8 selected[28]{};
    for(int index=0;index<224;index++) if(search_match(from,index,0)) selected[index/8]|=static_cast<u8>(1U<<(index%8));
    for(int index=0;index<224;index++) if(selected[index/8]&(1U<<(index%8))) {
        for(u16 j=0;j<11;j++) if(to[j]!='?') root[index*32+j]=static_cast<u8>(to[j]);
        root_changed(index);
    }
    al(flush_volume() ? 0 : 255);
}
static bool parse_delimiter(u8 c) {
    return c<=32 || c=='.' || c=='"' || c=='/' || c=='[' || c==']' || c==':' ||
        c=='+' || c=='=' || c==';' || c==',';
}
static void parse_fcb() {
    const u16 seg=dos_frame.ds,out_seg=dos_frame.es,out=dos_frame.di;
    u16 pos=dos_frame.si;const u8 flags=static_cast<u8>(dos_frame.ax);
    if(!(flags&2)) far_set(out_seg,out,0);
    if(!(flags&4)) for(u16 j=1;j<=8;j++) far_set(out_seg,static_cast<u16>(out+j),' ');
    if(!(flags&8)) for(u16 j=9;j<=11;j++) far_set(out_seg,static_cast<u16>(out+j),' ');
    sw(out_seg,static_cast<u16>(out+12),0);sw(out_seg,static_cast<u16>(out+14),0);
    auto ch=[&]() {return static_cast<u8>(far_get(seg,pos)&127);};
    auto spaces=[&]() {u32 n=0;while(n++<65536 && (ch()==' ' || ch()==9)) pos++;};
    spaces();
    if((flags&1) && (ch()=='+' || ch()=='=' || ch()==';' || ch()==',' || ch()==':')) {pos++;spaces();}
    u8 result=0;
    if(!parse_delimiter(ch()) && far_get(seg,static_cast<u16>(pos+1))==':') {
        const u8 drive=static_cast<u8>(upper(static_cast<char>(ch()))-'@');
        far_set(out_seg,out,drive);if(drive!=1) result=255;pos=static_cast<u16>(pos+2);
    }
    for(u16 part=0;part<2;part++) {
        const u16 width=part ? 3 : 8,start=part ? 9 : 1;u16 count=0;bool star=false;
        if(part) {if(ch()!='.') break;pos++;}
        if(!parse_delimiter(ch()) || part) {
            for(u16 j=0;j<width;j++) far_set(out_seg,static_cast<u16>(out+start+j),' ');
            for(u32 scanned=0;scanned<65536 && !parse_delimiter(ch());scanned++,pos++) {
                const u8 c=static_cast<u8>(upper(static_cast<char>(ch())));
                if(count>=width || star) continue;
                if(c=='*') {while(count<width) far_set(out_seg,static_cast<u16>(out+start+count++),'?');star=true;if(result!=255) result=1;}
                else {far_set(out_seg,static_cast<u16>(out+start+count++),c);if(c=='?' && result!=255) result=1;}
            }
        }
    }
    dos_frame.si=pos;al(result);
}
