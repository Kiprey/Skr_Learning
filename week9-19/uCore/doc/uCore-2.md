# uCore实验 - Lab2

## 简介

- 这里是笔者在完成`uCore` Lab 2时写下的一些笔记
- 内容涉及段页式存储管理、页机制以及uCore页目录与页表结构等

## 1. 知识点

### 1) 物理内存探测

- 操作系统需要知道了解整个计算机系统中的物理内存如何分布的，哪些被可用，哪些不可用。其基本方法是通过BIOS中断调用来帮助完成的。`bootasm.S`中新增了一段代码，使用BIOS中断检测物理内存总大小。

- 在讲解该部分代码前，先引入一个结构体

    ```cpp
    struct e820map {      // 该数据结构保存于物理地址0x8000
        int nr_map;       // map中的元素个数
        struct {
            uint64_t addr;    // 某块内存的起始地址
            uint64_t size;    // 某块内存的大小
            uint32_t type;    // 某块内存的属性。1标识可被使用内存块；2表示保留的内存块，不可映射。
        } __attribute__((packed)) map[E820MAX];
    };
    ```

- 以下是bootasm.S中新增的代码，详细信息均以注释的形式写入代码中。

    ```x86asm
    probe_memory:
        movl $0, 0x8000   # 初始化，向内存地址0x8000，即uCore结构e820map中的成员nr_map中写入0
        xorl %ebx, %ebx   # 初始化%ebx为0，这是int 0x15的其中一个参数
        movw $0x8004, %di # 初始化%di寄存器，使其指向结构e820map中的成员数组map
    start_probe:
        movl $0xE820, %eax  # BIOS 0x15中断的子功能编号 %eax == 0xE820
        movl $20, %ecx    # 存放地址范围描述符的内存大小，至少20
        movl $SMAP, %edx  # 签名， %edx == 0x534D4150h("SMAP"字符串的ASCII码)
        int $0x15     # 调用0x15中断
        jnc cont      # 如果该中断执行失败，则CF标志位会置1，此时要通知UCore出错
        movw $12345, 0x8000 # 向结构e820map中的成员nr_map中写入特殊信息，报告当前错误
        jmp finish_probe    # 跳转至结束，不再探测内存
    cont:
        addw $20, %di   # 如果中断执行正常，则目标写入地址就向后移动一个位置
        incl 0x8000     # e820::nr_map++
        cmpl $0, %ebx   # 执行中断后，返回的%ebx是原先的%ebx加一。如果%ebx为0，则说明当前内存探测完成
        jnz start_probe
    finish_probe:
    ```

- 这部分代码执行完成后，从BIOS中获得的内存分布信息以结构体`e820map`的形式写入至物理`0x8000`地址处。稍后ucore的page_init函数会访问该地址并处理所有的内存信息。

### 2) 链接地址

- 审计`lab2/tools/kernel.ld`这个链接脚本，我们可以很容易的发现，链接器设置kernel的链接地址(link address)为`0xC0100000`，这是个虚拟地址。在uCore的bootloader中，bootloader使用如下语句来加载kernel：

  ```cpp
  readseg(ph->p_va & 0xFFFFFF, ph->p_memsz, ph->p_offset);
  ```
  
  `0xC0010000 & 0xFFFFFF == 0x00100000`即kernel最终被装载入物理地址`0x10000`处，其相对偏移为`-0xc0000000`，与uCore中所设置的虚拟地址的偏移量相对应。
  
- 需要注意的是，在lab2的一些代码中会使用到如下两个变量，但这两个变量并没有被定义在任何C语言的源代码中：

  ```cpp
  extern char end[];
  extern char edata[];
  ```

  实际上，它们定义于`kernel.ld`这个链接脚本中

  ```cpp
  . = ALIGN(0x1000);
  .data.pgdir : {
      *(.data.pgdir)
  }
  
  PROVIDE(edata = .);
  
  .bss : {
      *(.bss)
  }
  
  PROVIDE(end = .);
  
  /DISCARD/ : {
      *(.eh_frame .note.GNU-stack)
  }
  // 脚本文件的结尾
  ```

  `edata`表示`kernel`的`data`段结束地址；`end`表示`bss`段的结束地址（即整个`kernel`的结束地址）

  `edata[]`和 `end[]`这些变量是ld根据kernel.ld链接脚本生成的全局变量，表示相应段的结束地址，它们不在任何一个.S、.c或.h文件中定义，但仍然可以在源码文件中使用。

### 3) uCore的内存空间布局

- 在uCore中，CPU先在bootasm.S（实模式）中通过调用BIOS中断，将物理内存的相关描述符写入特定位置`0x8000`，然后读入kernel至物理地址`0x10000`、虚拟地址`0xC0000000`。

