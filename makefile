
include_path = include/
ASS_PARAM = -I $(include_path)
GCC_PARAM = -m32 -I lib/kernel/ -I lib/ -I kernel/ -c -fno-builtin
LD_PARAM = -m elf_i386 -Ttext 0xc0001500 

%.bin : %.s
	nasm $(ASS_PARAM) -o $@ $<

loader : mbr.bin loader.bin

# 编译lib/kernel/print.s 生成目标文件
build/%.o	: lib/kernel/%.s
	nasm -f elf -o $@ $<

# 编译kernel/kernel.s生成目标文件

build/%.o	: kernel/%.s
	nasm -f elf -o $@ $<

# 编译lib/kernel/prinf.s 生成目标文件
build/%.o : kernel/%.c
	gcc-4.4 -c -o $@ $< $(GCC_PARAM)

# 链接顺序必须是main在前，print在后，main.c 文件中用到了 print.o 中的 put_char函数，在链接顺序上，
# 属于“调用在前，实现在后”的顺序。如果将 print.o 放在前面， main.o 放在后面，也就是实现在前，调用
# 在后，此时生成的可执行文件起始虚拟地址并不准确，会有向后顺延的现象，并且 segment 的数量也不一样。
# 链接的顺序一定要 先调用，后实现，就是调用的.o文件要在主函数后面
build/kernel.bin : build/main.o build/print.o build/init.o build/interrupt.o build/kernel.o
	ld $^ $(LD_PARAM) -e main -o $@ 

kernel : build/kernel.bin

write2hd : 
	dd if=./mbr.bin of=./hd60M.img bs=512 count=1 conv=notrunc
	dd if=./loader.bin of=./hd60M.img bs=512 count=4 seek=2 conv=notrunc
	dd if=./build/kernel.bin of=./hd60M.img bs=512 count=200 seek=9 conv=notrunc

openOS :
	bochs -f bochsrc.disk

install : loader write2hd

clean : 
	rm -rf *.bin
	rm -rf kernel/*.bin kernel/*.o
	rm -rf lib/kernel/*.o
	rm -rf build/*.o

all : clean kernel install

debug : 
	echo $(COMPLE_PARAM)

 
.PHONY : mbr debug clean loader write2hd openOS install kernel all