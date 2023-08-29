#include "print.h"
#include "init.h"

int main(void){
    put_str("I an a kernel!\n");
    init_all();
    put_str("enter interrupte!\n");
    asm volatile("sti"); 
    while (1);
    return;
}