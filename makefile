BUILD_DIR = ./build
include_path = include/
ASS_PARAM = -f elf -g
ENTRY_POINT = 0xc0001500
LIB = -I lib/kernel/ -I lib/ -I kernel/
GCC_PARAM = -m32 $(LIB) -c -fno-builtin -Wall -W -Wstrict-prototypes -Wmissing-prototypes -g
LD_PARAM = -m elf_i386 -Ttext $(ENTRY_POINT) -e main
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/print.o $(BUILD_DIR)/init.o \
	$(BUILD_DIR)/interrupt.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/debug.o
# HD60M_PATH = /home/fujinhang/OS/bochs-gdb/hd60M.img
HD60M_PATH = ./hd60M.img

%.bin : %.s
	nasm $(ASS_PARAM) -I $(include_path) -o $@ $<

loader : mbr.bin loader.bin

# 编译lib/kernel/print.s 生成目标文件
$(BUILD_DIR)/%.o	: lib/kernel/%.s
	nasm $(ASS_PARAM) -o $@ $<

# 编译kernel/kernel.s生成目标文件

$(BUILD_DIR)/%.o	: kernel/%.s
	nasm $(ASS_PARAM) -o $@ $<

# 编译kernel/*.c文件 生成目标文件
$(BUILD_DIR)/%.o : kernel/%.c
	gcc-4.4 -c -o $@ $< $(GCC_PARAM)

# 编译lib/kernel/debug.c 生成目标文件
$(BUILD_DIR)/%.o : lib/kernel/%.c
	gcc-4.4 -c -o $@ $< $(GCC_PARAM)

# 链接顺序必须是main在前，print在后，main.c 文件中用到了 print.o 中的 put_char函数，在链接顺序上，
# 属于“调用在前，实现在后”的顺序。如果将 print.o 放在前面， main.o 放在后面，也就是实现在前，调用
# 在后，此时生成的可执行文件起始虚拟地址并不准确，会有向后顺延的现象，并且 segment 的数量也不一样。
# 链接的顺序一定要 先调用，后实现，就是调用的.o文件要在主函数后面
$(BUILD_DIR)/kernel.bin : $(OBJS)
	ld $^ $(LD_PARAM)  -o $@ 

kernel : $(BUILD_DIR)/kernel.bin

write2hd : 
	dd if=./mbr.bin of=$(HD60M_PATH) bs=512 count=1 conv=notrunc
	dd if=./loader.bin of=$(HD60M_PATH) bs=512 count=4 seek=2 conv=notrunc
	dd if=./$(BUILD_DIR)/kernel.bin of=$(HD60M_PATH) bs=512 count=200 seek=9 conv=notrunc

gdb_symbol :
	objcopy --only-keep-debug $(BUILD_DIR)/kernel.bin $(BUILD_DIR)/kernel.sym

openOS :  
# bochs -q -f ../bochs-gdb/bochsrc.disk 
	bochs -q -f ./bochsrc.disk 

install : loader write2hd

clean : 
	find ../os_from_zero -type f \( -name "*.bin" -o -name "*.o" -o -name "*.sym" \) -exec rm {} \;

all : clean kernel install gdb_symbol


debug:
	gdb -ex "target remote localhost:1234" -ex "symbol-file /home/fujinhang/OS/os_from_zero/build/kernel.sym"
 
.PHONY : mbr debug clean loader write2hd openOS install kernel all gdb_symbol