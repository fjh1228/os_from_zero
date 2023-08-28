#include "interrupt.h"
#include "stdint.h"
#include "global.h"
#include "io.h"

#define IDT_DESC_CNT 0x21
#define PIC_M_CTRL 0x20      //主片的控制端口是0x20
#define PIC_M_DATA 0x21      //主片的数据端口是0x21
#define PIC_S_CTRL 0x20      //从片的控制端口是0x20
#define PIC_S_DATA 0x21      //从片的数据端口是0x21

/*初始化可编程中断控制器8295A*/
static void pic_init(void){
    /*初始化主片*/
    outb(PIC_M_CTRL, 0X11);        // ICW1:边沿触发，级联8295，需要ICW4
    outb(PIC_M_DATA, 0X20);        // ICW2:起始中断向量号为0x20
    outb(PIC_M_CTRL, 0X04);        // ICW3:IR2接从片
    outb(PIC_M_DATA, 0X01);        // ICW4:8086模式，正常EOI

    /*初始化从片*/
    outb(PIC_S_CTRL, 0x11);        // ICW1:边沿触发，级联8295，需要ICW4
    outb(PIC_S_DATA, 0X28);        // ICW2:起始中断向量号为0x28
    outb(PIC_S_CTRL, 0X02);        // ICW3:设置从片连接到主片的IR2
    outb(PIC_S_DATA, 0X01);        // ICW4:8086模式，正常EOI

    /*打开主片上的IR0，也就是目前只接受时钟中断*/
    outb(PIC_M_DATA, 0XFE);
    outb(PIC_S_DATA, 0XFF);
    put_str("pic_init done\n");
}

/*中断门描述符结构体*/
struct gate_desc
{
    uint16_t func_offset_low_word;
    uint16_t selector;
    uint8_t  dcount;
    uint8_t  attribute;
    uint16_t func_offset_high_word;
};

//静态函数声明，非必须
static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attr, intr_handler function);
static struct gate_desc idt[IDT_DESC_CNT];      //中断描述符表

extern intr_handler intr_entry_table[IDT_DESC_CNT];      //声明引用定义在kernel.s中的中断处理函数入口数组

/*创建中断门描述符*/
static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attr, intr_handler function){
    p_gdesc->func_offset_low_word = (uint32_t)function & 0x0000ffff;
    p_gdesc->selector = SELECTOR_K_CODE;
    p_gdesc->dcount = 0;
    p_gdesc->attribute = attr;
    p_gdesc->func_offset_high_word = ((uint32_t)function & 0xffff0000) >> 16;
}

/*初始化中断描述符表*/
static void idt_desc_init(void){
    int i;
    for(i=0; i<IDT_DESC_CNT; i++){
        make_idt_desc(&idt[i], IDT_DESC_ATTR_DPL0, intr_entry_table[i]);
        put_str("  idt_desc_init done\n");
    }
}
/*完成所有的初始化工作*/
void idt_init(){
    put_str("idt_init start\n");
    idt_desc_init();                //初始化中断描述符表
    pic_init();                     //初始化8295A

    /*加载idt*/
    uint64_t idt_operand = ((sizeof(idt) - 1) | ((uint64_t)(uint32_t)idt << 16));
    asm volatile("lidt %0" : : "m" (idt_operand));
}