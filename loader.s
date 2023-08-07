%include "boot.inc"

LOADER_STACK_TOP equ LOADER_BASE_ADDR

section loader vstart=LOADER_BASE_ADDR
    
    jmp loader_start
    ;构建GDT及其内部的描述符
    ;GDT[0] 填充0
    GDT_BASE:   dd 0x00000000,0x00000000
    CODE_DESC:  dd 0x0000FFFF,DESC_CODE_HIGH4
    DATA_STACK_DESC:    dd 0x0000FFFF,DESC_DATA_HIGH4
    VIDEO_DESC: dd 0x80000007,DESC_VIDEO_HIGH4          ;LIMIT=(0xbffff-0xb8000) / 4k = 0x7
    
    GDT_SIZE    equ $-GDT_BASE
    GDT_LIMIT   equ GDT_SIZE-1

    times 60 dq 0

    SELECTOR_DATA   equ     0x1 << 3 + TI_GDT + PRL_0
    SELECTOR_CODE   equ     0x2 << 3 + TI_GDT + PRL_0
    SELECTOR_VIDEO  equ     0x3 << 3 + TI_GDT + PRL_0

    GDT_PTR dw  GDT_LIMIT
            dd  GDT_BASE
    loadermsg   db  '2 loader in real.'

loader_start:
    ;mov gs, cs
    ;使用BIOS中断0x10输出字符串
    mov sp, LOADER_STACK_TOP
    mov bp, loadermsg
    mov cx, 17
    mov ax, 0x1301
    mov bx, 0x001f
    mov dx, 0x1800
    int 0x10
;------------------------------准备进入保护模式-------------------------
;1 打开A20，使用0x92端口
;2 加载gdt
;3 将cr0的pe位置1
    in al, 0x92                 ;端口一般是8位
    or al, 0x01
    out 0x92, al

    lgdt [GDT_PTR]

    mov eax, cr0                ;cr0是控制寄存器已经是32位，处于保护模式
    or eax, 1
    mov cr0, eax
    
    jmp dword  SELECTOR_CODE:p_mode_start
; 进入保护模式之后，段寄存器的值会被清零

[bits 32]
p_mode_start:
    mov ax, SELECTOR_DATA
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, LOADER_STACK_TOP
    mov ax, SELECTOR_VIDEO
    mov gs, ax

    mov byte [gs:80], 'P'
    mov byte [gs:81], 0xA4
    jmp $
