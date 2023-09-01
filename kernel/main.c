#include "print.h"
#include "interrupt.h"
#include "init.h"

int main(){
    put_str("I am a kernel!\n");

    init_all();

    asm volatile ("sti");   //演示中断，在这里临时开中断
    put_str("program is now!\n");
    while(1);

}