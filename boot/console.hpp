// VGA text terminal. Serial output remains plain text for DOS tools and tests.
static constexpr u8 text_normal=0x07,text_muted=0x08,text_blue=0x09,
    text_green=0x0a,text_accent=0x0b,text_error=0x0c,text_warning=0x0e;
static constexpr u16 console_columns=80,console_first_row=2,console_last_row=22;
static u8 console_attribute=text_normal;
static u16 console_row=console_first_row,console_column;
static bool console_ready;
static void console_cell(u16 row,u16 column,u8 character,u8 attribute) {
    const u16 offset=static_cast<u16>((row*console_columns+column)*2);
    far_set(0xb800,offset,character);far_set(0xb800,static_cast<u16>(offset+1),attribute);
}
static void console_row_text(u16 row,u16 column,const char* text,u8 attribute) {
    while(*text && column<console_columns) console_cell(row,column++,static_cast<u8>(*text++),attribute);
}
static void console_cursor() {bios_video(1,(console_row<<8)|console_column);}
static void console_chrome() {
    for(u16 column=0;column<console_columns;column++) {
        console_cell(0,column,' ',0x17);console_cell(24,column,' ',0x18);
    }
    console_row_text(0,2,">_",0x1b);
    console_row_text(0,6,"MS-DOS cpp",0x17);
    console_row_text(0,56,"TERMINAL  /  DOS 1.25",0x18);
    console_row_text(24,2,"A:\\",0x1a);
    console_row_text(24,8,"FAT12",0x17);
    console_row_text(24,17,"F1 Help   Up/Down History   Tab Complete   Ctrl+L Clear",0x18);
}
static void console_clear() {
    for(u16 row=1;row<24;row++) for(u16 column=0;column<console_columns;column++)
        console_cell(row,column,' ',text_normal);
    console_row=console_first_row;console_column=0;console_attribute=text_normal;
    console_chrome();console_cursor();
}
static void console_init() {
    bios_video(0,0);console_ready=true;console_clear();
}
static void console_scroll() {
    for(u16 row=console_first_row;row<console_last_row;row++) for(u16 column=0;column<console_columns;column++) {
        const u16 offset=static_cast<u16>(((row+1)*console_columns+column)*2);
        console_cell(row,column,static_cast<u8>(far_get(0xb800,offset)),static_cast<u8>(far_get(0xb800,offset+1)));
    }
    for(u16 column=0;column<console_columns;column++) console_cell(console_last_row,column,' ',text_normal);
    console_row=console_last_row;
}
extern "C" void console_write(u32 value) {
    const u8 character=static_cast<u8>(value);
    if(!console_ready) return;
    if(character=='\r') console_column=0;
    else if(character=='\n') console_row++;
    else if(character=='\b') {
        if(console_column) console_column--;
        else if(console_row>console_first_row) {console_row--;console_column=console_columns-1;}
    }
    else if(character=='\t') {
        const u16 spaces=static_cast<u16>(8-console_column%8);
        for(u16 n=0;n<spaces;n++) console_write(' ');
        return;
    }
    else if(character>=32) {
        console_cell(console_row,console_column,character,console_attribute);
        if(++console_column==console_columns) {console_column=0;console_row++;}
    }
    if(console_row>console_last_row) console_scroll();
    console_cursor();
}
static void console_color(u8 attribute) {console_attribute=attribute;}
static void console_input_bar() {
    for(u16 column=0;column<console_columns;column++) console_cell(console_row,column,' ',0x17);
    console_column=0;console_cursor();
}
