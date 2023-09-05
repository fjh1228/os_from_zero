#ifndef __LIB_KERNEL_DEBUG_H
#define __LIB_KERNEL_DEBUG_H

#define PANIC(...) panic_span(__FILE__, __LINE__, __func__, __VA_ARGS__)

void panic_span(char* filename, int line, const char* func, const char* condition);

#ifdef NDEBUG
    #define ASSERT(CONDITION){(void)0}
#else
    #define ASSERT(CONDITION) \
    if (CONDITION){}        \
    else{                   \
        PANIC(#CONDITION);  \
    }
#endif /*_NDEBUG*/

#endif /*debug.h*/