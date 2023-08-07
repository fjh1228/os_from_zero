
LOADER_BASE_ADDR equ 0x900              ;将loader放在0x900的位置
LOADER_START_SECTOR equ 0x2             ;选择硬盘的第二个sector作为存放扇区，和第0扇区间隔了一个扇区

;----------------------------GDT----------------------------

DESC_G_4K       equ     1_0000_0000_0000_0000_0000_00b
DESC_DB_32      equ     1_0000_0000_0000_0000_0000_0b
DESC_L          equ     0_0000_0000_0000_0000_0000b
DESC_AVL        equ     0_0000_0000_0000_0000_000b

DESC_LIMIT_CODE2 equ    1111_0000_0000_0000_000b
DESC_LIMIT_DATA2 equ   DESC_LIMIT_CODE2
DESC_LIMIT_VIDEO2 equ   0000_0000_0000_0000_000b

DESC_P          equ     0_0000_0000_0000_00b
DESC_DPL_0      equ     00_0000_0000_0000b
DESC_DPL_1      equ     01_0000_0000_0000b
DESC_DPL_2      equ     10_0000_0000_0000b
DESC_DPL_3      equ     11_0000_0000_0000b

DESC_S_SYS      equ     0_0000_0000_000b    
DESC_S_CODE     equ     1_0000_0000_000b
DESC_S_DATA    equ     DESC_S_CODE
;x=1,c=0,r=0,a=0 代码段可执行，非一致性，不可读，已访问位清0
DESC_TYPE_CODE  equ     1000_0000_0000b
;x=0,e=0,w=1,a=0 数据段不可执行，向上扩展，可写，已访问位清零
DESC_TYPE_DATA  equ     0010_0000_0000b

DESC_CODE_HIGH4 equ     (0x00) << 24 + DESC_G_4K + DESC_DB_32 + DESC_L + DESC_AVL + DESC_LIMIT_CODE2 +\
                        DESC_P + DESC_DPL_0 + DESC_S_CODE + DESC_TYPE_CODE + 0x00

DESC_DATA_HIGH4 equ     (0x00) << 24 + DESC_G_4K + DESC_DB_32 + DESC_L + DESC_AVL + DESC_LIMIT_DATA2 +\
                        DESC_P + DESC_DPL_0 + DESC_S_DATA + DESC_TYPE_DATA + 0x00


DESC_VIDEO_HIGH4 equ     (0x00) << 24 + DESC_G_4K + DESC_DB_32 + DESC_L + DESC_AVL + DESC_LIMIT_VIDEO2 +\
                        DESC_P + DESC_DPL_0 + DESC_S_DATA + DESC_TYPE_DATA + 0x00

;------------------------------------------选择子----------------------------


PRL_0               equ     00b
PRL_1               equ     01b
PRL_2               equ     10b
PRL_3               equ     11b
TI_GDT              equ     000b
TI_LDT              equ     100b