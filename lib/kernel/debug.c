#include "interrupt.h"
#include "print.h"
#include "debug.h"

void panic_span(char* filename, int line, const char* func, const char* condition)
{
    //关中断
    intr_disable();
    put_str("\n\n\nerror!!");
    put_str("\tfilename:\t");put_str(filename);put_char('\n');
    put_str("\tline:\t0x");put_int(line);put_char('\n');
    put_str("\tfilename:\t");put_str((char*)func);put_char('\n');
    put_str("\tcondition:\t");put_str((char*)condition);put_char('\n');
    while(1);
}