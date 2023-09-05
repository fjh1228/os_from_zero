#ifndef __KERNEL_INTERRUPT_H
#define __KERNEL_INTERRUPT_H
#include "stdint.h"
typedef void* intr_handler;
void init_all(void);

enum intr_status{
    INTR_ON,
    INTR_OFF
};

enum intr_status intr_enable(void);
enum intr_status intr_disable(void);
enum intr_status get_intr_status(void);
enum intr_status set_intr_status(enum intr_status status);
#endif