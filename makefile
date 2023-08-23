
include_path = include/
ASS_PARAM = -I $(include_path)
GCC_PARAM = -m32
LD_PARAM = -m elf_i386 -Ttext 0xc0001500 

%.bin : %.s
	nasm $(ASS_PARAM) -o $@ $<

loader : mbr.bin loader.bin

kernel/%.o : kernel/%.c
	gcc-4.4 -c -o $@ $< $(GCC_PARAM)

kernel/kernel.bin : kernel/main.o
	ld $< $(LD_PARAM) -e main -o $@ 

kernel : kernel/kernel.bin


write2hd : 
	dd if=./mbr.bin of=./hd60M.img bs=512 count=1 conv=notrunc
	dd if=./loader.bin of=./hd60M.img bs=512 count=4 seek=2 conv=notrunc
	dd if=./kernel/kernel.bin of=./hd60M.img bs=512 count=200 seek=9 conv=notrunc

openOS :
	bochs -f bochsrc.disk

install : loader write2hd

clean : 
	rm -rf *.bin
	rm -rf kernel/*.bin kernel/*.o

debug : 
	echo $(COMPLE_PARAM)

 
.PHONY : mbr debug clean loader write2hd openOS install kernel