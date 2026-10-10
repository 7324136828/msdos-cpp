// FAT12 root-directory storage for the generated floppy geometry.
// PACK/UNPACK follow v1.25/source/MSDOS.ASM; BIOS I/O is the native boundary.
static constexpr u16 cluster_limit = 2785, cluster_count = cluster_limit-2;
static u16 dirty_fat, dirty_root;
static bool verify_writes;
static u8 verify_sector[512];
static void store_word(u8* p,u16 v) {p[0]=static_cast<u8>(v);p[1]=static_cast<u8>(v>>8);}
static void store_long(u8* p,u32 v) {store_word(p,static_cast<u16>(v));store_word(p+2,static_cast<u16>(v>>16));}
static bool valid_cluster(u16 n) {return n>=2 && n<cluster_limit;}
static void pack_cluster(u16 n,u16 value) {
    const u16 p=static_cast<u16>(n+n/2), old=word(fat+p);
    store_word(fat+p,static_cast<u16>((n&1) ? (old&15)|((value&0xfff)<<4) : (old&0xf000)|(value&0xfff)));
    dirty_fat|=static_cast<u16>((1U<<(p/512))|(1U<<((p+1)/512)));
}
static bool write_sector(u16 lba) {
    if(verify_writes) for(u16 j=0;j<512;j++) verify_sector[j]=sector[j];
    if(!bios_write(lba,0x1000,pointer(sector))) return false;
    if(verify_writes) {
        if(!read_sector(lba)) return false;
        for(u16 j=0;j<512;j++) if(sector[j]!=verify_sector[j]) return false;
    }
    return true;
}
static bool flush_volume() {
    for(u16 i=0;i<9;i++) if(dirty_fat&(1U<<i)) {
        for(u16 copy=0;copy<2;copy++) {
            for(u16 j=0;j<512;j++) sector[j]=fat[i*512+j];
            if(!write_sector(static_cast<u16>(reserved+copy*9+i))) return false;
        }
        dirty_fat&=static_cast<u16>(~(1U<<i));
    }
    for(u16 i=0;i<14;i++) if(dirty_root&(1U<<i)) {
        for(u16 j=0;j<512;j++) sector[j]=root[i*512+j];
        if(!write_sector(static_cast<u16>(root_start+i))) return false;
        dirty_root&=static_cast<u16>(~(1U<<i));
    }
    return true;
}
static void root_changed(int index) {dirty_root|=static_cast<u16>(1U<<(index/16));}
static u16 free_clusters() {
    u16 count=0;for(u16 n=2;n<cluster_limit;n++) if(!next_cluster(n)) count++;
    return count;
}
// Validate an entire chain before changing it. Cycles/bad/reserved/out-of-range
// links are errors; they must never cause an unbounded loop or free another link.
static bool chain_info(u16 first,u16& count,u16& tail) {
    u8 seen[(cluster_limit+7)/8]{};count=0;tail=0;
    if(!first) return true;
    for(u16 n=first;;) {
        if(!valid_cluster(n) || (seen[n/8]&(1U<<(n%8)))) return false;
        seen[n/8]|=static_cast<u8>(1U<<(n%8));count++;tail=n;
        n=next_cluster(n);if(n>=0xff8) return true;
    }
}
static void release_chain(u16 first) {
    for(u16 n=first;valid_cluster(n);) {const u16 next=next_cluster(n);pack_cluster(n,0);n=next;}
}
static bool writable_entry(int index) {
    return index>=0 && index<224 && root[index*32] && root[index*32]!=0xe5 && !(root[index*32+11]&0x19);
}
static bool ensure_clusters(int index,u16 wanted) {
    u8* entry=root+index*32;u16 count,tail;
    if(!chain_info(word(entry+26),count,tail)) return false;
    if(count>=wanted) return true;
    if(wanted-count>free_clusters()) return false;
    const u16 old_tail=tail;u16 first_added=0;
    for(u16 n=2;count<wanted && n<cluster_limit;n++) if(!next_cluster(n)) {
        for(u16 j=0;j<512;j++) sector[j]=0;
        if(!write_sector(static_cast<u16>(data_start+n-2))) {
            if(old_tail) pack_cluster(old_tail,0xfff);
            else store_word(entry+26,0);
            release_chain(first_added);return false;
        }
        pack_cluster(n,0xfff);
        if(tail) pack_cluster(tail,n);else store_word(entry+26,n);
        if(!first_added) first_added=n;
        tail=n;count++;
    }
    root_changed(index);return count>=wanted;
}
static bool write_range(int index,u32 position,u16 seg,u16 off,u32 count,bool zero) {
    u16 cluster=word(root+index*32+26);
    for(u32 skip=position/512;skip;skip--) {
        if(!valid_cluster(cluster)) return false;
        cluster=next_cluster(cluster);
    }
    u16 within=static_cast<u16>(position%512);u32 done=0;
    while(done<count) {
        if(!valid_cluster(cluster) || !read_sector(static_cast<u16>(data_start+cluster-2))) return false;
        while(within<512 && done<count) {
            sector[within++]=zero ? 0 : static_cast<u8>(far_get(seg,static_cast<u16>(off+done)));done++;
        }
        if(!write_sector(static_cast<u16>(data_start+cluster-2))) return false;
        within=0;cluster=next_cluster(cluster);
    }
    return true;
}
static bool write_file(int index,u32 position,u16 seg,u16 off,u16 count) {
    if(!writable_entry(index) || static_cast<u32>(off)+count>65536 || position>cluster_count*512UL ||
       count>cluster_count*512UL-position) return false;
    if(!count) return true;
    const u32 old_size=longword(root+index*32+28), end=position+count;
    if(!ensure_clusters(index,static_cast<u16>((end+511)/512))) return false;
    if(position>old_size && !write_range(index,old_size,0,0,position-old_size,true)) return false;
    if(!write_range(index,position,seg,off,count,false)) return false;
    if(end>old_size) store_long(root+index*32+28,end);
    root[index*32+11]|=0x20;root_changed(index);return true;
}
static bool resize_file(int index,u32 size) {
    if(!writable_entry(index) || size>cluster_count*512UL) return false;
    u8* entry=root+index*32;u16 count,tail;
    if(!chain_info(word(entry+26),count,tail)) return false;
    const u32 old_size=longword(entry+28);
    const u16 wanted=static_cast<u16>((size+511)/512);
    if(size>old_size) {
        if(!ensure_clusters(index,wanted) || !write_range(index,old_size,0,0,size-old_size,true)) return false;
    }else if(count>wanted) {
        u16 n=word(entry+26);
        if(!wanted) {store_word(entry+26,0);release_chain(n);}
        else {
            for(u16 j=1;j<wanted;j++) n=next_cluster(n);
            const u16 rest=next_cluster(n);pack_cluster(n,0xfff);release_chain(rest);
        }
    }
    store_long(entry+28,size);entry[11]|=0x20;root_changed(index);return true;
}
static int create_file(const char* name,u8 attributes=0) {
    if(named_device(name)) return -1;
    int index=find(name);
    if(index>=0) return resize_file(index,0) ? index : -1;
    for(index=0;index<224;index++) if(!root[index*32] || root[index*32]==0xe5) break;
    if(index==224) return -1;
    u8* entry=root+index*32;for(u16 j=0;j<32;j++) entry[j]=0;
    for(u16 j=0;j<11;j++) entry[j]=static_cast<u8>(name[j]);
    entry[11]=static_cast<u8>(attributes&0x27);root_changed(index);return index;
}
static bool delete_file(int index) {
    if(!writable_entry(index)) return false;
    u16 count,tail;u8* entry=root+index*32;
    if(!chain_info(word(entry+26),count,tail)) return false;
    release_chain(word(entry+26));entry[0]=0xe5;root_changed(index);return true;
}
