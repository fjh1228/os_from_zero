[bits 32]
%define ERROR_CODE  nop 
%define RIGHT push 0
section .data
    intr_str bb "intrrupt occur!", 0xa, 0

extern put_str
section .text
intr_entry_table:
%macro VECTOR 2         ;定义名字为vector的宏，参数有两个
intr%1vector:
    %0
    push intr_str
    call put_str

    add esp, 4          ;跳过ERROR_CODE
    