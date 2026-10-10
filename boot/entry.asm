; Minimal real-mode ABI: BIOS calls, far memory, DOS interrupt entry, COM launch.
BITS 16
%ifidn __OUTPUT_FORMAT__,elf32
%define _kernel_main kernel_main
%define _dos_dispatch dos_dispatch
%define _dos_frame dos_frame
%define _run_com run_com
%define _bios_put bios_put
%define _bios_video bios_video
%define _console_write console_write
%define _bios_key bios_key
%define _bios_status bios_status
%define _bios_read bios_read
%define _bios_write bios_write
%define _bios_clock bios_clock
%define _bios_aux bios_aux
%define _bios_print bios_print
%define _call5_offset call5_offset
%define _raw_dispatch raw_dispatch
%define _far_get far_get
%define _far_set far_set
%define _install_dos install_dos
%endif
section .text.start
global _start, _boot_drive, _run_com, _bios_put, _bios_key, _bios_status
global _bios_video
global _bios_read, _bios_write, _bios_clock, _bios_aux, _bios_print, _call5_offset
global _far_get, _far_set, _install_dos, _program_return
extern _kernel_main, _dos_dispatch, _raw_dispatch, _dos_frame, _bss_start, _bss_end
extern _console_write

_start:
    cli
    mov ax,0x1000
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov esp,0xfffc
    cld
    mov edi,_bss_start
    mov ecx,_bss_end
    sub ecx,edi
    xor al,al
    a32 rep stosb
    mov [dword _boot_drive],dl
    ; COM1: 115200 baud, 8N1, polled, for automated VM tests.
    mov dx,0x3f9
    xor al,al
    out dx,al
    mov dx,0x3fb
    mov al,0x80
    out dx,al
    mov dx,0x3f8
    mov al,1
    out dx,al
    inc dx
    xor al,al
    out dx,al
    mov dx,0x3fb
    mov al,3
    out dx,al
    mov dx,0x3fc
    mov al,0x0b
    out dx,al
    sti
    call dword _kernel_main
.hang:
    hlt
    jmp .hang

_bios_put:
    push ebp
    mov ebp,esp
    pushad
    push dword [ss:ebp+8]
    call dword _console_write
    add esp,4
    mov dx,0x3fd
.serial:
    in al,dx
    test al,0x20
    jz .serial
    mov dx,0x3f8
    mov al,[ss:ebp+8]
    out dx,al
    popad
    pop ebp
    o32 ret

; Initialize the text display/palette, or place the hardware cursor.
_bios_video:
    push ebp
    mov ebp,esp
    push ds
    push es
    pushad
    cmp dword [ss:ebp+8],0
    jne .cursor
    mov ax,3
    int 0x10
    mov ax,0x1003          ; bright backgrounds, no blinking text
    xor bx,bx
    int 0x10
    xor esi,esi
.palette:
    mov ax,0x1000          ; map attributes to DAC entries 0..15
    mov bx,si
    mov bh,bl
    int 0x10
    mov bx,si
    imul edi,esi,3
    mov dh,[cs:dword terminal_palette+edi]
    mov ch,[cs:dword terminal_palette+edi+1]
    mov cl,[cs:dword terminal_palette+edi+2]
    mov ax,0x1010
    int 0x10
    inc si
    cmp si,16
    jb .palette
    mov ah,1
    mov cx,0x0d0f          ; slim underline cursor in the 8x16 BIOS font
    int 0x10
    jmp .done
.cursor:
    mov dx,[ss:ebp+12]
    mov ah,2
    xor bx,bx
    int 0x10
.done:
    popad
    pop es
    pop ds
    pop ebp
    o32 ret

_bios_key:
    push ebp
    push ebx
    push esi
    push edi
.wait:
    mov dx,0x3fd
    in al,dx
    test al,1
    jz .keyboard
    mov dx,0x3f8
    in al,dx
    movzx eax,al
    jmp .done
.keyboard:
    mov ah,1
    int 0x16
    jz .wait
    xor ah,ah
    int 0x16
    movzx eax,ax
.done:
    pop edi
    pop esi
    pop ebx
    pop ebp
    o32 ret

_bios_status:
    push ebp
    push ebx
    push esi
    push edi
    mov dx,0x3fd
    in al,dx
    test al,1
    jnz .yes
    mov ah,1
    int 0x16
    jnz .yes
    xor eax,eax
    pop edi
    pop esi
    pop ebx
    pop ebp
    o32 ret
.yes:
    mov eax,1
    pop edi
    pop esi
    pop ebx
    pop ebp
    o32 ret

_bios_read:
    mov byte [cs:dword disk_operation],2
    jmp bios_disk
_bios_write:
    mov byte [cs:dword disk_operation],3
bios_disk:
    push ebp
    mov ebp,esp
    push es
    push ds
    pushad
    mov byte [cs:dword disk_retries],3
.retry:
    mov ax,[ss:ebp+12]
    mov es,ax
    mov bx,[ss:ebp+16]
    mov ax,[ss:ebp+8]
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
    mov dl,[cs:dword _boot_drive]
    mov ah,[cs:dword disk_operation]
    mov al,1
    int 0x13
    jnc .result
    dec byte [cs:dword disk_retries]
    jz .result
    xor ah,ah
    mov dl,[cs:dword _boot_drive]
    int 0x13
    jmp .retry
.result:
    setnc al
    movzx eax,al
    mov [ss:esp+28],eax
    popad
    pop ds
    pop es
    pop ebp
    o32 ret

_bios_clock:
    push ebp
    mov ebp,esp
    push ebx
    push esi
    push edi
    mov ah,2
    cmp dword [ss:ebp+8],0
    je .read
    mov ah,4