- 而kernel在`page_init`函数中，读取物理内存地址`0x8000`处的内存，查找最大物理地址，并计算出所需的**页面数**。虚拟页表`VPT(Virtual Page Table)`的地址紧跟`kernel`，其地址为4k对齐。虚拟地址空间结构如下所示：

  ```cpp
  /* *
   * Virtual memory map:                                          Permissions
   *                                                              kernel/user
   *
   *     4G -----------> +---------------------------------+
   *                     |                                 |
   *                     |         Empty Memory (*)        |
   *                     |                                 |
   *                     +---------------------------------+ 0xFB000000
   *                     |   Cur. Page Table (Kern, RW)    | RW/-- PTSIZE
   *     VPT ----------> +---------------------------------+ 0xFAC00000
   *                     |        Invalid Memory (*)       | --/--
   *     KERNTOP ------> +---------------------------------+ 0xF8000000
   *                     |                                 |
   *                     |    Remapped Physical Memory     | RW/-- KMEMSIZE
   *                     |                                 |
   *     KERNBASE -----> +---------------------------------+ 0xC0000000
   *                     |                                 |
   *                     |                                 |
   *                     |                                 |
   *                     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   * (*) Note: The kernel ensures that "Invalid Memory" is *never* mapped.
   *     "Empty Memory" is normally unmapped, but user programs may map pages
   *     there if desired.
   *
   * */
  ```

  完成**物理内存页管理初始化工作**后，其物理地址的分布空间如下

  ```cpp
  +----------------------+ <- 0xFFFFFFFF(4GB)       ----------------------------  4GB
  |  一些保留内存，例如用于|                                保留空间
  |   32bit设备映射空间等  |
  +----------------------+ <- 实际物理内存空间结束地址 ----------------------------
  |                      |
  |                      |
  |     用于分配的         |                                 可用的空间
  |    空闲内存区域        |
  |                      |
  |                      |
  |                      |
  +----------------------+ <- 空闲内存起始地址      ----------------------------  
  |     VPT页表存放位置      |                                VPT页表存放的空间   (4MB左右)
  +----------------------+ <- bss段结束处           ----------------------------
  |uCore的text、data、bss |                              uCore各段的空间
  +----------------------+ <- 0x00100000(1MB)       ---------------------------- 1MB
  |       BIOS ROM       |
  +----------------------+ <- 0x000F0000(960KB)
  |     16bit设备扩展ROM  |                             显存与其他ROM映射的空间
  +----------------------+ <- 0x000C0000(768KB)
  |     CGA显存空间       |
  +----------------------+ <- 0x000B8000            ---------------------------- 736KB
  |        空闲内存       |
  +----------------------+ <- 0x00011000(+4KB)          uCore header的内存空间
  | uCore的ELF header数据 |
  +----------------------+ <-0x00010000             ---------------------------- 64KB
  |       空闲内存        |
  +----------------------+ <- 基于bootloader的大小          bootloader的
  |      bootloader的   |                                    内存空间
  |     text段和data段    |
  +----------------------+ <- 0x00007C00            ---------------------------- 31KB
  |   bootloader和uCore  |
  |      共用的堆栈       |                                 堆栈的内存空间
  +----------------------+ <- 基于栈的使用情况
  |     低地址空闲空间    |
  +----------------------+ <-  0x00000000           ---------------------------- 0KB
  ```

  易知，其页表地址之上的物理内存空间是空闲的（除去保留的内存），故将该物理地址之下的物理空间对应的页表全部设置为保留(reserved)。并将这些空闲的内存全部添加进页表项中。

### 4. 段页式存储管理（重要）

> 在保护模式中，x86 体系结构将内存地址分成三种：**逻辑地址**（也称**虚拟地址**）、**线性地址**和**物理地址**。

- 段式存储在内存保护方面有优势，页式存储在内存利用和优化转移到后备存储方面有优势。
- 在段式存储管理基础上，给每个段加一级页表。同时，通过指向相同的页表基址，实现进程间的段共享。
- 在段页式管理中，操作系统弱化了段式管理中的功能，实现以分页为主的内存管理。段式管理只起到了一个过滤的作用，它将地址不加转换直接映射成线性地址。将虚拟地址转换为物理地址的过程如下：
  - 根据段寄存器中的段选择子，获取GDT中的特定基址并加上目标偏移来确定**线性地址**。由于GDT中所有的基址全为0（因为弱化了段式管理的功能，对等映射），所以此时的逻辑地址和线性地址是相同的。
  - 根据该线性地址，获取对应页表项，并根据该页表项来获取对应的物理地址。
- **一级页表（页目录表PageDirectoryTable, PDT）的起始地址存储于`%cr3`寄存器中。**

#### a. 启动页机制（重要）

启动页机制的代码很简单，其对应的汇编代码为

```x86asm
# labcodes/lab2/kern/init/entry.S
kern_entry:
    # load pa of boot pgdir
    movl $REALLOC(__boot_pgdir), %eax
    movl %eax, %cr3
    # enable paging
    movl %cr0, %eax
    orl $(CR0_PE | CR0_PG | CR0_AM | CR0_WP | CR0_NE | CR0_TS | CR0_EM | CR0_MP), %eax
    andl $~(CR0_TS | CR0_EM), %eax
    movl %eax, %cr0

    # update eip
    # now, eip = 0x1xxxxx
    leal next, %eax
    # set eip = KERNBASE + 0x1xxxxx
    jmp *%eax
next:
  # .....省略剩余代码

# kernel builtin pgdir
# an initial page directory (Page Directory Table, PDT)
# These page directory table and page table can be reused!
.section .data.pgdir
.align PGSIZE
__boot_pgdir:
.globl __boot_pgdir
    # map va 0 ~ 4M to pa 0 ~ 4M (temporary)
    .long REALLOC(__boot_pt1) + (PTE_P | PTE_U | PTE_W)
    .space (KERNBASE >> PGSHIFT >> 10 << 2) - (. - __boot_pgdir) # pad to PDE of KERNBASE
    # map va KERNBASE + (0 ~ 4M) to pa 0 ~ 4M
    .long REALLOC(__boot_pt1) + (PTE_P | PTE_U | PTE_W)
    .space PGSIZE - (. - __boot_pgdir) # pad to PGSIZE

.set i, 0
__boot_pt1:
.rept 1024
    .long i * PGSIZE + (PTE_P | PTE_W)
    .set i, i + 1
.endr
```

