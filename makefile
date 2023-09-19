
include_path = include/
COMPILE_PARAM = -I $(include_path)
img_path = /home/fujinhang/OS/bochs-gdb
bochs_path = /home/fujinhang/OS/bochs-gdb

%.bin : %.s
	nasm $(COMPILE_PARAM) -o $@ $<

loader : mbr.bin loader.bin

write2hd : 
	dd if=./mbr.bin of=$(img_path)/hd60M.img bs=512 count=1 conv=notrunc
	dd if=./loader.bin of=$(img_path)/hd60M.img bs=512 count=1 seek=2 conv=notrunc

openOS :
	bochs -q -f $(bochs_path)/bochsrc.disk

install : loader write2hd

clean : 
	rm -rf *.bin
	rm -rf *.out
debug : 
	gdb -ex "target remote localhost:1234"
# gdb -ex "target remote localhost:1234" -ex "symbol-file /home/fujinhang/OS/os_from_zero/build/kernel.sym"
 
.PHONY : mbr debug clean loader write2hd openOS install