#include "print.h"
#include "interrupt.h"
#include "stdint.h"
#include "global.h"
#include "io.h"

/*********************************************定义中断相关的数据结构*******************************/
// #define IDT_DESC_CNT 0x21

#define IDT_DESC_CNT 0x21	   //支持的中断描述符个数33


//定义IDT结构体
struct gate_desc{
    int16_t low_16_offset;
    int16_t selector;
    int8_t  cnt;
    int8_t  attr;
    int16_t high_16_offset;
};

// struct gate_desc {
//    uint16_t    func_offset_low_word;        //函数地址低字
//    uint16_t    selector;                    //选择子字段
//    uint8_t     dcount;                      //此项为双字计数字段，是门描述符中的第4字节。这个字段无用
//    uint8_t     attribute;                   //属性字段
//    uint16_t    func_offset_high_word;       //函数地址高字
// };

static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attribute, intr_handler intr_func);
extern intr_handler intr_entry_table[IDT_DESC_CNT];
static struct gate_desc idt[IDT_DESC_CNT];          //中断门描述符（结构体），不是中断描述符表，一个中断门可以在中断描述符表中存在很多的
intr_handler idt_table[IDT_DESC_CNT];
char* intr_name[IDT_DESC_CNT];
// static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attr, intr_handler function);
// static struct gate_desc idt[IDT_DESC_CNT];   //中断门描述符（结构体）数组，名字叫idt

// extern intr_handler intr_entry_table[IDT_DESC_CNT];	    //引入kernel.s中定义好的中断处理函数地址数组，intr_handler就是void* 表明是一般地址类型

// char* intr_name[IDT_DESC_CNT];		         //存储中断/异常的名字
// intr_handler idt_table[IDT_DESC_CNT];	     // 定义中断处理程序数组.在kernel.S中定义的intrXXentry只是中断处理程序的入口,最终调用的是ide_table中的处理程序


/***************************************补全中断描述符表************************************/
static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attribute, intr_handler intr_func){
        p_gdesc->low_16_offset =  (uint32_t)(intr_func) & (0x0000FFFF);
        p_gdesc->selector = SELECTOR_K_CODE;
        p_gdesc->cnt = 0;
        p_gdesc->attr = attribute;
        p_gdesc->high_16_offset =((uint32_t)(intr_func) & (0xFFFF0000))>>16;
}

// static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attr, intr_handler function) { 
//    p_gdesc->func_offset_low_word = (uint32_t)function & 0x0000FFFF;
//    p_gdesc->selector = SELECTOR_K_CODE;
//    p_gdesc->dcount = 0;
//    p_gdesc->attribute = attr;
//    p_gdesc->func_offset_high_word = ((uint32_t)function & 0xFFFF0000) >> 16;
// }


// static void idt_desc_init(void) {
//    int i;
//    for (i = 0; i < IDT_DESC_CNT; i++) {
//       make_idt_desc(&idt[i], IDT_DESC_ATTR_DPL0, intr_entry_table[i]); 
//    }
//    put_str("   idt_desc_init done\n");
// }

static void idt_desc_init(){
    put_str("   idt_desc init start!\n");
    int i;
    for(i = 0; i < IDT_DESC_CNT; i++){
        make_idt_desc(&idt[i], IDT_DESC_ATTR_DPL0,intr_entry_table[i]);
        // if (i == 0){
        //     put_str("p_gdesc->low_16_offset:  ");
        //     put_int(idt[0].low_16_offset);
        //     put_char('\n');
        //     put_str("p_gdesc->selector:  ");
        //     put_int(idt[0].selector);
        //     put_char('\n');
        //     put_str("p_gdesc->cnt:  ");
        //     put_int(idt[0].cnt);
        //     put_char('\n');
        //     put_str("p_gdesc->attr:  ");
        //     put_int(idt[0].attr);
        //     put_char('\n');
        //     put_str("p_gdesc->high_16_offset:  ");
        //     put_int(idt[0].high_16_offset);
        //     put_char('\n');
        // }
    }
    put_str("   idt_desc init done!\n");
} 

/***************************************为每一个中断程序赋予不同的处理*******************************************/
// static void general_intr_handler(uint8_t vec_nr) {
//    if (vec_nr == 0x27 || vec_nr == 0x2f) {	//伪中断向量，无需处理。详见书p337
//       return;		
//    }
//    put_str("int vector: 0x");
//    put_int(vec_nr);
//    put_char('\n');
// }

static void generate_idtr_handler(uint8_t idx){
    /*
    编写每一个中断程序，内容就是打印自己的中断号，然后没了
    */
    if(idx == 0x27 || idx == 0x2f){
        //这两个中断是伪中断
        //0x2f是8295A上的最后一个引脚，保留项
        return;             
    }
    put_str("Now is intr 0x");
    put_int(idx);
    put_char('\n');

}

// /* 完成一般中断处理函数注册及异常名称注册 */
// static void exception_init(void) {			    // 完成一般中断处理函数注册及异常名称注册
//    int i;
//    for (i = 0; i < IDT_DESC_CNT; i++) {

