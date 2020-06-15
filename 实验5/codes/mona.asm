BITS 16
extern main
extern lyfnheader
extern new_layfun
extern clock_hotwheel
extern clock_time
extern ouch_color
extern clock_ouch
extern clock
global _start
global _put
global _move
global _get
global _cls
global _callf
global _display
global _get_time
global _time
global _set_int
global _clock
_start:
    push dword ebp
    mov ax, cs
    mov ds, ax
    mov bx, ss
    mov ss, ax
    mov ebp, esp
    mov esp, 0xffff
    push word bx
    push dword ebp
    call dword main
    mov bx, word[esp+4]
    pop dword esp
    mov ss, bx
    pop dword ebp
    retf
_put:
    mov	al, byte[esp+4]       ; 字符=第一个参数
    mov	bx, 0007h             ; 页号为0(BH = 0) 黑底白字(BL = 07h)
    mov cx, 1                 ; 重复次数
    mov ah, 9                 ; 功能号
    int 10h                   ; 打印字符
    o32 ret
_move:
    mov dl, byte[esp+4]       ; x
    mov dh, byte[esp+8]       ; y
    mov bh, 0                 ; 显示页码
    mov ah, 2                 ; 功能号
    int 10h                   ; 移动光标
    o32 ret
_get:
    mov ah, 1
    int 16h
    jz _get
    mov ah, 0
    int 16h
    o32 ret
_cls:
    mov ax, 3
    int 10h
    o32 ret
_callf:
    xor ax, ax                ; AX = 0
    mov es, ax                ; ES = 0
    mov eax, dword[es:36]     ; 设置中断向量的偏移地址
    mov ax, 0xb00
    mov es, ax                ; es:bx
    mov cl, byte[esp+4]       ; 扇区号
    mov ch, byte[esp+8]       ; 柱面号 ; 起始编号为0
    mov dh, byte[esp+12]      ; 磁头号 ; 起始编号为0
    mov al, byte[esp+16]      ; 扇区数
    mov bx, 0x100             ; 偏移地址; 存放数据的内存偏移地址
    mov ah, 2                 ; 功能号
    mov dl, 0                 ; 驱动器号 ; 软盘为0，硬盘和U盘为80H
    int 13H                   ; 调用读磁盘BIOS的13h功能
    mov dword[_press], 0
    push dword 9
    push dword _ouch
    push dword 0x800
    call dword _set_int
    add esp, 12
    call 0xb00:0x100          ; 0xb100
    mov ax, cs                ; 恢复段地址
    mov ds, ax
    push dword 9
    push dword _int_9_ip
    push dword _int_9_cs
    call dword _set_int
    add esp, 12
    o32 ret
_display:
    mov eax, dword[esp+12]
    mov ebx, 80
    mul ebx
    add eax, dword[esp+8]
    mov ebx, 2
    mul ebx
    mov ebx, eax
    mov	ax, 0B800h            ; 文本窗口显存起始地址
    mov	gs, ax                ; GS = B800h
    mov ah, byte[esp+16]      ; 0000：黑底、1111：亮白字（默认值为07h）
    mov al, byte[esp+4]       ; AL = 显示字符值（默认值为20h=空格符）
    mov [gs:ebx], ax          ; 屏幕第 x 行, 第 y 列
    o32 ret
_get_time:
    mov al, 0                 ; Get seconds (00 to 59)
    out 0x70, al
    in al, 0x71
    mov byte[_second], al

    mov al, 0x02              ; Get minutes (00 to 59)
    out 0x70, al
    in al, 0x71
    mov byte[_minute], al

    mov al, 0x04              ; Get hours (see notes)
    out 0x70, al
    in al, 0x71
    mov byte[_hour], al

    mov al, 0x07              ; Get day of month (01 to 31)
    out 0x70, al
    in al, 0x71
    mov byte[_day], al

    mov al, 0x08              ; Get month (01 to 12)
    out 0x70, al
    in al, 0x71
    mov byte[_month], al

    mov al, 0x09              ; Get year (00 to 99)
    out 0x70, al
    in al, 0x71
    mov byte[_year], al
    o32 ret
_save:
    push word ds
    push word cs
    pop word ds
    pop word [_ds]
    pop dword [_ret_addr]
    pop word [_ip]
    pop word [_cs]
    pop word [_flags]
    mov word [_es], es 
    mov word [_ss], ss 
    mov word [_fs], fs 
    mov word [_gs], gs
    mov dword [_ax], eax
    mov dword [_bx], ebx
    mov dword [_cx], ecx
    mov dword [_dx], edx
    mov dword [_sp], esp
    mov dword [_bp], ebp
    mov dword [_si], esi
    mov dword [_di], edi
    mov ax, cs
    mov ss, ax
    mov esp, 0x100
    push dword [_ret_addr]
    o32 ret
_restart:
    pop dword [_ret_addr]
    mov eax, dword [_ax]
    mov ebx, dword [_bx]
    mov ecx, dword [_cx]
    mov edx, dword [_dx]
    mov esp, dword [_sp]
    mov ebp, dword [_bp]
    mov esi, dword [_si]
    mov edi, dword [_di]
    mov es, word [_es] 
    mov ss, word [_ss] 
    mov fs, word [_fs] 
    mov gs, word [_gs]
    push word [_flags]
    push word [_cs]
    push word [_ip]
    push dword [_ret_addr]
    mov ds, word [_ds]
    o32 ret
_set_int:
    cli
    xor ax, ax                ; AX = 0
    mov es, ax                ; ES = 0
    mov ax, word[esp+8]
    mov ebx, dword[esp+12]
    shl ebx, 2
    mov word[es:ebx], ax      ; 设置中断向量的偏移地址
    mov ax, word[esp+4]
    mov word[es:ebx+2], ax    ; 设置中断向量的段地址
    sti
    o32 ret
_clock:
    call dword _save
    dec dword[_count]         ; 递减计数变量
    jnz _clock_end            ; >0：跳转
    call dword clock
    mov dword[_count], _delay ; 重置计数变量=初值delay
_clock_end:
    mov al, 20h               ; AL = EOI
    out 20h, al               ; 发送EOI到主8529A
    out 0A0h, al              ; 发送EOI到从8529A
    call dword _restart
    iret                      ; 从中断返回
_ouch:
    call dword _save
    in al, 60h
    pushf
    pushf
    pop ax
    and ah, 0b11111100
    push ax
    popf
    call _int_9_cs:_int_9_ip
    cmp dword[_press], 0
    jz _ouch_end
    mov dword[ouch_color], 0b11100100
    push dword [lyfnheader]
    push dword 4
    push dword clock_ouch
    call dword new_layfun
    add esp, 12
_ouch_end:
    xor dword[_press], 0xffff
    mov al, 20h               ; AL = EOI
    out 20h, al               ; 发送EOI到主8529A
    out 0A0h, al              ; 发送EOI到从8529A
    call dword _restart
    iret                      ; 从中断返回
_data_define:
    _count dd _delay          ; 计时器计数变量，初值=delay
    _press dd 0
_time:
    _year db 0
    _month db 0
    _day db 0
    _hour db 0
    _minute db 0
    _second db 0
_register:
    _ax dd 0
    _bx dd 0
    _cx dd 0
    _dx dd 0
    _sp dd 0
    _bp dd 0
    _si dd 0
    _di dd 0
    _ip dw 0
    _flags dw 0
    _es dw 0
    _cs dw 0
    _ss dw 0
    _ds dw 0
    _fs dw 0
    _gs dw 0
_ret_addr: dd 0
_delay equ 4
_int_9_cs equ 0xf000
_int_9_ip equ 0xe987