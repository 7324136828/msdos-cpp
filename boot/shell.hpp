// C++ command processor. These implement selected COMMAND 1.17 contracts;
// this is not a source-complete translation of reference COMMAND.ASM.
static u8 transfer_buffer[512];
static void number(u32 n,u16 width=0) {
    char digits[11];u16 length=0;do {digits[length++]=static_cast<char>('0'+n%10);n/=10;}while(n);
    while(width>length) {put(' ');width--;}
    while(length) put(digits[--length]);
}
static void two_digits(u16 n) {put(static_cast<char>('0'+n/10));put(static_cast<char>('0'+n%10));}
static void print_name(const u8* entry) {
    for(u16 j=0;j<8;j++) if(entry[j]!=' ') put(static_cast<char>(entry[j]));
    if(entry[8]!=' ') {put('.');for(u16 j=8;j<11;j++) if(entry[j]!=' ') put(static_cast<char>(entry[j]));}
}
static const char* const shell_commands[]={"HELP","DIR","TYPE","COPY","DEL","ERASE",
    "REN","RENAME","REM","PAUSE","DATE","TIME","VERIFY","CLS","CLEAR","VER","HISTORY"};
static char shell_history[8][128];
static u16 shell_history_count;
static void shell_prompt() {
    console_input_bar();styled("A:\\",0x10|text_green);styled("> ",0x10|text_accent);console_color(0x17);
}
static u16 line_copy(char* destination,const char* source,u16 capacity) {
    u16 length=0;while(source[length] && length+1<capacity) {destination[length]=source[length];length++;}
    destination[length]=0;return length;
}
static void shell_remember(const char* line) {
    if(!*line || (shell_history_count && equal(line,shell_history[shell_history_count-1]))) return;
    if(shell_history_count==8) {
        for(u16 n=1;n<8;n++) line_copy(shell_history[n-1],shell_history[n],128);
        shell_history_count--;
    }
    line_copy(shell_history[shell_history_count++],line,128);
}
static void completion_match(const char* candidate,const char* prefix,u16 length,char* common,u16& matches) {
    for(u16 n=0;n<length;n++) if(!candidate[n] || upper(candidate[n])!=upper(prefix[n])) return;
    if(!matches++) line_copy(common,candidate,13);
    else {u16 n=0;while(common[n] && common[n]==candidate[n]) n++;common[n]=0;}
}
static void shell_complete(char* line,u16& length,u16 capacity) {
    u16 start=length;while(start && line[start-1]!=' ' && line[start-1]!='\t') start--;
    // Optional A: qualifier is retained; there are no subdirectories yet.
    u16 prefix=start;if(length>=start+2 && upper(line[start])=='A' && line[start+1]==':') prefix+=2;
    char common[13]{};u16 matches=0;
    if(!start && !prefix) for(const char* candidate:shell_commands)
        completion_match(candidate,line,length,common,matches);
    for(int index=0;index<224;index++) {
        const u8* entry=root+index*32;
        if(!entry[0]) break;
        if(entry[0]==0xe5 || (entry[11]&0x18)) continue;
        if(!start && !((entry[8]=='C' && entry[9]=='O' && entry[10]=='M') ||
            (entry[8]=='B' && entry[9]=='A' && entry[10]=='T'))) continue;
        char name[13];u16 n=0;
        for(u16 j=0;j<8;j++) if(entry[j]!=' ') name[n++]=static_cast<char>(entry[j]);
        if(entry[8]!=' ') {name[n++]='.';for(u16 j=8;j<11;j++) if(entry[j]!=' ') name[n++]=static_cast<char>(entry[j]);}
        name[n]=0;completion_match(name,line+prefix,static_cast<u16>(length-prefix),common,matches);
    }
    if(matches) length=static_cast<u16>(prefix+line_copy(line+prefix,common,static_cast<u16>(capacity-prefix)));
}
static void shell_input(char* line,u16 capacity) {
    if(!capacity) return;
    if(capacity>128) capacity=128;
    u16 length=0,cursor=0,history=shell_history_count;char draft[128]{};line[0]=0;
    for(;;) {
        const u32 key=bios_key();const u8 character=static_cast<u8>(key),scan=static_cast<u8>(key>>8);
        if(character=='\r' || character=='\n') {
            while(cursor<length) put(line[cursor++]);
            put('\n');console_color(text_normal);shell_remember(line);return;
        }
        if((character==0 || character==0xe0) && scan==0x4b) {if(cursor) {put('\b');cursor--;}continue;}
        if((character==0 || character==0xe0) && scan==0x4d) {if(cursor<length) put(line[cursor++]);continue;}
        if((character==0 || character==0xe0) && scan==0x47) {while(cursor) {put('\b');cursor--;}continue;}
        if((character==0 || character==0xe0) && scan==0x4f) {while(cursor<length) put(line[cursor++]);continue;}
        if(character==12) {console_clear();shell_prompt();print(line);for(u16 n=cursor;n<length;n++) put('\b');continue;}
        if(character>=32 && character<127 && cursor==length && length+1<capacity) {
            line[length++]=static_cast<char>(character);line[length]=0;cursor=length;put(static_cast<char>(character));continue;
        }
        if((character=='\b' || character==127) && cursor==length && cursor) {
            line[--length]=0;cursor=length;print("\b \b");continue;
        }
        // Redraw from the prompt after edits, including lines wrapped across rows.
        const u16 previous_length=length,previous_cursor=cursor;
        char previous[128];line_copy(previous,line,128);
        if((character==0 || character==0xe0) && (scan==0x48 || scan==0x50)) {
            if(history==shell_history_count) line_copy(draft,line,128);
            if(scan==0x48 && history) history--;
            if(scan==0x50 && history<shell_history_count) history++;
            length=line_copy(line,history==shell_history_count ? draft : shell_history[history],capacity);cursor=length;
        }else if((character==0 || character==0xe0) && scan==0x3b) {
            length=line_copy(line,"HELP",capacity);cursor=length;
        }else if(character==9) {
            if(cursor!=length) continue;
            shell_complete(line,length,capacity);cursor=length;
        }else if(character==27 || character==21 || character==3) {length=cursor=0;line[0]=0;}
        else if(character=='\b' || character==127) {
            if(!cursor) continue;
            for(u16 n=cursor;n<=length;n++) line[n-1]=line[n];
            length--;cursor--;
        }else if((character==0 || character==0xe0) && scan==0x53) {
            if(cursor==length) continue;
            for(u16 n=cursor;n<length;n++) line[n]=line[n+1];
            length--;
        }else if(character>=32 && character<127 && length+1<capacity) {
            for(u16 n=length+1;n>cursor;n--) line[n]=line[n-1];
            line[cursor++]=static_cast<char>(character);length++;
        }else continue;
        for(u16 n=previous_cursor;n<previous_length;n++) put(previous[n]);
        for(u16 n=0;n<previous_length;n++) print("\b \b");
        print(line);for(u16 n=cursor;n<length;n++) put('\b');
        if((character==0 || character==0xe0) && scan==0x3b) {put('\n');console_color(text_normal);shell_remember(line);return;}
    }
}
static void shell_help() {
    styled("Commands\n",text_accent);
    print("  DIR [/W] [/P] [pattern]  List files      TYPE file   Read a file\n");
    print("  COPY source destination                REN old new   Rename\n");
    print("  DEL / ERASE file                       REM           Comment\n");
    print("  DATE [YYYY-MM-DD]   TIME [HH:MM:SS]     VERIFY [ON|OFF]\n");
    print("  CLS / CLEAR        Clear screen        HISTORY       Recent commands\n");
    print("  VER                DOS version         PAUSE         Wait for a key\n");
    styled("\nKeyboard\n",text_accent);
    print("  Up/Down history   Tab completion   Left/Right, Home/End, Delete\n");
    print("  F1 help   Ctrl+L clear screen   Esc / Ctrl+U / Ctrl+C clear input\n");
    styled("\nTry it\n",text_accent);
    print("  DEMO README.TXT   Run a COM program. BAT files accept %0-%9.\n");
}
static bool shell_name(const char* text,char* name,bool wildcard=false) {
    if(text[0] && text[1]==':') {if(upper(text[0])!='A') return false;text+=2;}
    if(!wildcard) return name83(text,name);
    for(u16 j=0;j<11;j++) name[j]=' ';
    u16 part=0,count=0;bool star=false,any=false;
    for(;*text;text++) {
        if(*text=='.') {if(part) return false;part=1;count=0;star=false;continue;}
        const u16 width=part ? 3 : 8,start=part ? 8 : 0;
        if(*text=='*') {while(count<width) name[start+count++]='?';star=true;any=true;continue;}
        if(!file_character(*text) && *text!='?') return false;
        if(!star) {if(count>=width) return false;name[start+count++]=upper(*text);}
        any=true;
    }
    return any && name[0]!=' ';
}
static char* token(char*& p) {
    while(*p==' ' || *p=='\t') p++;
    char* start=p;while(*p && *p!=' ' && *p!='\t') p++;
    if(*p) *p++=0;
    return start;
}
static bool no_more(char* p) {while(*p==' ' || *p=='\t') p++;return !*p;}
static void directory(char* tail) {
    char pattern[11];for(u16 j=0;j<11;j++) pattern[j]='?';
    bool wide=false,page=false,named=false;
    while(!no_more(tail)) {
        char* arg=token(tail);
        if(arg[0]=='/' && !arg[2] && upper(arg[1])=='W') wide=true;
        else if(arg[0]=='/' && !arg[2] && upper(arg[1])=='P') page=true;
        else if(named || !shell_name(arg,pattern,true)) {print("Invalid DIR arguments\n");return;}
        else named=true;
    }
    styled("\n  Directory: A:\\\n\n",text_accent);
    if(!wide) styled("  Name                Bytes   Modified\n  ----------------  --------  ----------------\n",text_muted);
    u16 count=0,rows=0;
    for(int index=0;index<224;index++) if(search_match(pattern,index,0)) {
        const u8* entry=root+index*32;count++;
        if(!wide || count%5==1) print("  ");
        console_color(entry[8]=='C' && entry[9]=='O' && entry[10]=='M' ? text_blue : text_normal);
        print_name(entry);console_color(text_normal);
        if(wide) {
            u16 length=0;for(u16 j=0;j<11;j++) if(entry[j]!=' ') length++;
            if(entry[8]!=' ') length++;
            while(length++<15) put(' ');
            if(count%5) continue;
        }else {
            u16 length=0;for(u16 j=0;j<11;j++) if(entry[j]!=' ') length++;
            if(entry[8]!=' ') length++;
            while(length++<16) put(' ');
            number(longword(entry+28),8);print("  ");console_color(text_muted);
            const u16 date=word(entry+24),time=word(entry+22);
            two_digits(static_cast<u16>((date>>5)&15));put('-');two_digits(date&31);put('-');number(1980+(date>>9));put(' ');
            two_digits(time>>11);put(':');two_digits(static_cast<u16>((time>>5)&63));
        }
        console_color(text_normal);put('\n');if(page && ++rows==14) {styled("Press a key to continue\n",text_muted);bios_key();rows=0;}
    }
    if(wide && count%5) put('\n');
    console_color(text_muted);print("\n  ");number(count);print(" File(s)  |  ");number(free_clusters()*512UL);print(" bytes free\n");console_color(text_normal);
}
static bool type_file(const char* text) {
    char name[11];const int index=shell_name(text,name) ? find(name) : -1;
    if(index<0) {styled("File not found\n",text_error);return false;}
    const u32 size=longword(root+index*32+28);
    for(u32 position=0;position<size;) {
        const u16 count=static_cast<u16>(size-position>512 ? 512 : size-position);
        if(!read_file(index,position,0x1000,pointer(transfer_buffer),count)) {print("Disk read failed\n");return false;}
        for(u16 j=0;j<count;j++) {if(transfer_buffer[j]==0x1a) return true;bios_put(transfer_buffer[j]);}
        position+=count;
    }
    return true;
}
static bool copy_file(int source,const char* name) {
    if(find(name)==source) {print("File cannot be copied onto itself\n");return false;}
    const int target=create_file(name);
    if(target<0) {print("File creation error\n");return false;}
    const u32 size=longword(root+source*32+28);
    for(u32 position=0;position<size;) {
        const u16 count=static_cast<u16>(size-position>512 ? 512 : size-position);
        if(!read_file(source,position,0x1000,pointer(transfer_buffer),count) ||
           !write_file(target,position,0x1000,pointer(transfer_buffer),count)) {
            flush_volume();print("File copy failed\n");return false;
        }
        position+=count;
    }
    stamp_file(target);if(!flush_volume()) {print("Disk write failed\n");return false;}return true;
}
static void copy_command(char* tail) {
    const char* source=token(tail),*destination=token(tail);char from[11],to[11];
    if(!*source || !*destination || !no_more(tail) || !shell_name(source,from,true) || !shell_name(destination,to,true)) {
        print("Usage: COPY source destination (8.3 names, ? and * supported)\n");return;
    }
    u8 selected[28]{};u16 matches=0;
    for(int index=0;index<224;index++) if(search_match(from,index,0)) {selected[index/8]|=static_cast<u8>(1U<<(index%8));matches++;}
    bool wildcard=false;for(u16 j=0;j<11;j++) if(to[j]=='?') wildcard=true;
    if(matches>1 && !wildcard) {print("Multiple sources require a wildcard destination\n");return;}
    for(int index=0;index<224;index++) if(selected[index/8]&(1U<<(index%8))) {
        char name[11];for(u16 j=0;j<11;j++) name[j]=to[j]=='?' ? static_cast<char>(root[index*32+j]) : to[j];
        const int existing=find(name);
        if(existing>=0 && (selected[existing/8]&(1U<<(existing%8)))) {print("Copy destination overlaps a source\n");return;}
    }
    u16 copied=0;
    for(int index=0;index<224;index++) if(selected[index/8]&(1U<<(index%8))) {
        char name[11];for(u16 j=0;j<11;j++) name[j]=to[j]=='?' ? static_cast<char>(root[index*32+j]) : to[j];
        if(copy_file(index,name)) copied++;
    }
    console_color(text_green);number(copied);print(" File(s) copied\n");console_color(text_normal);
}
static void mutation_command(char* tail,bool rename) {
    char* source=token(tail);char* destination=rename ? token(tail) : nullptr;
    char from[11],to[11];
    if(!*source || (rename && !*destination) || !no_more(tail) || !shell_name(source,from,true) || (rename && !shell_name(destination,to,true))) {
        print(rename ? "Usage: REN source destination\n" : "Usage: DEL filename\n");return;
    }
    // Use a private FCB in a separate segment, preserving application/DTA state.
    const Frame saved=dos_frame;const u16 saved_segment=dta_segment,saved_offset=dta_offset;
    for(u16 j=0;j<37;j++) far_set(0x3000,j,0);
    for(u16 j=0;j<11;j++) far_set(0x3000,static_cast<u16>(1+j),static_cast<u8>(from[j]));
    if(rename) for(u16 j=0;j<11;j++) far_set(0x3000,static_cast<u16>(17+j),static_cast<u8>(to[j]));
    dos_frame.ds=0x3000;dos_frame.dx=0;
    if(rename) rename_fcb();else delete_fcb();
    const bool success=(dos_frame.ax&255)==0;dos_frame=saved;dta_segment=saved_segment;dta_offset=saved_offset;
    if(!success) print("File operation failed\n");
}
static bool numeric_field(const char*& p,u16& value,char separator) {
    value=0;u16 count=0;
    while(*p>='0' && *p<='9') {if(++count>4) return false;value=static_cast<u16>(value*10+*p++-'0');}
    if(!count || *p!=separator) return false;
    if(separator) p++;
    return true;
}
static void date_command(char* tail) {
    while(*tail==' ') tail++;
    if(*tail) {
        const char* p=tail;u16 year,month,day;
        if(!numeric_field(p,year,'-') || !numeric_field(p,month,'-') || !numeric_field(p,day,0) ||
           month>12 || day>31 || !set_date(year,static_cast<u8>(month),static_cast<u8>(day))) {print("Invalid date; use YYYY-MM-DD\n");return;}
    }
    clock_now();number(clock_year);put('-');two_digits(clock_month);put('-');two_digits(clock_day);put('\n');
}
static void time_command(char* tail) {
    while(*tail==' ') tail++;
    if(*tail) {
        const char* p=tail;u16 hour,minute,second;
        if(!numeric_field(p,hour,':') || !numeric_field(p,minute,':') || !numeric_field(p,second,0) ||
           hour>23 || minute>59 || second>59 || !set_time(static_cast<u8>(hour),static_cast<u8>(minute),static_cast<u8>(second),0)) {print("Invalid time; use HH:MM:SS\n");return;}
    }
    const u32 now=clock_now()/100;two_digits(static_cast<u16>(now/3600));put(':');two_digits(static_cast<u16>((now/60)%60));put(':');two_digits(static_cast<u16>(now%60));put('\n');
}
static u8 batch_depth;
static bool command_line(char*);
static bool run_batch(const char* command,const char* tail) {
    char name[11];if(!shell_name(command,name)) return false;
    if(name[8]==' ') {name[8]='B';name[9]='A';name[10]='T';}
    if(name[8]!='B' || name[9]!='A' || name[10]!='T') return false;
    const int index=find(name);if(index<0) return false;
    if(batch_depth>=4) {print("Batch nesting limit reached\n");return true;}
    char arguments[10][128]{};u16 n=0;
    while(command[n] && n<127) {arguments[0][n]=command[n];n++;}
    for(u16 arg=1;arg<10;arg++) {
        while(*tail==' ' || *tail=='\t') tail++;
        n=0;
        while(*tail && *tail!=' ' && *tail!='\t') {if(n<127) arguments[arg][n++]=*tail;tail++;}
    }
    batch_depth++;const u32 size=longword(root+index*32+28);u32 position=0;
    while(position<size) {
        char line[128];u16 length=0;bool truncated=false;
        while(position<size) {
            if(!read_file(index,position++,0x1000,pointer(transfer_buffer),1)) {print("Batch read failed\n");batch_depth--;return true;}
            const char c=static_cast<char>(transfer_buffer[0]);
            if(c==0x1a) {position=size;break;}if(c=='\r') continue;if(c=='\n') break;
            if(c=='%' && position<size) {
                if(!read_file(index,position,0x1000,pointer(transfer_buffer),1)) {batch_depth--;return true;}
                const char next=static_cast<char>(transfer_buffer[0]);
                if(next>='0' && next<='9') {
                    position++;for(const char* arg=arguments[next-'0'];*arg;arg++) {if(length<127) line[length++]=*arg;else truncated=true;}continue;
                }
                if(next=='%') position++;
            }
            if(length<127) line[length++]=c;else truncated=true;
        }
        line[length]=0;if(truncated) {print("Batch line too long\n");continue;}
        if(length) command_line(line);
    }
    batch_depth--;return true;
}
static bool command_line(char* line) {
    char* tail=line;char* command=token(tail);for(char* p=command;*p;p++) *p=upper(*p);
    if(!*command || equal(command,"REM")) return true;
    if(equal(command,"HELP")) shell_help();
    else if(equal(command,"CLS") || equal(command,"CLEAR")) console_clear();
    else if(equal(command,"VER")) {styled("MS-DOS cpp",text_accent);print("  DOS 1.25 compatibility\n");}
    else if(equal(command,"HISTORY")) {
        for(u16 n=0;n<shell_history_count;n++) {console_color(text_muted);number(n+1,3);print("  ");console_color(text_normal);print(shell_history[n]);put('\n');}
    }
    else if(equal(command,"DIR")) directory(tail);
    else if(equal(command,"TYPE")) {char* name=token(tail);if(!*name || !no_more(tail)) print("Usage: TYPE filename\n");else {type_file(name);put('\n');}}
    else if(equal(command,"COPY")) copy_command(tail);
    else if(equal(command,"DEL") || equal(command,"ERASE")) mutation_command(tail,false);
    else if(equal(command,"REN") || equal(command,"RENAME")) mutation_command(tail,true);
    else if(equal(command,"PAUSE")) {print("Press a key to continue\n");bios_key();}
    else if(equal(command,"DATE")) date_command(tail);
    else if(equal(command,"TIME")) time_command(tail);
    else if(equal(command,"VERIFY")) {
        char* value=token(tail);for(char* p=value;*p;p++) *p=upper(*p);
        if(!no_more(tail) || (*value && !equal(value,"ON") && !equal(value,"OFF"))) print("Usage: VERIFY [ON|OFF]\n");
        else {if(*value) verify_writes=equal(value,"ON");print(verify_writes ? "Verify is ON\n" : "Verify is OFF\n");}
    }
    else if(!execute(command,tail) && !run_batch(command,tail)) {styled("Bad command or file name",text_error);styled(". Type HELP for commands.\n",text_muted);return false;}
    return true;
}
