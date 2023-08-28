/* 机器模式
    b --输出寄存器QImode名称，寄存器中的最低8位[a-d]l
    w --输出寄存器HImode名称，寄存器中2个字节的部分比如：【a-d】x
    HImode
        "Half-Integer"模式，表示一个两字节的整数
    QImode
        "Quarter-integer"模式，表示一个
*/

#ifndef __LIB_KERNEL_IO_H
#define __LIB_KERNEL_IO_H
#include "stdint.h"
/* 向端口写入一个字节*/
static inline void outb(uint16_t port, uint8_t data){
    /*对端口制定N表示0-255，d表示dx存储端口号*/
    asm volatile("outb %b0, %w1"::"a"(data),"Nd"(port));
}

/*将addr处起始的word_cnt个字写入端口port*/
static inline void outsw(uint16_t port, const void* addr, uint32_t word_cnt){
    /*+表示此限制即做输入又做输出
    outsw 是把ds:esi处的16位的内容写入port端口，我们在设置段描述符时，已经将ds,es,ss设置为了相同的值*/
    asm volatile("cld; rep outsw":"+S"(addr), "+c"(word_cnt):"d"(port));
}

/*将从端口port读入的一个字节返回*/
static inline uint8_t inb(uint16_t port){
    uint8_t data;
    asm volatile("inb %w1, %b0":"=a"(data): "Nd"(port));
}

/*将从端口port读入的word_cnt个字写入addr*/
static inline void insw(uint16_t port, void* addr, uint32_t word_cnt){
    /*insw是从端口port处读入的16位内容写入es：edi指向的内存*/
    asm volatile("cld; rep insw":"+D"(addr), "+c"(word_cnt));
}
#endif