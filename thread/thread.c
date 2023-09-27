#include "thread.h"
#include "stdint.h"
#include "string.h"

#define PG_SIZE 4096


/*C中的的函数原型
void* thread_func (void* _arg) {
    unsigned int * arg = _arg;
    printf(” new thread: my tid is %u\n”，*arg);
}

void main() {
    pthread_t new thread id;
    pthread_create(&new_thread_i d, NULL , thread_func , &new_thread_id);
    printf(”main thread: my tid is 在u\n ”， pthread_self () ) ;
    usleep (100);
}
*/




/* 由kernel_thread去执行function(func_arg) , 这个函数就是线程中去开启我们要运行的函数*/
static void kernel_thread(thread_func* func, void* func_arg)
{
    func(func_arg);
}

//内核态还是用户态调用未知
/*根据传入的PCB地址，运行函数以及函数的参数地址来初始化线程栈中的运行信息，核心就是要填入运行的函数地址与参数*/
void thread_create(struct task_struct* pthread, thread_func* func, void* func_arg)
{
    //预留中断栈使用的空间
    pthread->self_kstack = (uint32_t*)((int)(pthread->self_kstack - sizeof(struct intr_stack)));
    //预留出线程栈使用的空间
    pthread->self_kstack = (uint32_t*)((int)(pthread->self_kstack - sizeof(struct thread_stack)));
    
    //没搞懂这里是干嘛的
    struct thread_stack* kthread_stack = (struct thread_stack*)pthread->self_kstack;
    kthread_stack->eip = kernel_thread;
    kthread_stack->function = func;
    kthread_stack->func_arg = func_arg;
    kthread_stack->ebp = kthread_stack->ebx = kthread_stack->edi = kthread_stack->esi = 0;
    
}

//初始化线程的基本信息，PCB中存储的是线程的管理信息，
void init_pthread(struct task_struct* pthread, char* name, int prio)
{
    memset(pthread, 0, sizeof(*pthread));
    strcpy(pthread->name, name);
    pthread->priority = prio;
    pthread->status = TASK_RUNNING;
    pthread->self_kstack = (uint32_t*)((uint32_t)pthread + PG_SIZE);        //一页的PCB
    pthread->stack_magic = 0x19870916;                                      //边界数字
    
}


struct task_struct* thread_start(char* name, int prio, thread_func func, void* func_arg)
{
    /*pcb位于内核空间，包括用户进程的PCB也是在内核空间 */
    struct task_struct* thread = get_kernel_pages(1);                       //为线程的PCB申请一页的空间
    init_pthread(thread, name, prio);
    thread_create(thread, func, func_arg);

    //进入
    asm volatile("movl %0, %%esp; pop %%ebp; pop %%ebx; pop %%edi; pop %%esi; ret" : :"g"(thread->self_kstack): "memory");
    return thread;
};


