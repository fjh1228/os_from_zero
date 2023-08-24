#include "print.h"

int main(void){
    put_str("I an a kernel!\n");
    put_int(9);
    put_char('\n');
    put_int(0x12345678);
    put_char('\n');
    while (1);
    return;
}