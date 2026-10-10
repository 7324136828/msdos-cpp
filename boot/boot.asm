; Hardware boundary only: BIOS loads a freestanding C++ real-mode kernel.
; Requires a 386+ CPU, because GCC's -m16 emits 32-bit register operations.
BITS 16
ORG 0x7c00
jmp short start
nop
db 'DOSCPP  '
dw 512
db 1
dw 65                       ; boot sector + 64 reserved kernel sectors
db 2
dw 224
dw 2880
db 0xf0
dw 9
dw 18
dw 2
dd 0
dd 0
db 0,0,0x29
dd 0x125c0001
db 'MSDOS CPP  '
db 'FAT12   '
start:
    cli
    xor ax,ax
    mov ds,ax
    mov ss,ax
    mov sp,0x7c00
    sti
    mov [drive],dl
    mov ax,0x1000
    mov es,ax
    xor bx,bx
    mov si,1
.sector:
    mov ax,si
    xor dx,dx
    mov cx,36
    div cx
    push ax                    ; preserve cylinder while CX holds the head divisor
    mov ax,dx
    xor dx,dx
    mov cx,18
    div cx
    mov dh,al
    mov cl,dl
    inc cl
    pop ax
    mov ch,al
    mov dl,[drive]
    mov ax,0x0201
    push bx
    push si
    int 0x13
    pop si
    pop bx
    jc failed
    add bx,512
    inc si
    cmp si,65
    jb .sector
    mov dl,[drive]
    jmp 0x1000:0
failed:
    mov si,error
.print:
    lodsb
    test al,al
    jz .hang
    mov ah,0x0e
    xor bx,bx
    int 0x10
    jmp .print
.hang:
    cli
    hlt
    jmp .hang
drive db 0
error db 'DOS-cpp: boot disk read failed',0
times 510-($-$$) db 0
dw 0xaa55
