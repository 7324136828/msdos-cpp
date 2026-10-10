// Session clock driven by the BIOS tick counter, initially seeded from the RTC.
// Date validation and weekday bias follow GET/SETDATE in v1.25/MSDOS.ASM.
static constexpr u32 ticks_per_day=0x1800b0, hundredths_per_day=8640000;
static u16 clock_year=1982;
static u8 clock_month=3,clock_day=3;
static u32 clock_base,clock_anchor;
static bool leap(u16 year) {return !(year%4) && ((year%100)!=0 || !(year%400));}
static u8 month_days(u16 year,u8 month) {
    const u8 days[]={31,28,31,30,31,30,31,31,30,31,30,31};
    return static_cast<u8>(days[month-1]+(month==2 && leap(year)));
}
static u32 ticks() {
    u16 high,low;do {high=fw(0,0x46e);low=fw(0,0x46c);}while(high!=fw(0,0x46e));
    return (low|(static_cast<u32>(high)<<16))%ticks_per_day;
}
// Exact bounded 32-bit multiply/divide without a freestanding 64-bit runtime.
static u32 tick_hundredths(u32 value) {
    u32 result=0,remainder=0;
    for(int bit=31;bit>=0;bit--) {
        remainder=remainder*2+((value&(1UL<<bit)) ? hundredths_per_day : 0);
        result=result*2+remainder/ticks_per_day;remainder%=ticks_per_day;
    }
    return result;
}
static void next_day() {
    if(++clock_day>month_days(clock_year,clock_month)) {
        clock_day=1;if(++clock_month>12) {clock_month=1;clock_year++;}
    }
}
static u32 clock_now() {
    const u32 now=ticks(),elapsed=now>=clock_anchor ? now-clock_anchor : now+ticks_per_day-clock_anchor;
    u32 value=clock_base+tick_hundredths(elapsed);
    if(value>=hundredths_per_day) {
        next_day();value-=hundredths_per_day;clock_anchor=now;clock_base=value;
    }
    return value;
}
static u8 weekday() {
    u32 days=2;for(u16 year=1980;year<clock_year;year++) days+=leap(year) ? 366 : 365;
    for(u8 month=1;month<clock_month;month++) days+=month_days(clock_year,month);
    return static_cast<u8>((days+clock_day-1)%7);
}
static u8 bcd(u8 value) {return static_cast<u8>((value>>4)*10+(value&15));}
static bool set_date(u16 year,u8 month,u8 day) {
    if(year<1980 || year>2099 || !month || month>12 || !day || day>month_days(year,month)) return false;
    clock_now();clock_year=year;clock_month=month;clock_day=day;return true;
}
static bool set_time(u8 hour,u8 minute,u8 second,u8 hundredth) {
    if(hour>23 || minute>59 || second>59 || hundredth>99) return false;
    clock_now();clock_anchor=ticks();clock_base=((hour*60UL+minute)*60+second)*100+hundredth;return true;
}
static void init_clock() {
    const u32 date=bios_clock(1),time=bios_clock(0);
    if(date) set_date(static_cast<u16>(bcd(static_cast<u8>(date>>24))*100+bcd(static_cast<u8>(date>>16))),
                      bcd(static_cast<u8>(date>>8)),bcd(static_cast<u8>(date)));
    if(time) set_time(bcd(static_cast<u8>(time>>24)),bcd(static_cast<u8>(time>>16)),bcd(static_cast<u8>(time>>8)),0);
    clock_anchor=ticks();
}
static void stamp_file(int index) {
    const u32 now=clock_now()/100;
    store_word(root+index*32+22,static_cast<u16>(((now/3600)<<11)|(((now/60)%60)<<5)|((now%60)/2)));
    store_word(root+index*32+24,static_cast<u16>(((clock_year-1980)<<9)|(clock_month<<5)|clock_day));
    root_changed(index);
}
