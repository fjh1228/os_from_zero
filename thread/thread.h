#ifndef __THREAD_THREAD_H
#define __THREAD_THREAD_H
#include "stdint.h"

/*定义返回函数类型，该函数返回值是空，参数类型为void* */

typedef void thread_func(void*);

/*  先写PCB部分的数据结构,PCB一般结构如下
            寄存器映像
            栈
            栈指针
            PID
            进程状态
            优先级
            时间片
            页表
            文件描述符
            父进程
*/


//进程状态部分
enum task_status{
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_WATTING,
    TASK_HANGING,
    TASK_DEAD
};


//PCB
struct task_struct
{
    uint32_t* self_kstack;          //存储内核0特权级线程栈顶的位置
    enum task_status status;
    uint8_t priority;
    char name[16];
    uint32_t stack_magic;           //限制栈的长度
};




//中断栈。属于OS内核 
//用户态陷入核态时调用,保存所有的上下文内容
//压入顺序为：
struct intr_stack
{
    //以下通用内容为13个
    uint32_t vec_no;        //保存中断号，中断号的上限为255
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy;     //esp时刻在变，即使弹出，也会被忽略
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

    //以下由CPU进行压入
    uint32_t error_code;
    void (*eip)(void);      //指令指针寄存器，这个实际上保存的是函数的地址
    uint32_t cs;
    uint32_t eflags;
    void* esp;
    uint32_t ss;
};

/*
线程栈，属于线程, 但是存在于内核
每个线程都有自己的栈，用于存储局部变量、函数调用信息和线程执行过程中的临时数据。
线程栈是线程独立的，不同线程之间拥有各自的栈空间，
*/
struct thread_stack{
    //以下四个寄存器是因为被调用函数要维护其值不变
    uint32_t ebp;
    uint32_t ebx;
    uint32_t edi;
    uint32_t esi;
    
    //函数指针
    void (*eip) (thread_func* func, void* func_arg);

    void (*unused_retaddr);
    thread_func* function;
    void* func_arg;
};



#endif