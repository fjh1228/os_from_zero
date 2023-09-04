#include "print.h"
#include "interrupt.h"
#include "init.h"

int main(){
    put_str("I am a kernel!\n");

    init_all();

    int i = 0;
    asm volatile ("sti");   //演示中断，在这里临时开中断
    for(i == 0; i < 50000; i++){
        if(i >= 40000){
            asm volatile ("cli");
        }
    }
    put_str("program is now!\n");
    while(1);

}