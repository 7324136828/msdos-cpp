; Standard DOS .COM program, executed natively by the C++ kernel.
BITS 16
ORG 0x100
mov dx,message
mov ah,9
int 0x21
mov dx,0x5c
mov ah,15
int 0x21
cmp al,0
jne done
mov dx,buffer
mov ah,26
int 0x21
mov dx,0x5c
mov ah,20
int 0x21
cmp al,1
je done
mov si,buffer
mov cx,128
.print:
lodsb
cmp al,0x1a
je done
test al,al
jz done
mov dl,al
mov ah,2
int 0x21
loop .print
done:
int 0x20
message db 'DOS .COM execution and INT 21h work.',13,10,'$'
buffer times 128 db 0
