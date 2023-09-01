[bits 32]
%define ERROR_CODE  nop 
%define ZERO push 0

extern idt_table
extern put_str
section .data
global intr_entry_table
intr_entry_table:

%macro VECTOR 2         ;定义名字为vector的宏，参数有两个
section .text
intr%1vector:
    %2
    push ds
    push es
    push fs
    push gs
    pushad              ;因为使用汇编调用C语言，所以必须保存寄存器的环境

    ;如果是从片上进入的中断
    ;除了往从片上发送 EOI 外，还要往主片上发送 EOI
    mov al, 0x20
    out 0xa0, al
    out 0x20, al

    push %1

    call [idt_table + %1 * 4]
    jmp intr_exit
section .data
    dd intr%1vector
%endmacro
section .text
global intr_exit
intr_exit:
    add esp, 4          ;跳过中断号
    popad
    pop gs
    pop fs
    pop es
    pop ds
    add esp, 4          ;跳过error code
    iretd


VECTOR 0x00,ZERO                            ;调用之前写好的宏来批量生成中断处理函数，传入参数是中断号码与上面中断宏的%2步骤，这个步骤是什么都不做，还是压入0看p303
VECTOR 0x01,ZERO
VECTOR 0x02,ZERO
VECTOR 0x03,ZERO 
VECTOR 0x04,ZERO
VECTOR 0x05,ZERO
VECTOR 0x06,ZERO
VECTOR 0x07,ZERO 
VECTOR 0x08,ERROR_CODE
VECTOR 0x09,ZERO
VECTOR 0x0a,ERROR_CODE
VECTOR 0x0b,ERROR_CODE 
VECTOR 0x0c,ZERO
VECTOR 0x0d,ERROR_CODE
VECTOR 0x0e,ERROR_CODE
VECTOR 0x0f,ZERO 
VECTOR 0x10,ZERO
VECTOR 0x11,ERROR_CODE
VECTOR 0x12,ZERO
VECTOR 0x13,ZERO 
VECTOR 0x14,ZERO
VECTOR 0x15,ZERO
VECTOR 0x16,ZERO
VECTOR 0x17,ZERO 
VECTOR 0x18,ERROR_CODE
VECTOR 0x19,ZERO
VECTOR 0x1a,ERROR_CODE
VECTOR 0x1b,ERROR_CODE 
VECTOR 0x1c,ZERO
VECTOR 0x1d,ERROR_CODE
VECTOR 0x1e,ERROR_CODE
VECTOR 0x1f,ZERO 
VECTOR 0x20,ZERO