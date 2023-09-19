
include_path = include/
COMPILE_PARAM = -I $(include_path)
img_path = /home/fujinhang/OS/bochs-gdb/hd60M.img
bochs_path = /home/fujinhang/OS/bochs-gdb

%.bin : %.s
	nasm $(COMPILE_PARAM) -o $@ $<

loader : mbr.bin loader.bin

write2hd : 
	dd if=./mbr.bin of=$(img_path)/hd60M.img bs=512 count=1 conv=notrunc
	dd if=./loader.bin of=$(img_path)/hd60M.img bs=512 count=1 seek=2 conv=notrunc

openOS :
	bochs -f $(bochs_path)/bochsrc.disk

install : loader write2hd

clean : 
	rm -rf *.bin
	rm -rf *.out
debug : 
	

 
.PHONY : mbr debug clean loader write2hd openOS install