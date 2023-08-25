[bits 32]
%define ERROR_CODE nop                      ;有些中断进入CPU前会自动压入错误码（32位）
                                            ;有些不会压入错误码，为了统一管理，对于不压入错误码的中断手动压入一个0
%define ZERO push 0

extern put_str                              ;声明外部函数

section .data
intr_str db "interrupt occur", 0xa, 0
global intr_entry_table
intr_entry_table:
%macro VECTOR 2
section .text
intr%1entry:                                ;每个中断处理程序都要压入中断向量号
                                            ;所以一个中断类型一个中断处理程序
                                            ;自己需要知道中断向量号是多少
    %2
    push intr_str                           ;使用栈传递参数
    call put_str
    add esp, 4                              ;跳过参数

    ;如果是从片上进入的中断，除了往从片上发送EOI外，还要往主片上发送EOI
    mov al, 0x20                            ;中断结束命令EOI
    out 0xa0, al                            ;往从片发送
    out 0x20, al                            ;往主片发送

    add esp, 4                              ;跨过error_code
    iret                                    ;从中断返回

section .data
    dd intr%1entry                          ;存储各个中断程序的入口地址

%endmacro

VECTOR 0x00, ZERO
VECTOR 0x01, ZERO
VECTOR 0x02, ZERO
VECTOR 0x03, ZERO
VECTOR 0x04, ZERO
VECTOR 0x05, ZERO
VECTOR 0x06, ZERO
VECTOR 0x07, ZERO
VECTOR 0x08, ERROR_CODE
VECTOR 0x09, ZERO
VECTOR 0x0a, ERROR_CODE
VECTOR 0x0b, ERROR_CODE
VECTOR 0x0c, ERROR_CODE
VECTOR 0x0d, ERROR_CODE
VECTOR 0x0e, ERROR_CODE
VECTOR 0x0f, ZERO
VECTOR 0x10, ZERO
VECTOR 0x11, ERROR_CODE
VECTOR 0x12, ZERO
VECTOR 0x13, ZERO
VECTOR 0x14, ZERO
VECTOR 0x15, ZERO
VECTOR 0x16, ZERO
VECTOR 0x17, ZERO
VECTOR 0x18, ZERO
VECTOR 0x19, ZERO
VECTOR 0x1a, ZERO
VECTOR 0x1b, ZERO
VECTOR 0x1c, ZERO
VECTOR 0x1d, ZERO
VECTOR 0x1e, ERROR_CODE
VECTOR 0x1f, ZERO
VECTOR 0x20, ZERO