; Native DOS program: exercise FCB writes through the public ABI, not C++ helpers.
BITS 16
ORG 0x100
    mov cl,9
    mov dx,call5_message
    call 5
    mov dx,fcb
    mov ah,22
    int 0x21
    test al,al
    jnz failed
    mov dx,buffer
    mov ah,26
    int 0x21
    mov bx,2
.write:
    mov dx,fcb
    mov ah,21
    int 0x21
    test al,al
    jnz failed
    dec bx
    jnz .write
    cmp word [fcb+16],256
    jne failed
    mov word [fcb+33],5
    mov dx,fcb
    mov ah,34
    int 0x21
    test al,al
    jnz failed
    cmp word [fcb+16],768
    jne failed
    mov ah,16
    int 0x21
    test al,al
    jnz failed
    mov dx,fcb
    mov ah,15
    int 0x21
    test al,al
    jnz failed
    mov byte [fcb+32],0
    mov dx,readback
    mov ah,26
    int 0x21
    mov dx,fcb
    mov ah,20
    int 0x21
    test al,al
    jnz failed
    mov si,buffer
    mov di,readback
    mov cx,128
    cld
    repe cmpsb
    jne failed
    mov word [fcb+33],1
    xor cx,cx
    mov dx,fcb
    mov ah,40
    int 0x21
    test al,al
    jnz failed
    cmp word [fcb+16],128
    jne failed
    mov ah,16
    int 0x21
    test al,al
    jnz failed
    mov si,new_name
    mov di,fcb+17
    mov cx,11
    rep movsb
    mov byte [fcb+16],0
    mov dx,fcb
    mov ah,23
    int 0x21
    test al,al
    jnz failed
    mov si,new_name
    mov di,fcb+1
    mov cx,11
    rep movsb
    mov dx,fcb
    mov ah,19
    int 0x21
    test al,al
    jnz failed
    xor ax,ax
    mov bx,raw_buffer
    mov cx,1
    mov dx,65
    int 0x25
    pop bx                    ; DOS absolute disk calls leave a flags word
    jc failed
    cmp byte [raw_buffer],0xf0
    jne failed
    mov dx,success
    mov ah,9
    int 0x21
    int 0x20
failed:
    mov dx,error_message
    mov ah,9
    int 0x21
    int 0x20
call5_message db 'CALL 5 dispatch works.',13,10,'$'
success db 'FCB create/write/read/truncate/rename/delete passed.',13,10
        db 'Absolute sector read and flags-stack cleanup passed.',13,10,'$'
error_message db 'RWTEST FAILED.',13,10,'$'
new_name db 'RWDONE  TMP'
fcb db 0,'RWTEST  TMP'
    times 25 db 0
buffer:
%assign n 0
%rep 128
    db (n*7)&255
%assign n n+1
%endrep
readback times 128 db 0
raw_buffer times 512 db 0