- 首先，将一级页表 **__boot_pgdir**  （页目录表PDT）的**物理**基地址加载进`%cr3`寄存器中。

  - 该一级页表**暂时**将虚拟地址 **0xC0000000 + (0~4M)** 以及虚拟地址 **(0~4M)**   设置为物理地址 **(0-4M)** 。

    之后会重新设置一级页表的映射关系。

    > 为什么要将两段虚拟内存映射到同一段物理地址呢？思考一下，答案就在下方。

- 之后，设置`%cr0`寄存器中**PE、PG、AM、WP、NE、MP**位，关闭**TS** 与**EM** 位，以启动分页机制。

  > 先介绍一下`%cr0`寄存器主要3个标志位的功能：
  >
  > - **P**rotection **E**nable: 启动保护模式，默认只是打开分段。
  > - **P**a**g**ing: 设置分页标志。只有PE和PG位同时置位，才能启动分页机制。
  > - **W**rite **P**rotection: 当该位为1时，CPU会禁止ring0代码向read only page写入数据。这个标志位主要与**写时复制**有关系。
  >
  > 除**PE**、**PG**与**WP** 的**其他**标志位与分页机制关联不大，其设置或清除的原因盲猜可能是通过启动分页机制这个机会来**顺便做个初始化**。

  > 当改变PE和PG位时，必须小心。只有**当执行程序至少有部分代码和数据在线性地址空间和物理地址空间中具有相同地址时，我们才能改变PG位的设置**。
  >
  > 因为当`%cr0`寄存器一旦设置，则**分页机制立即开启**。此时这部分具有相同地址的代码在分页和未分页之间起着桥梁的作用。无论是否开启分页机制，这部分代码都必须具有相同的地址。
  >
  > 而这一步的操作能否成功，关键就在于一级页表的设置。一级页表将虚拟内存中的两部分地址**KERNBASE+(0-4M)** 与 **(0-4M)** 暂时都映射至物理地址 **(0-4M)** 处，这样就可以满足上述的要求。

- 最后，**必须**来个简单的`jmp`指令，将`eip`从物理地址“修改”为虚拟地址。

  > 在修改该了PE位之后程序必须立刻使用一条跳转指令，以刷新处理器执行管道中已经获取的不同模式下的任何指令。

#### b. uCore段页机制启动过程

- bootloader在启动保护模式前，会设置一个临时GDT以便于进入CPU保护模式后的bootloader和uCore所使用。
- uCore被bootloader加载进内存后，在`kern_entry`中启动页机制。
- 在`pmm_init`中建立双向链表来管理物理内存，并设置一级页表（页目录表）与二级页表。
- 最后重新加载新的GDT。

> lab2相对于lab1，新增了页机制相关的处理过程，其他过程没有改变。

#### c. uCore物理页结构

- uCore中用于管理物理页的结构如下所示

  ```cpp
  /* *
  * struct Page - Page descriptor structures. Each Page describes one
  * physical page. In kern/mm/pmm.h, you can find lots of useful functions
  * that convert Page to other data types, such as phyical address.
  * */
  struct Page {
    int ref;                // 当前页被引用的次数，与内存共享有关
    uint32_t flags;         // 标志位的集合，与eflags寄存器类似
    unsigned int property;  // 空闲的连续page数量。这个成员只会用在连续空闲page中的第一个page
    list_entry_t page_link; // 两个分别指向上一个和下一个非连续空闲页的指针。
  };
  ```

  目前在lab2中，flags可以设置的位只有`reserved`位和`Property`位。

  > `reserved`位表示当前页是否被保留，一旦保留该页，则该页无法用于分配；
  >
  > `Property`位表示当前页是否已被分配，为1则表示已分配。

- 所有的数据结构Page都存放在**一维Page结构数组**中。但请注意，这并非虚拟页表（VPT），即该**一维Page结构数组并非分页机制用于*将虚拟地址转换为物理地址*这个过程所用到的一级与二级页表**，它们只是用于设置对应物理页表的相关属性，例如当前物理页表被二级页表的引用次数等等。

- 同时，该一维Page结构数组的存储位置与虚拟页表VPT的存储位置不同。前者的起始存储地址为kernel结尾地址向上4k对齐后的第一个物理页面，而后者则存储于指定虚拟地址`0xFAC00000`。

- 页目录表使用**线性地址**的首部(PDX, 10bit)作为索引，二级页表使用**线性地址**的中部(PTX, 10bit)作为索引，而Page结构数组使用**物理地址**的首部与中部(PPN, 20bit)作为索引（注意是**物理**地址）。

