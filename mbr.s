%include "boot.inc"

SECTION MBR vstart=0x7c00
	;初始化相关的寄存器
	mov ax, cs
	mov ds, ax
	mov es, ax					;这里为什么初始化扩展寄存器是因为后面可能要用到这个
	mov ss, ax
	mov fs, ax
	mov sp, 0x7c00				;mbr在内存中的地址
	mov ax, 0xb800
	mov gs, ax					;显示器的地址


	;清屏，利用系统调用功能号0x06，上卷全部行
	;int 0x10 功能号0x06
	;ah 存储功能号
	;al 上卷的行数，如果为0，表示全部
	;bh 上卷行的属性
	;cl, ch 窗口左上角的坐标(X, Y)
	;dl, dh 窗口右下角的坐标(X, Y)
	;显示器最多80列，25行共2000个字符，每个字符用两个字节表示所以每页是4000字节
	;第一个字节代表颜色等信息，第二个是字符
	mov ax, 0x0600
	mov bx, 0x0700
	xor cx, cx
	mov dx, 0x184f
	
	int 0x10

	;输出字符
	mov byte [gs:0x00], '1'
	mov byte [gs:0x01], 0xA4
	
	mov byte [gs:0x02], ' '
	mov byte [gs:0x03], 0xA4
	
	mov byte [gs:0x04], 'M'
	mov byte [gs:0x05], 0xA4
	
	mov byte [gs:0x06], 'B'
	mov byte [gs:0x07], 0xA4
	
	mov byte [gs:0x08], 'R'
	mov byte [gs:0x09], 0xA4


	;开始进行加载boot
	mov eax, LOADER_START_SECTOR	;要读的地址
	mov bx, LODAER_BASE_ADDR		;需要加载到内存的地址0x900
	mov	cx, 4						;读入的扇区数量
	call rd_disk_m_16


	jmp LODAER_BASE_ADDR
rd_disk_m_16:
	;保存寄存器的值
	mov esi, eax
	mov di, cx
	;从硬盘读入MBR
	;读入的顺序
	;1、设置要读入的扇区数
	;2、告诉硬盘读入数据的地址，说全就是往该通道的三个LBA寄存器写入起始地址的低24位
	;3、往device的低4位写入LBA的24-27位，置第六位为1，选择LBA模式，置第四位为0，选择master硬盘
	;4、往command命令寄存器中写入操作命令（说明你要干啥）
	;5、读取status寄存器看看你的命令执行完了没有
	;6、如果是读命令将硬盘数据读出，否则写完收工
	
	;步骤1
	mov dx, 0x1f2
	mov al, cl
	out dx, al
	
	;恢复eax,eax存的是要读的扇区索引，这里是第二个扇区
	mov eax, esi
	;步骤2
	;7~0位写入LBA地址
	mov dx, 0x1f3
	mov al, bl
	out dx, al

	mov cl, 8
	shr eax, cl
	mov dx, 0x1f4
	out dx, al

	shr eax, cl
	mov dx, 0x1f5
	out dx, al

	;步骤3
	shr eax, cl
	and al, 0x0f
	or al, 0xe0
	mov dx, 0x1f6
	out dx, al

	;步骤4，写入读命令0x20
	xor eax, eax
	mov al, 0x20
	mov dx, 0x1f7
	out dx, al

	;步骤5，检测硬盘status状态,若第3位为1表示可以输出，第7位为1表示硬盘正忙
.notready:
	nop
	xor eax, eax
	in al, dx
	and al, 0x88
	cmp al, 0x88
	jnz .notready

	;步骤6，读出数据
	mov ax, di
	mov dx, 256
	mul dx
	mov cx, ax 			;算出要读多少次，读一次2个字节，一共ax个扇区
	mov dx, 0x1f0

.go_on_read:
	in ax, dx
	mov [bx], ax
	add bx, 2
	loop .go_on_read
	ret

	times 510-($-$$) db 0
	db 0x55, 0xaa