// /* idt_table数组中的函数是在进入中断后根据中断向量号调用的,
//  * 见kernel/kernel.S的call [idt_table + %1*4] */
//       idt_table[i] = general_intr_handler;		    // 默认为general_intr_handler。
// 							    // 以后会由register_handler来注册具体处理函数。
//       intr_name[i] = "unknown";				    // 先统一赋值为unknown 
//    }
//    intr_name[0] = "#DE Divide Error";
//    intr_name[1] = "#DB Debug Exception";
//    intr_name[2] = "NMI Interrupt";
//    intr_name[3] = "#BP Breakpoint Exception";
//    intr_name[4] = "#OF Overflow Exception";
//    intr_name[5] = "#BR BOUND Range Exceeded Exception";
//    intr_name[6] = "#UD Invalid Opcode Exception";
//    intr_name[7] = "#NM Device Not Available Exception";
//    intr_name[8] = "#DF Double Fault Exception";
//    intr_name[9] = "Coprocessor Segment Overrun";
//    intr_name[10] = "#TS Invalid TSS Exception";
//    intr_name[11] = "#NP Segment Not Present";
//    intr_name[12] = "#SS Stack Fault Exception";
//    intr_name[13] = "#GP General Protection Exception";
//    intr_name[14] = "#PF Page-Fault Exception";
//    // intr_name[15] 第15项是intel保留项，未使用
//    intr_name[16] = "#MF x87 FPU Floating-Point Error";
//    intr_name[17] = "#AC Alignment Check Exception";
//    intr_name[18] = "#MC Machine-Check Exception";
//    intr_name[19] = "#XF SIMD Floating-Point Exception";

// }
void exception_init(){
    /*
    为每一个中断赋予一个名字
    */
    int i;
    for(i = 0; i < IDT_DESC_CNT; i++){
        intr_name[i] = "unknown";
        idt_table[i] = generate_idtr_handler;           //蒂花之秀，太秀了
    }
    intr_name[0] = "#DE Divide Error";
    intr_name[1] = "#DB Debug Exception";
    intr_name[2] = "NMI Interrupt";
    intr_name[3] = "#BP Breakpoint Exception";
    intr_name[4] = "#OF Overflow Exception";
    intr_name[5] = "#BR BOUND Range Exceeded Exception";
    intr_name[6] = "#UD Invalid Opcode Exception";
    intr_name[7] = "#NM Device Not Available Exception";
    intr_name[8] = "#DF Double Fault Exception";
    intr_name[9] = "Coprocessor Segment Overrun";
    intr_name[10] = "#TS Invalid TSS Exception";
    intr_name[11] = "#NP Segment Not Present";
    intr_name[12] = "#SS Stack Fault Exception";
    intr_name[13] = "#GP General Protection Exception";
    intr_name[14] = "#PF Page-Fault Exception";
    // intr_name[15] 第15项是intel保留项，未使用
    intr_name[16] = "#MF x87 FPU Floating-Point Error";
    intr_name[17] = "#AC Alignment Check Exception";
    intr_name[18] = "#MC Machine-Check Exception";
    intr_name[19] = "#XF SIMD Floating-Point Exception";
}


/***************************************初始化可编程中断控制器8295A**************************/

#define PIC_M_CTRL 0x20	       // 这里用的可编程中断控制器是8259A,主片的控制端口是0x20
#define PIC_M_DATA 0x21	       // 主片的数据端口是0x21
#define PIC_S_CTRL 0xa0	       // 从片的控制端口是0xa0
#define PIC_S_DATA 0xa1	       // 从片的数据端口是0xa1

static void pic_init(void) {

   /* 初始化主片 */
   outb (PIC_M_CTRL, 0x11);   // ICW1: 边沿触发,级联8259, 需要ICW4.
   outb (PIC_M_DATA, 0x20);   // ICW2: 起始中断向量号为0x20,也就是IR[0-7] 为 0x20 ~ 0x27.
   outb (PIC_M_DATA, 0x04);   // ICW3: IR2接从片. 
   outb (PIC_M_DATA, 0x01);   // ICW4: 8086模式, 正常EOI

   /* 初始化从片 */
   outb (PIC_S_CTRL, 0x11);	// ICW1: 边沿触发,级联8259, 需要ICW4.
   outb (PIC_S_DATA, 0x28);	// ICW2: 起始中断向量号为0x28,也就是IR[8-15] 为 0x28 ~ 0x2F.
   outb (PIC_S_DATA, 0x02);	// ICW3: 设置从片连接到主片的IR2引脚
   outb (PIC_S_DATA, 0x01);	// ICW4: 8086模式, 正常EOI

   /* 打开主片上IR0,也就是目前只接受时钟产生的中断 */
   outb (PIC_M_DATA, 0xfe);
   outb (PIC_S_DATA, 0xff);

   put_str("   pic_init done!\n");
}


/***************************************中断初始化程序************************************/
void idt_init(){
    put_str("idt init start!\n");
    idt_desc_init();
    exception_init();
    pic_init();

    //加载lidt
    // uint64_t idt_addr = ((uint64_t)(uint32_t)idt << 16) | (sizeof(idt) - 1);
    uint64_t idt_addr = ((sizeof(idt) - 1) | ((uint64_t)(uint32_t)idt << 16));
    put_str("idt_addr:  ");
    put_int(idt_addr);
    put_char('\n');
    // asm volatile("lidt %0"::"m"(idt_addr));
    asm volatile("lidt %0" : : "m" (idt_addr));
    put_str("idt init done!\n");
    
    // put_str("idt_init start\n");
    // idt_desc_init();	   //调用上面写好的函数完成中段描述符表的构建
    // exception_init();	   // 异常名初始化并注册通常的中断处理函数
    // pic_init();		  //设定化中断控制器，只接受来自时钟中断的信号

    // /* 加载idt */
    // uint64_t idt_operand = ((sizeof(idt) - 1) | ((uint64_t)(uint32_t)idt << 16));    //定义要加载到IDTR寄存器中的值
    // asm volatile("lidt %0" : : "m" (idt_operand));
    // put_str("idt_init done\n");

}