.read:
    int 0x1a
    jc .failed
    movzx eax,cx
    shl eax,16
    mov ax,dx
    jmp .done
.failed:
    xor eax,eax
.done:
    pop edi
    pop esi
    pop ebx
    pop ebp
    o32 ret

_bios_aux:
    push ebp
    mov ebp,esp
    push ebx
    push esi
    push edi
    mov ah,[ss:ebp+8]       ; 1=send, 2=receive, BIOS COM1
    mov al,[ss:ebp+12]
    xor dx,dx
    int 0x14
    test ah,0x80
    jz .done
    mov al,0x1a
.done:
    movzx eax,al
    pop edi
    pop esi
    pop ebx
    pop ebp
    o32 ret

_bios_print:
    push ebp
    mov ebp,esp
    pushad
    mov al,[ss:ebp+8]
    xor ah,ah
    xor dx,dx
    int 0x17
    popad
    pop ebp
    o32 ret

_call5_offset:
    mov eax,call5
    o32 ret
call5:
    pop ax                 ; far CALL's IP (PSP:000a)
    pop ax                 ; program CS
    pop word [cs:dword call5_ip] ; near CALL 5's return IP
    pushf
    cli
    push ax
    push word [cs:dword call5_ip]
    cmp cl,36
    ja .invalid
    mov ah,cl
    jmp int21
.invalid:
    xor al,al
    iret

_far_get:
    push es
    mov ax,[ss:esp+6]
    mov es,ax
    mov edx,[ss:esp+10]
    movzx eax,byte [es:edx]
    pop es
    o32 ret

_far_set:
    push es
    mov ax,[ss:esp+6]
    mov es,ax
    mov edx,[ss:esp+10]
    mov al,[ss:esp+14]
    mov [es:edx],al
    pop es
    o32 ret

_install_dos:
    push es
    xor ax,ax
    mov es,ax
    mov eax,int20
    mov [es:0x80],ax
    mov word [es:0x82],0x1000
    mov eax,int21
    mov [es:0x84],ax
    mov word [es:0x86],0x1000
    mov eax,int25
    mov [es:0x94],ax
    mov word [es:0x96],0x1000
    mov eax,int26
    mov [es:0x98],ax
    mov word [es:0x9a],0x1000
    pop es
    o32 ret

int20:
    push ax
    push bp
    mov bp,sp
    mov eax,_program_return
    mov [ss:bp+4],ax
    mov word [ss:bp+6],0x1000
    pop bp
    pop ax
    iret

int21:
    mov byte [cs:dword interrupt_kind],0
    jmp dos_interrupt
int25:
    mov byte [cs:dword interrupt_kind],1
    jmp dos_interrupt
int26:
    mov byte [cs:dword interrupt_kind],2
dos_interrupt:
    push ds
    push es
    pusha
    mov ax,ss
    mov [cs:dword caller_ss],ax
    mov [cs:dword caller_sp],sp
    mov es,ax
    movzx esi,sp
    mov ax,0x1000
    mov ds,ax
    mov edi,_dos_frame
    mov cx,13
.copy_in:
    mov ax,[es:esi]
    mov [edi],ax
    add esi,2
    add edi,2
    loop .copy_in
    mov ax,0x1000
    mov es,ax                  ; GCC string operations require ES = DS = SS
    mov ss,ax
    mov esp,0xe000
    cld
    sti
    cmp byte [dword interrupt_kind],0
    jne .raw_call
    call dword _dos_dispatch
    jmp .called
.raw_call:
    movzx eax,byte [dword interrupt_kind]
    dec eax
    push eax
    call dword _raw_dispatch
    add esp,4
.called:
    cli
    test eax,eax
    jz .resume
    mov eax,_program_return
    mov [dword _dos_frame+20],ax
    mov word [dword _dos_frame+22],0x1000
.resume:
    mov ax,[dword caller_ss]
    mov es,ax
    movzx edi,word [dword caller_sp]
    mov esi,_dos_frame
    mov cx,13
.copy_out:
    mov ax,[esi]
    mov [es:edi],ax
    add edi,2
    add esi,2
    loop .copy_out
    mov ax,[dword caller_ss]
    movzx esp,word [dword caller_sp]
    mov ss,ax
    popa
    pop es
    pop ds
    cmp byte [cs:dword interrupt_kind],0
    jne .raw_return
    iret
.raw_return:
    ; INT 25h/26h leave the flags word on the caller's stack. The caller
    ; removes it separately; RETF (not IRET) preserves this DOS convention.
    push bp
    mov bp,sp
    push word [ss:bp+6]
    popf
    pop bp
    retf

_run_com:
    pushad
    mov [cs:dword kernel_sp],esp
    cli
    mov ax,0x2000
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov esp,0xfffe
    push word 0
    xor ax,ax               ; default FCB drive-validity result for COM entry
    sti
    jmp 0x2000:0x100
_program_return:
    cli
    mov ax,0x1000
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov esp,[cs:dword kernel_sp]
    cld
    sti
    popad
    o32 ret

section .rodata
; Six-bit RGB: charcoal, soft whites, muted blue/green/cyan/red/yellow.
terminal_palette:
    db 4,4,5, 9,9,11, 20,37,25, 18,36,38
    db 40,20,23, 32,25,40, 42,34,18, 52,53,55
    db 32,34,37, 30,43,60, 38,51,34, 26,49,54
    db 60,30,31, 47,38,57, 57,49,32, 61,61,62

section .bss
_boot_drive resb 1
caller_ss resw 1
caller_sp resw 1
kernel_sp resd 1
disk_operation resb 1
disk_retries resb 1
call5_ip resw 1
interrupt_kind resb 1
