
include_path = include/
COMPILE_PARAM = -I $(include_path)

%.bin : %.s
	nasm $(COMPILE_PARAM) -o $@ $<

loader : mbr.bin loader.bin

write2hd : 
	dd if=./mbr.bin of=./hd60M.img bs=512 count=1 conv=notrunc
	dd if=./loader.bin of=./hd60M.img bs=512 count=4 seek=2 conv=notrunc

openOS :
	bochs -f bochsrc.disk

install : loader write2hd

clean : 
	rm -rf *.bin

debug : 
	echo $(COMPLE_PARAM)

 
.PHONY : mbr debug clean loader write2hd openOS install