- 为了加快查找，所有连续空闲pages中的第一个Page结构都会构成一个双向链表。相互链接，其第一个结点是`free_area.free_list`

  ```cpp
  /* free_area_t - maintains a doubly linked list to record free (unused) pages */
  typedef struct {
    list_entry_t free_list;         // the list header
    unsigned int nr_free;           // # of free pages in this free list
  } free_area_t;
  free_area_t free_area;
  ```

#### d. 虚拟页表结构

每个页表项（PTE）都由一个32位整数来存储数据，其结构如下

```cpp
      31-12      9-11     8    7    6   5   4      3    2   1   0
+--------------+-------+-----+----+---+---+-----+-----+---+---+---+
|     Offset   | Avail | MBZ | PS | D | A | PCD | PWT | U | W | P |
+--------------+-------+-----+----+---+---+-----+-----+---+---+---+
```

- 0 - **P**resent: 表示当前PTE所指向的物理页面是否驻留在内存中

- 1 - **W**riteable: 表示是否允许读写

- 2 - **U**ser: 表示该页的访问所需要的特权级。即User(ring 3)是否允许访问

- 3 - **P**age**W**rite**T**hough:  表示是否使用write through缓存写策略

- 4 - **P**age**C**ache**D**isable: 表示是否**不对**该页进行缓存

- 5 - **A**ccess: 表示该页是否已被访问过

- 6 - **D**irty: 表示该页是否已被修改

- 7 - **P**age**S**ize: 表示该页的大小

- 8 - **M**ust**B**e**Z**ero: 该位必须保留为0

- 9-11 - **Avail**able: 第9-11这三位并没有被内核或中断所使用，可保留给OS使用。

- 12-31 - Offset: 目标地址的后20位。

  > 因为目标地址以4k作为对齐标准，所以该地址的低12位永远为0，故这12位空间可用于设置标志位。

#### e. 自映射

- 自映射的好处

  - 当页目录与页表建立完成后，如果需要按虚拟地址的地址**顺序显示整个页目录表和页表的内容**，则要查找页目录表的页目录表项内容，并根据页目录表项内容找到页表的物理地址，再转换成对应的虚地址，然后访问页表的虚地址，搜索整个页表的每个页目录项。这样的过程比较繁琐，而自映射可以改善这个过程。
  - 节省4KB空间
  - 方便用户态程序访问页表，可以用这种方式实现一个用户地址空间的映射

- 页表自映射的关键点

  - 把所有的页表（4KB * 1024个）放到**连续**的4MB **虚拟地址** 空间中，并且要求这段空间4MB对齐，这样，**就会有一张虚拟页的内容与页目录的内容完全相同**。

  - 页目录结构必须和页表结构相同。

  - 此时在页目录表中，会存在一个页目录条目，该页目录条目指向某个二级页表。而该二级页表的物理地址，正是页目录表所处于物理页的**物理**地址。

    即，页目录表中存在一个页目录条目，该条目内含的物理地址就是页目录表本身的物理地址。

    uCore中的这条代码证实了这个结论:

    ```cpp
    // recursively insert boot_pgdir in itself
    // to form a virtual page table at virtual address VPT
    //
    // PDX(VPT)为4MB虚拟页表所对应的PageDirectoryIndex
    boot_pgdir[PDX(VPT)] = PADDR(boot_pgdir) | PTE_P | PTE_W;
    ```

    而下面这张图演示了其指向过程:

    > 注意页目录表此时存储于VPT的4MB范围中。

    ```mermaid
    graph LR;
    PDT-->PT1
    PDT-->PT2/PDT
    PT1-->PhyPage1
    PT1-->PhyPage2
    PT2/PDT-->PhyPage3/PT1
    PT2/PDT-->PhyPage4/PT2
    ```

