#ifndef __KERNEL_BITMAP_H
#define __KERNEL_BITMAP_H
#include "global.h"
#define BIT_MASK 1
struct bitmap
{
    uint32_t bitmap_length;
    uint8_t* bits;
};


void init_bitmap(struct bitmap* bitmap);
void set_bitmap(struct bitmap* bitmap, uint32_t idx, int value);
bool test_bitmap(struct bitmap* bitmap, uint32_t idx);
int bitmap_scan(struct bitmap* bitmap, uint32_t cnt);

#endif
