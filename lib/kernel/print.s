
TI_GDT equ 0
RPL0 equ 0
SELECTOR_VIDEO equ ( 0x0003 << 3) + TI_GDT + RPL0

[bit 32]
section .text
;--------------put_char------------------
;description: 把栈中的一个字符写入光标所在处
;----------------------------------------
global put_char
put_char:
    pushad                              ;备份32位寄存器环境
    ;需保证gs中为正确的视频段选择子
    ;为保险起见，每次打印时都为gs进行赋值
    mov ax, SELECTOR_VIDEO
    mov gs, ax
    ;获取当前光标的位置
    ;先获取光标的高8位
    mov dx, 0x03d4                      ;索引寄存器
    mov al, 0x0e                        ;提供光标位置的高8位
    out dx, al
    mov dx, 0x03d5                      ;通过读写数据端口0x3d5来获得或设置光标位置
    in al, dx                           ;得到光标位置的高8位
    mov ah, al

    ;获取光标位置的低8位
    mov dx, 0x03d4
    mov al, 0x0f
    out dx, al
    mov dx, 0x03d5
    in al, dx

    ;将光标位置存入bx
    mov bx, ax

    ;在栈中获取待打印的字符
    mov ecx, [esp + 36]                 ;pushad压入4*8=32字节，pushad命令压入8个通用的寄存器，返回地址4个字节
    cmp cl, 0xd                         ;CR换行是10，0xa，LF回车是0xd，在Linux中换行0xa表示行的结束
    jz .is_carrage_return
    cmp cl, 0xa
    jz .is_line_feed

    cmp cl, 0x8                         ;0x8是BS（backspace）
    jz .is_backspace
    jmp .put_other

.is_backspace:
;当为backspace时，本质本质上只要将光标移向前一个显存位置即可．后面再输入的字符自然会覆盖此处的字符
;但有可能在键入backspace 后并不再键入新的字符，这时光标已经向前移动到待删除的字符位置，但字符还在原处
;这就显得好怪异，所以此处添加了空格或空字符。
    dec bx                              ;bx存放的是光标的位置
    shl bx, 1                           ;光标左移一位表示乘2，表示光标对应显存中的偏移字节,因为一个光标是占用两个字节，即偏移是光标的位置的2倍

    mov byte [gs,bx], 0x20              ;0x20是空格
    inc bx
    mov byte [gs,bx], 0x07              ;高字节的颜色等信息
    shr bx, 1                           ;恢复光标的偏移值
    jmp .set_cursor

.put_other:
    shl bx, 1                           ;获取光标的偏移值

    mov [gs,bx], cl                     ;cl中存放的是ASCII码
    inc bx
    mov [gs,bx], 0x07                   ;字符属性
    shr bx, 1
    inc bx
    cmp bx, 2000
    jl .set_cursor                      ;若光标的值小于2000，表示此页没有写满，不用翻页

.is_line_feed:                          ;是换行符       这里将回车换行统一为Linux的回车换行
.is_carrage_return:                     ;是回车符
;如果是回车符，只需要将光标移到行首
    xor dx, dx                          ;dx是被除数的高16位
    mov ax, bx                          ;ax是被除数的低16位
    mov si, 80

    div si                              ;效仿Linux，换行0x0a表示下一行的行首
    sub bx, dx                          ;减去余数就是换到了行首，我觉得不太对，
                                        ;比如25*80=2000,一行80个字符，81/80=1余1，
                                        ;bx减去dx是本行的行首，不是下行的行首应该在加上80才对

.is_carrage_return_end:
;回车符CR 处理结束
    add bx, 80
    cmp bx, 2000
.is_line_feed_end:
;如果小于2000，那就给光标值加上80
    jl .set_cursor