- 参考：[页表自映射](https://blog.csdn.net/u010513059/article/details/80311248)

### 5) uCore栈的迁移

- 在原先的lab1中，bootloader所设置的栈起始地址为`0x7c00`，之后的uCore的代码也沿用了这个栈，但仍然划分出了一个全局数组作为TSS上的ring0栈地址（该全局数组位于uCore的bss段）。

  > 注意此时的**两个**内核栈是不一样的，一个是中断外使用的栈，另一个是中断内使用的栈。

- 而在lab2中，栈稍微做了一些改变。bootloader所设置的栈起始地址仍然为`0x7c00`，但在将uCore加载进内存之后，在`kern_entry`中，该部分代码在启动页机制后将栈设置为uCore data段上的一个全局数组的末尾地址`bootstacktop`（8KB），并也在`gdt_init`将TSS ring0栈地址设置为了`bootstacktop`。

  > 与Lab1不同，之后内核可以使用的内核栈只有一个。

  > 中断处理程序可能会从高地址开始，向下覆盖ring3的栈数据。这个漏洞可能是因为未完全实现的内存管理机制所导致的。

## 2. 练习解答

### 1) 练习0

> **填写已有实验**，将完成的实验1中的代码添加进实验2中。

这个没什么好说的，一个个照搬就成。

### 2) 练习1

> **实现 first-fit 连续物理内存分配算法**。

原先的uCore实验2代码中几乎已经完全实现了first-fit算法，但其中仍然存在一个问题，以至于无法通过check。什么问题呢：

> `first-fit`算法要求将空闲内存块**按照地址从小到大的方式**连起来。

但uCore中的代码没有实现这一点。所以要手动修改相关的代码。

- `default_init_memmap`

  - 该函数将新页面插入链表时，没有按照地址顺序插入

    ```cpp
    list_add(&free_list, &(base->page_link));
    ```

  - 故需要修改该行代码，使其按地址顺序插入至双向链表中。

    ```cpp
    list_add_before(&free_list, &(base->page_link));
    ```

- `default_alloc_pages`

  - 在原先的代码中，当获取到了一个大小足够大的页面地址时，程序会先将该页头从链表中断开，切割，并将剩余空间放回链表中。但将*剩余空间放回链表*时，并没有按照地址顺序插入链表。

    > 连续空闲页面中的第一个页称为页头，page header。

    ```cpp
    if (page != NULL) {
        list_del(&(page->page_link));
        if (page->property > n) {
            struct Page *p = page + n;
            p->property = page->property - n;
            // 注意这一步
            list_add(&free_list, &(p->page_link));
        }
        nr_free -= n;
        ClearPageProperty(page);
    }
    ```

  - 以下是修改后的代码

    ```cpp
    if (page != NULL) {
        if (page->property > n) {
            struct Page *p = page + n;
            p->property = page->property - n;
            SetPageProperty(p);
            // 注意这一步add after
            list_add_after(&(page->page_link), &(p->page_link));
        }
        list_del(&(page->page_link));
        nr_free -= n;
        ClearPageProperty(page);
    }
    ```

- `default_free_pages`

  - 该函数默认会在函数末尾处，将待释放的页头插入至链表的第一个节点。

    ```cpp
    list_add(&free_list, &(base->page_link));
    ```

  - 所以我们需要修改这部分代码，使其按地址顺序插入至对应的链表结点处。

    ```cpp
    // 将空闲页面按地址大小插入至链表中
    for(le = list_next(&free_list); le != &free_list; le = list_next(le))
    {
        p = le2page(le, page_link);
        if (base + base->property <= p) {
            assert(base + base->property != p);
            break;
        }
    }
    list_add_before(le, &(base->page_link));
    ```

### 3) 练习2

> **实现寻找虚拟地址对应的页表项**.

> 通过设置页表和对应的页表项，可建立虚拟内存地址和物理内存地址的对应关系。
>
> 其中的`get_pte`函数是设置页表项环节中的一个重要步骤。此函数找到一个虚地址对应的二级页表项的内核虚地址，如果此二级页表项不存在，则分配一个包含此项的二级页表。

以下为实现的代码

```cpp
pte_t * get_pte(pde_t *pgdir, uintptr_t la, bool create) {
    // 获取传入的线性地址中所对应的页目录条目的物理地址
    pde_t *pdep = &pgdir[PDX(la)];
    // 如果该条目不可用(not present)
    if (!(*pdep & PTE_P)) {
        struct Page *page;
        // 如果分配页面失败，或者不允许分配，则返回NULL
        if (!create || (page = alloc_page()) == NULL)
            return NULL;
        // 设置该物理页面的引用次数为1
        set_page_ref(page, 1);
        // 获取当前物理页面所管理的物理地址
        uintptr_t pa = page2pa(page);
        // 清空该物理页面的数据。需要注意的是使用虚拟地址
        memset(KADDR(pa), 0, PGSIZE);
        // 将新分配的页面设置为当前缺失的页目录条目中
        // 之后该页面就是其中的一个二级页面
        *pdep = pa | PTE_U | PTE_W | PTE_P;
    }
    // 返回在pgdir中对应于la的二级页表项
    return &((pte_t *)KADDR(PDE_ADDR(*pdep)))[PTX(la)];
}
```

- 请描述页目录项（Pag Director Entry）和页表（Page Table Entry）中每个组成部分的含义和以及对ucore而言的潜在用处。

  > 请查看[虚拟页表结构](#d-虚拟页表结构)

- 如果ucore执行过程中访问内存，出现了页访问异常，请问硬件要做哪些事情？

  > 以下答案参考了其他blog，具体细节留待以后再来研究。

  - 将引发页访问异常的地址将被保存在cr2寄存器中
  - 设置错误代码
  - 引发Page Fault，将外存的数据换到内存中
  - 进行上下文切换，退出中断，返回到中断前的状态

### 4) 练习3

> **释放某虚地址所在的页并取消对应二级页表项的映射**.

> 当释放一个包含某虚地址的物理内存页时，需要让对应此物理内存页的管理数据结构Page做相关的清除处理，使得此物理内存页成为空闲；另外还需把表示虚地址与物理地址对应关系的二级页表项清除。

这个练习不是很难，对着注释完成即可。以下为实现的代码：

```cpp
//page_remove_pte - free an Page sturct which is related linear address la
//                - and clean(invalidate) pte which is related linear address la
//note: PT is changed, so the TLB need to be invalidate
static inline void
page_remove_pte(pde_t *pgdir, uintptr_t la, pte_t *ptep) {
    // 如果传入的页表条目是可用的
    if (*ptep & PTE_P) {
        // 获取该页表条目所对应的地址
        struct Page *page = pte2page(*ptep);
        // 如果该页的引用次数在减1后为0
        if (page_ref_dec(page) == 0)
            // 释放当前页
            free_page(page);
        // 清空PTE
        *ptep = 0;
        // 刷新TLB内的数据
        tlb_invalidate(pgdir, la);
    }
}
```

- 数据结构Page的全局变量（其实是一个数组）的每一项与页表中的页目录项和页表项有无对应关系？如果有，其对应关系是啥？

  - 当页目录项或页表项有效时，Page数组中的项与页目录项或页表项存在对应关系。

  - 页目录表中存放着数个页表条目PTE，这些页表条目中存放了某个二级页表所在物理页的信息，包括该二级页表的**物理地址**，但使用**线性地址**的头部PDX(Page Directory Index)来索引页目录表。

    > 总结一下，页目录表内存放二级页表的**物理地址**，但却使用**线性地址**索引页目录表中的条目。

  - 而页表（二级页表）与页目录（一级页表）具有类似的特性，页表中的页表项指向所管理的物理页的**物理地址**（不是数据结构Page的地址），使用线性地址的中部PTX(Page Table Index)来索引页表。

  - 当二级页表获取物理页时，需要对该物理页所对应的数据结构page来做一些操作。其操作包括但不限于设置引用次数，这样方便共享内存。

  > 为什么页目录表中存放的是**物理**地址呢？可能是为了防止递归查找。
  >
  > 即原先查找页目录表的目的是想将某个线性地址转换为物理地址，但如果页目录表中存放的是二级页表的**线性**地址，则需要先查找该二级页表的物理地址，此时需要递归查找，这可能会出现永远也查找不到物理地址的情况。

- 如果希望虚拟地址与物理地址相等，则需要如何修改lab2，完成此事？ **鼓励通过编程来具体完成这个问题**

  - 将`labcodes/lab2/tools/kernel.ld`中的加载地址从`0xC0100000`修改为`0x0`

    ```cpp
    // 修改前
    . = 0xC0100000;
    // 修改后
    . = 0x0;
    ```

  - 将`mm/`中的内核偏移地址修改为0

    ```cpp
    // 修改前
    #define KERNBASE            0xC0000000
    // 修改后
    #define KERNBASE            0x0
    ```

  - 最后一步，但也是必须要做的一步——**关闭页机制**。将开启页机制的那一段代码删除或注释掉最后一句即可。

    ```x86asm
    # 修改后
    movl %cr0, %eax
    orl $(CR0_PE | CR0_PG | CR0_AM | CR0_WP | CR0_NE | CR0_TS | CR0_EM | CR0_MP), %eax
    andl $~(CR0_TS | CR0_EM), %eax
    # 注释了最后一句
    # movl %eax, %cr0
    ```

    > 为什么要关闭页机制？只将偏移地址设置为0不够么？这是个值得探讨的问题。
    >
    > 注意到`kern/init.entry.S`中有这样一段代码
    >
    > ```x86asm
    > next:
    >   # unmap va 0 ~ 4M, it's temporary mapping
    >   xorl %eax, %eax
    >   movl %eax, __boot_pgdir
    > ```
    >
    > 当CPU完成了`eip`的地址更新后，这两条指令会删除页目录表中的一个**临时**映射（va 0 ~ 4M to pa 0 ~ 4M）
    >
    > 但一旦删除了这个临时映射后，CPU无法正常寻址，即便页目录表中还有一个映射（va KERNBASE + (0 ~ 4M) to pa 0 ~ 4M， 注意KERNBASE为0）
    >
    > 但只要基地址不为0，则不会出错。
    >
    > 具体的问题在哪呢？或许，需要查询一下intel 80386的相关手册。

### 5) 扩展练习

#### Challenge1

> **buddy system（伙伴系统）分配算法**
>
> Buddy System算法把系统中的可用存储空间划分为存储块(Block)来进行管理, 每个存储块的大小必须是2的n次幂(Pow(2, n)), 即1, 2, 4, 8, 16, 32, 64, 128...

##### a. 前置准备

伙伴系统中每个存储块的大小都必须是2的n次幂，所以其中必须有个**可以将传入数转换为最接近该数的2的n次幂**的函数，相关代码如下

```cpp
// 传入一个数，返回最接近该数的2的指数（包括该数为2的整数这种情况）
size_t getLessNearOfPower2(size_t x)
{
    size_t _i;
    for(_i = 0; _i < sizeof(size_t) * 8 - 1; _i++)
        if((1 << (_i+1)) > x)
            break;
    return (size_t)(1 << _i);
}
```

##### b. 初始化

初始时，程序会多次将一块尺寸很大的物理内存空间传入`init_memmap`函数，但该物理内存空间的大小却不一定是2的n次幂，所以需要对其进行分割。设定分割后的内存布局如下

```cpp
/*
buddy system中的内存布局
      某块较大的物理空间
低地址                              高地址
+-+--+----+--------+-------------------+
| |  |    |        |                   |
+-+--+----+--------+-------------------+
低地址的内存块较小             高地址的内存块较大

*/
```

同时，在双向链表`free_area.free_list`中，令空间较小的内存块在双向链表中靠前，空间较大的内存块在双向链表中靠后；低地址在前，高地址在后。故以下是最终的链表布局：

```cpp
/*
free_area.free_list中的内存块顺序:

1. 一大块连续物理内存被切割后，free_area.free_list中的内存块顺序
    addr: 0x34       0x38           0x40
        +----+     +--------+     +---------------+
    <-> | 0x4| <-> | 0x8    | <-> |     0x10      | <->
        +----+     +--------+     +---------------+

2. 几大块物理内存（这几块之间可能不连续）被切割后，free_area.free_list中的内存块顺序
    addr: 0x34       0x104       0x38           0x108          0x40                 0x110
        +----+     +----+     +--------+     +--------+     +---------------+     +---------------+
    <-> | 0x4| <-> | 0x4| <-> | 0x8    | <-> | 0x8    | <-> |     0x10      | <-> |     0x10      | <->
        +----+     +----+     +--------+     +--------+     +---------------+     +---------------+
*/
```

根据上面的内存规划，可以得到`buddy_init_memmap`的代码

```cpp
static void
buddy_init_memmap(struct Page *base, size_t n) {
    assert(n > 0);

    // 设置当前页向后的curr_n个页
    struct Page *p = base;
    for (; p != base + n; p ++) {
        assert(PageReserved(p));
        p->flags = p->property = 0;
        set_page_ref(p, 0);
    }
    // 设置总共的空闲内存页面
    nr_free += n;
    // 设置base指向尚未处理内存的end地址
    base += n;
    while(n != 0)
    {
        size_t curr_n = getLessNearOfPower2(n);
        // 向前挪一块
        base -= curr_n;
        // 设置free pages的数量
        base->property = curr_n;
        // 设置当前页为可用
        SetPageProperty(base);
        // 按照块的大小来插入空闲块，从小到大排序
        // @note 这里必须使用搜索的方式来插入块而不是直接list_add_after(&free_list)，因为存在大的内存块不相邻的情况
        list_entry_t* le;
        for(le = list_next(&free_list); le != &free_list; le = list_next(le))
        {
            struct Page *p = le2page(le, page_link);
            // 排序方式以内存块大小优先，地址其次。
            if((p->property > base->property)
                 || (p->property ==  base->property && p > base))
                break;
        }
        list_add_before(le, &(base->page_link));
        n -= curr_n;
    }
}
```

##### c. 空间分配

分配空间时，遍历双向链表，查找大小合适的内存块。

- 若链表中不存在合适大小的内存块，则**对半切割**遍历过程中遇到的第一块大小大于所需空间的内存块。
- 如果切割后的两块内存块的大小还是太大，则继续切割**第一块**内存块。
- 循环该操作，直至切割出合适大小的内存块。
- 最终`buddy_alloc_pages`代码如下

  ```cpp
  static struct Page *
  buddy_alloc_pages(size_t n) {
      assert(n > 0);
      // 向上取2的幂次方，如果当前数为2的幂次方则不变
      size_t lessOfPower2 = getLessNearOfPower2(n);
      if (lessOfPower2 < n)
          n = 2 * lessOfPower2;
      // 如果待分配的空闲页面数量小于所需的内存数量
      if (n > nr_free) {
          return NULL;
      }
      // 查找符合要求的连续页
      struct Page *page = NULL;
      list_entry_t *le = &free_list;
      while ((le = list_next(le)) != &free_list) {
          struct Page *p = le2page(le, page_link);
          if (p->property >= n) {
              page = p;
              break;
          }
      }
      // 如果需要切割内存块时，一定分配切割后的前面那块
      if (page != NULL) {
          // 如果内存块过大，则持续切割内存
          while(page->property > n)
          {
              page->property /= 2;
              // 切割出的右边那一半内存块不用于内存分配
              struct Page *p = page + page->property;
              p->property = page->property;
              SetPageProperty(p);
              list_add_after(&(page->page_link), &(p->page_link));
          }
          nr_free -= n;
          ClearPageProperty(page);
          assert(page->property == n);
          list_del(&(page->page_link));
      }
      return page;
  }
  ```

##### d. 内存释放

释放内存时

- 先将该内存块按照**内存块大小从小到大与内存块地址从小到大**的顺序插入至双向链表（具体请看上面的链表布局）。

- 尝试向前合并，一次就够。如果向前合并成功，则一定不能再次向前合并。

- 之后循环向后合并，直至无法合并。

  > 需要注意的是，在查找两块内存块能否合并时，若当前内存块合并过，则其大小会变为原来的2倍，此时需要遍历比原始大小（合并前内存块大小）更大的内存块。

- 判断当前内存块的位置是否正常，如果不正常，则需要断开链表并重新插入至新的位置。

  > 如果当前内存块没有合并则肯定正常，如果合并过则**不一定异常**。

- 最终代码如下

  ```cpp
  static void
  buddy_free_pages(struct Page *base, size_t n) {
      assert(n > 0);
      // 向上取2的幂次方，如果当前数为2的幂次方则不变
      size_t lessOfPower2 = getLessNearOfPower2(n);
      if (lessOfPower2 < n)
          n = 2 * lessOfPower2;
      struct Page *p = base;
      for (; p != base + n; p ++) {
          assert(!PageReserved(p) && !PageProperty(p));
          p->flags = 0;
          set_page_ref(p, 0);
      }
      base->property = n;
      SetPageProperty(base);
      nr_free += n;
      list_entry_t *le;
      // 先插入至链表中
      for(le = list_next(&free_list); le != &free_list; le = list_next(le))
      {
          p = le2page(le, page_link);
          if ((base->property <= p->property)
                   || (p->property ==  base->property && p > base))) {
              break;
          }
      }
      list_add_before(le, &(base->page_link));
      // 先向左合并
      if(base->property == p->property && p + p->property == base) {
          p->property += base->property;
          ClearPageProperty(base);
          list_del(&(base->page_link));
          base = p;
          le = &(base->page_link);
      }
  
      // 之后循环向后合并
      // 此时的le指向插入块的下一个块
      while (le != &free_list) {
          p = le2page(le, page_link);
          // 如果可以合并(大小相等+地址相邻),则合并
          // 如果两个块的大小相同，则它们不一定内存相邻。
          // 也就是说，在一条链上，可能存在多个大小相等但却无法合并的块
          if (base->property == p->property && base + base->property == p)
          {
              // 向右合并
              base->property += p->property;
              ClearPageProperty(p);
              list_del(&(p->page_link));
              le = &(base->page_link);
          }
          // 如果遍历到的内存块一定无法合并，则退出
          else if(base->property < p->property)
          {
              // 如果合并不了，则需要修改base在链表中的位置，使大小相同的聚在一起
              list_entry_t* targetLe = list_next(&base->page_link);
              p = le2page(targetLe, page_link);
              while(p->property < base->property)
                   || (p->property ==  base->property && p > base))
                  targetLe = list_next(targetLe);
              // 如果当前内存块的位置不正确，则重置位置
              if(targetLe != list_next(&base->page_link))
              {
                  list_del(&(base->page_link));
                  list_add_before(targetLe, &(base->page_link));
              }
              // 最后退出
              break;
          }
          le = list_next(le);
      }
  }
  ```

##### 5. 算法检查

`buddy_check`是一个不能忽视的检查函数，该函数可以帮助查找出程序内部隐藏的bug。笔者将其中原本用于检查`FIFO`算法的内容修改成检查`buddySystem`的内容。所修改的内容如下：

```cpp
//.........................................................
// 先释放
free_pages(p0, 26);     // 32+  (-:已分配 +: 已释放)
// 首先检查是否对齐2
p0 = alloc_pages(6);    // 8- 8+ 16+
p1 = alloc_pages(10);   // 8- 8+ 16-
assert((p0 + 8)->property == 8);
free_pages(p1, 10);     // 8- 8+ 16+
assert((p0 + 8)->property == 8);
assert(p1->property == 16);
p1 = alloc_pages(16);   // 8- 8+ 16-
// 之后检查合并
free_pages(p0, 6);      // 16+ 16-
assert(p0->property == 16);
free_pages(p1, 16);     // 32+
assert(p0->property == 32);

p0 = alloc_pages(8);    // 8- 8+ 16+
p1 = alloc_pages(9);    // 8- 8+ 16-
free_pages(p1, 9);     // 8- 8+ 16+
assert(p1->property == 16);
assert((p0 + 8)->property == 8);
free_pages(p0, 8);      // 32+
assert(p0->property == 32);
// 检测链表顺序是否按照块的大小排序的
p0 = alloc_pages(5);
p1 = alloc_pages(16);
free_pages(p1, 16);
assert(list_next(&(free_list)) == &((p1 - 8)->page_link));
free_pages(p0, 5);
assert(list_next(&(free_list)) == &(p0->page_link));

p0 = alloc_pages(5);
p1 = alloc_pages(16);
free_pages(p0, 5);
assert(list_next(&(free_list)) == &(p0->page_link));
free_pages(p1, 16);
assert(list_next(&(free_list)) == &(p0->page_link));

// 还原
p0 = alloc_pages(26);
//.........................................................
```

##### 6. 总结与完整代码

- `buddySystem`在**所分配的内存大小均为2的n次幂**这种环境下，使用效果极佳。

- 由于`buddySystem`的特性，最好使用二叉树而非普通双向链表来管理内存块，这样就可以避免一系列的bug。

  即便普通双向链表可以很好的实现`buddySystem`，但其中仍然存在一个较为麻烦的问题：

  >  **当某个物理块释放，将其插入至双向链表后，如果该物理块既可以和上一个物理块合并，又可以和下一个物理块合并，那么此时该合并哪一个物理块？**

  这个问题，双向链表无法很好的解决，该问题很可能会使一些物理块因为错误的合并顺序而最终导致内存的碎片化，降低内存的使用率。

- 完整代码位于[github](https://github.com/Kiprey/Skr_Learning/blob/master/week9-19/uCore/os_kernel_lab-master_RAW/labcodes/lab2/kern/mm/buddySystem_pmm.c)，如有需要请自取。

#### Challenge2

> **任意大小的内存单元slub分配算法**
>
> slub算法，实现两层架构的高效内存单元分配，第一层是基于页大小的内存分配，第二层是在第一层基础上实现基于任意大小的内存分配。可简化实现，能够体现其主体思想即可。

> Challenge2 先鸽了，赶进度QWQ
