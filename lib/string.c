#include "debug.h"
#include "string.h"
#include "global.h"

//将src开始的n个字节置为指定的值
void memset(void* dst_, uint8_t value, uint32_t size)
{   
    ASSERT(dst_ != NULL);
    uint8_t* dst = dst_;
    while(size-- > 0)
        *dst++ = value;
}

//将src开始的n个字节的数据拷贝到指定的地址
void memcpy(void* dst_, const void* src_, uint32_t size)
{
    ASSERT(src_ != NULL || dst_ != NULL);
    uint8_t* dst = dst;
    const uint8_t* src = src;             //这里也要使用const
    while (size-- > 0)
        *dst++ = *src++;
}

//将a地址开始的size个字节和b地址开始的size个字节进行比较，相同返回0，a > b返回1，否则返回0
uint8_t memcmp(const void* a_, const void* b_, uint32_t size)
{
    ASSERT(a_ != NULL || b_ != NULL);
    const uint8_t* a = a_;            //这里也要写const
    const uint8_t* b = b_;
    while(size-- > 0)
    {
        if(*a != *b)
        {
            return *a > *b ? 1 : -1;
        };
        a++;
        b++;
    }
    return 0;
}

//strcpy 将src地址的字符串拷贝到dst地址
// void strcpy(void* dst_, const void* src_)             //只读需要加const,返回值需要是一个地址
char* strcpy(void* dst_, const void* src_) 
{
    ASSERT(dst_ != NULL || src_ != NULL);
    char* dst = dst_;                                   //这里dst可以直接进行操作，为了安全也可以不操作
    const char* src = src_;
    /*
    *src：首先，读取 src 指针指向的内存位置的内容，即源字符串的当前字符。
    *dst：然后，将读取到的字符写入 dst 指针指向的内存位置，即目标字符串的当前位置。
    src++ 和 dst++：接着，递增（自增）src 和 dst 指针，使它们分别指向源字符串和目标字符串中的下一个字符。
    *src：再次读取 src 指针指向的内存位置的内容，即源字符串的下一个字符。
    判断 *src：此时会判断 *src 的值，如果它不是 null 终止符（\0），则继续执行循环。如果 *src 是 null 终止符，表示源字符串的末尾已经达到，循环停止。
    */
    while((*dst++ = *src++));    
    return dst_;   
}

//strlen 给出给定字符串地址的长度
// uint32_t strlen(const void* src_)
// {
//     ASSERT(src_ != NULL);
//     const char* src = src_;
//     uint32_t size = 0;
//     while(*src++)
//         size++;
//     return size;
// }

//更帅的解法
uint32_t strlen(const char* str)
{
    ASSERT(str != NULL);                //直接使用差值得出结果，少一个变量的空间
    char* p = str;
    while(*p++);
    return (p - str - 1);
}



//strcmp 将字符串a和b进行比较，如果a和b相等，返回0，a>b 返回1，否则返回0
// uint8_t strcmp(const void* a_, const void* b_)
// {
//     ASSERT(a_ != NULL || b_ != NULL);           //这里也可以直接使用给定的两个指针
//     const char* a = a_;
//     const char* b = b_;
//     while(*a != 0)                   //这里有问题，如果*a == 0，但是*b不等于0，a串是小于b串的，这时进不到循环了，返回了0
//     {
//         if(*a != *b)
//             return *a > *b ? 1 : -1;
//         a++;
//         b++;
//     }
//     return 0;
// }

//作者的代码 ，更加简洁高效
int8_t strcmp(const char* a, const char* b)
{
    ASSERT(a != NULL || b != NULL);
    while(a != NULL && *a == *b)                //当走到最后的时候，1、若a比b长，则字符不等于0，a>b;2、若a比b短，则字符不等于0
    {
        a++;
        b++;
    };
    return *a < *b ? -1 : *a > *b;              
}


//str_find_ch_addr 从左到右查找字符ch出现的地址
char* str_find_ch_addr(const char* str, const char ch)             //这里已经是指定了字符串，就要用char*类型，不要在使用void*类型
{
    ASSERT(str != NULL);
    // while(*str++ != ch);                 //写的有问题
    // --str;
    // return *str;
    while(*str != 0)
    {
        if(*str == ch)
            return (char*)str;              //需要强制转化为char*，否则编译器汇报const属性丢失
        str++;
    }
    return NULL;
}

//str_rfind_ch_addr 从右到左查找字符ch出现的地址
// uint32_t* str_rfind_ch_addr(const void* str_, const char ch)
char* str_rfind_ch_addr(const char* str, const char ch)
{
    ASSERT(str != NULL);
    const char* last_ch = NULL;         //这个同样不允许进行修改
    while(*str!= 0)
    {
        if(*str == ch)
            last_ch = str;
        str++;                          //这里少了一个更新变量
    }
    return (char*)last_ch;              //需要强制转换
}

//strcat 将src地址的字符串拼接到dst中/并返回拼接的地址
// char* strcat(char* dst_, const char* src_)
// {
//     ASSERT(src_ != NULL || dst_ != NULL);        
//     const char* src = src_;
//     const char* tmp = src_;
//     char* dst = dst_;
//     while(tmp != 0)          //这个完全就找错地方
//     {
//         tmp++;
//         dst++;
//     }
//     dst--;
//     while (*dst++ = *src++);
//     return dst_;
// }

//作者的代码更加高效
char* strcat(char* dst_, const char* src_)
{
    ASSERT(src_ != NULL || dst_ != NULL);
    char* str = dst_;
    while(*str++);          //这个直接找到dst的结尾
    --str;
    while((*str++ = *src_++));
    return dst_;
}


//str_ch_nums 输出ch在str中出现的次数
uint32_t str_ch_nums(const char* str_, char ch)
{
    ASSERT(str_ != NULL);
    const uint8_t* p = str_;      //这里命名也有问题，这里必须命名为p
    uint32_t num = 0;
    // while (str != 0)            //这里少了一个星
    while (*p != 0)
    {
        if(*p == ch)
            num ++;
        p ++;
    }
    return num;
}