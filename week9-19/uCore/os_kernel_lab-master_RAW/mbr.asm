;主引导程序
  ;------------------------------------------------------------
  SECTION MBR vstart=0x7c00 ; 起始地址编译为0x7c00
    mov ax,cs   ; 此时的cs为0，用0来初始化所有的段寄存器
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov fs,ax
    mov sp,0x7c00 ; 0x7c00 以下空间暂时安全，故可用做栈。
    mov ax,0xb800 ; 0xb800-0xbffff 用于文本模式显示适配器
    mov gs,ax

  ; 清屏 利用0x06号功能，上卷全部行，则可清屏。
  ; -----------------------------------------------------------
  ;INT 0x10   功能号:0x06   功能描述:上卷窗口
  ;------------------------------------------------------
  ;输入：
  ;AH 功能号= 0x06
  ;AL = 上卷的行数(如果为0,表示全部)
  ;BH = 上卷行属性
  ;(CL,CH) = 窗口左上角的(X,Y)位置
  ;(DL,DH) = 窗口右下角的(X,Y)位置
  ;无返回值：
    mov     ax, 0x600
    mov     bx, 0x700
    mov     cx, 0          ; 左上角: (0, 0)
    mov     dx, 0x184f     ; 右下角: (80,25),
          ; VGA文本模式中,一行只能容纳80个字符,共25行。
          ; 下标从0开始,所以0x18=24,0x4f=79
    int     0x10            ; int 0x10
    
    ; 输出背景色绿色，前景色红色，并且跳动的字符串“1 MBR"
    mov byte [gs:0x00], '1'
    mov byte [gs:0x01], 0xa4   ; A表示绿色背景闪烁，4表示前景色为红色
    mov byte [gs:0x02], ' '
    mov byte [gs:0x03], 0xa4
    mov byte [gs:0x04], 'M'
    mov byte [gs:0x05], 0xa4
    mov byte [gs:0x06], 'B'
    mov byte [gs:0x07], 0xa4
    mov byte [gs:0x08], 'R'
    mov byte [gs:0x09], 0xa4
    jmp $   ; 始终跳转到这条代码，为死循环，使程序悬停在此

    message db "杨涛 kreios"
    ; 用\0 将剩余空间填满
    times 510-($-$$) db 0
    ; 最后两位一定是0x55, 0xaa
    db 0x55,0xaa