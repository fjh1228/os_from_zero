#include "debug.h"
#include "global.h"
#include "bitmap.h"
#include "string.h"
//将位图初始化为0
void init_bitmap(struct bitmap* bitmap)
{   
    ASSERT(bitmap != NULL);
    /*
    这里有memset函数，可对指定地址的字节长度进行初始化
    uint32_t i;
    for(i = 0; i < bitmap->bitmap_length / 8 ; i++)
    {
        bitmap->bits = 0;  
        bitmap++;
    }
    */
   memset(bitmap->bits, 0, bitmap->bitmap_length);
    
}

//将位图的某一位设置为指定值
void set_bitmap(struct bitmap* bitmap, uint32_t idx, int value)
{
    ASSERT(bitmap != NULL || value == 1 || value == 0);
    ASSERT(bitmap->bitmap_length < idx);
    uint32_t byte_idx = idx / 8;
    uint8_t bit_idx = idx % 8;
    uint8_t target_byte = bitmap->bits[byte_idx];
    // target_byte |= (BIT_MASK << bit_idx);               //这里有问题，不能这样直接进行设置，如果为1可以进行或操作，如果设置为0，显然错误
    if(value)
    {
        target_byte |= (BIT_MASK << bit_idx);
    }
    else
    {
        //为0的情况
        target_byte &= ~(BIT_MASK << bit_idx);
    }
}

//测试某位是否为1
bool test_bitmap(struct bitmap* bitmap, uint32_t idx)
{
    ASSERT(bitmap != NULL);
    ASSERT(bitmap->bitmap_length > idx);
    uint32_t byte_idx = idx / 8;
    uint8_t bit_idx = idx % 8;
    return bitmap->bits[byte_idx] & (BIT_MASK << bit_idx);
}

//在位图中申请连续的cnt个位，如果有有返回idx，如果没有返回-1
int bitmap_scan(struct bitmap* bitmap, uint32_t cnt)
{
    ASSERT(bitmap != NULL);
    uint32_t byte_idx = 0;
    while(byte_idx < bitmap->bitmap_length)
    {
        if(bitmap->bits[byte_idx] == 0xff)          //说明这一个字节已经被全部分配了
        {
            byte_idx++;
        }

    }
    if(byte_idx == bitmap->bitmap_length)
        return -1;                                  //已经满了
    
    uint8_t bit_idx = 0;
    for(bit_idx = 0; bit_idx < 8; bit_idx++)
    {
        if(test_bitmap(bitmap->bits[byte_idx], bit_idx) == 1)
            break;
        else
            {
                bit_idx++; 
            }
    }
    int bit_idx_start = byte_idx*8 + bit_idx;
    if(cnt == 1)                                    //如果只需要一块，那么肯定这里有
    {
        return bit_idx_start;
    }
    //记录还有多少位需要判断。
    uint32_t bit_left = (bitmap->bitmap_length * 8) - bit_idx_start;
    uint32_t next_bit = bit_idx_start + 1;
    uint32_t count = 1;                             //用于记录找到的空闲位的个数

    bit_idx_start = -1;
    while (bit_left-- > 0)
    {
        if(!test_bitmap(bitmap, next_bit))
        {
            //如果下一位为0
            count++;
        }
        else
        {
            //有一位为1了
            count = 0;
        }
        if (count == cnt)
        {
            //找到了连续的cnt个位
            bit_idx_start = next_bit - cnt + 1;
            break;
        }
        next_bit++;
    }
    return bit_idx_start;
}
