# uCore实验 - Lab3
<!-- TOC -->

- [uCore实验 - Lab3](#ucore实验---lab3)
  - [简介](#简介)
  - [知识点](#知识点)
    - [1. 虚拟内存](#1-虚拟内存)
    - [2. 页面置换算法小叙](#2-页面置换算法小叙)
      - [I. 局部页面置换算法](#i-局部页面置换算法)
        - [1) 最近最少用算法（LRU）](#1-最近最少用算法lru)
          - [a. 简介](#a-简介)
          - [b. 具体实现](#b-具体实现)
        - [2) 改进的时钟页面置换算法（Clock）](#2-改进的时钟页面置换算法clock)
          - [a. 简介](#a-简介-1)
          - [b. 具体实现](#b-具体实现-1)
      - [II. 全局置换算法](#ii-全局置换算法)
        - [1) 工作集置换算法](#1-工作集置换算法)
          - [a. 工作集与常驻集](#a-工作集与常驻集)
          - [b. 思路](#b-思路)
          - [c. 具体实现](#c-具体实现)
        - [2) 缺页率置换算法（PPF）](#2-缺页率置换算法ppf)
          - [a. 简介](#a-简介-2)
          - [b. 具体实现](#b-具体实现-2)
      - [III. Belady现象](#iii-belady现象)
    - [3. uCore虚拟内存机制的实现](#3-ucore虚拟内存机制的实现)
      - [I. 虚拟内存管理](#i-虚拟内存管理)
      - [II. 页面置换](#ii-页面置换)
  - [练习解答](#练习解答)
    - [1. 练习0](#1-练习0)
    - [2. 练习1](#2-练习1)
    - [3. 练习2](#3-练习2)
    - [4. 扩展练习](#4-扩展练习)
      - [Challenge 1](#challenge-1)
      - [Challenge 2](#challenge-2)

<!-- /TOC -->
## 简介

- 这里是笔者在完成`uCore` Lab 3时写下的一些笔记
- 内容涉及虚拟内存存储等

## 知识点

### 1. 虚拟内存

- 虚拟内存是CPU可以看到的“内存”。

  - 虚拟内存所对应的实际物理内存单元可能不存在。
  - 虚拟内存的地址和对应物理内存的地址可能不一致。
  - 通过操作系统所实现的某种内存映射机制，可以达到访问的虚拟内存地址转换为物理内存地址的目的。

- 当程序访问内存遇上特殊情况时，CPU会执行第十四号中断处理程序——缺页处理程序来处理。

  - 特殊情况有以下两种

    - **写入**一个**存在物理页**的虚拟页——**写时复制**。
    - 读写一个不存在物理页的虚拟页——**缺页**。
    - 不满足访问权限。

  - 当程序触发缺页中断时，CPU会把产生异常的线性地址存储在**CR2**寄存器中，并且把**页访问异常错误码**保存在中断栈中。

    > 其中，页访问异常错误码的位0为１表示对应物理页不存在；位１为１表示写异常；位２为１表示访问权限异常。

- 由于虚拟内存空间比物理内存空间大得多，所以必须在合适的情况下，将不常用的页面调至外存，或者将待用的页面从外存调入内存中。 **这个过程对应用程序无感。** 而什么时候调进调出，选择哪个页面调出，这都是值得考究的，这就是使用页面置换算法的目的。

### 2. 页面置换算法小叙

> 当物理页面不够用时，需要将某个页面置换到外存中。
>
> 那么该置换哪个物理页面呢？这就是页面置换算法的用处。

#### I. 局部页面置换算法

> 置换页面的选择范围仅限于当前进程占用的物理页面内.

##### 1) 最近最少用算法（LRU）

###### a. 简介

- 思路：选择**最长时间没有被引用**的页面进行置换。
- 实现：缺页时，计算内存中每个逻辑页面的上一个访问时间，并选择**上一个使用到当前时间最长**的页面。
- 特征：最优置换算法的一种近似。

###### b. 具体实现

- 页面链表
  - 系统维护一个按最近一次访问时间排序的页面链表
    - 链表首节点是最近刚刚使用过的页面
    - 链表尾结点是最久未使用的页面
  - 访问内存时，找到相应页面并将其移至链表之首
  - 缺页时，置换链表尾结点的页面
- 活动页面栈
  - 访问页面时，将此页号压入栈底。并将栈内相同的页号抽出
  - 缺页时，置换栈底的页面。

> 上述的两种实现都需要维护以及遍历搜索某个数据结构，
>
> 同时LRU对于过去的访问情况统计**过于细致**，所以该方法较为复杂。

##### 2) 改进的时钟页面置换算法（Clock）

###### a. 简介

- 思路：
  - 仅对页面的访问情况进行大致统计
  - 减小修改页的缺页处理开销
- 数据结构：
  - 在页表项中增加访问位，描述页面在过去一段时间的内访问情况。
  - 在页表项中增加修改位，以判断当前页面是否修改过但没有存入外存。
  - 各页面组织成环形链表，同时指针指向最先调入的页面。
- 算法
  - 访问页面时，在页表项记录页面访问情况
  - 缺页时，从指针处开始顺序查找**未被访问与未被修改**的页面进行置换。
- 特征： 时钟算法是LRU与FIFO的折中。

###### b. 具体实现

- 页面装入内存时，访问位初始化为0
- 访问页面（读/写）时，访问位置为1
- 缺页时，从指针当前位置顺序检查环形链表。
  - 若当前遍历到的页面访问位为0，则**置换该页**
  - 若当前遍历到的页面访问位为1，则**设置该页的访问位为0**，并移动指针到下一个页面，直到找到可置换的页面。

#### II. 全局置换算法

- 思路：全局置换算法为进程分配可变数目的物理页面。
- 要解决的问题：
  - 进程在不同阶段的内存需求是有变化的。
  - 分配给进程的内存也需要在不同阶段有所变化。
  - 全局置换算法需要确定分配给进程的物理页面数。
- CPU利用率与并发进程数存在相互制约的关系。
  - 进程数少时，提高并发进程数，可提高CPU利用效率。
  - 并发进程导致内存访问增加
  - 并发进程的内存访问会降低了访存的局部性特征。
  - 局部性特征的下降会导致缺页率上升和CPU利用率下降。

##### 1) 工作集置换算法

###### a. 工作集与常驻集

- **工作集**是一个进程当前正在使用的**逻辑页面**集合，可表示为二元函数$W(t, \Delta)$
  - $t$是当前的执行时刻
  - $\Delta$ 称为**工作集窗口**(working-set window)，即一个定长的页面访问时间的窗口。
  - $W(t, \Delta)$指在当前时刻$t$前的$\Delta$时间窗口中的**所有访问页面**所组成的集合。
  - $|W(t, \Delta)|$指工作集的大小，即页面数目。
- **常驻集**是当前时刻进程**实际驻留**在内存中的页面集合。
- 工作集与常驻集的关系
  - 工作集是进程在运行过程中固有的性质
  - 常驻集取决于系统分配给进程的物理页面数目和页面置换算法。

###### b. 思路

- 当前时刻前$\tau$个内存访问的页引用是工作集。其中$\tau$被称为**窗口大小**。

- 换出不在工作集中的页面

###### c. 具体实现

- 访存链表：维护窗口内的访存页面
- **访存时，换出不在工作集的页面；** 更新访存链表。
- 缺页时，换入页面，更新访存链表。

##### 2) 缺页率置换算法（PPF）

###### a. 简介

通过调节常驻集大小，使每个进程的缺页率保持在一个合理的范围内。

- 若进程缺页率过高，则增加常驻集以分配更多的物理内存
- 若进程缺页率过低，则减小常驻集以减小它的物理页面数。

###### b. 具体实现

- 访存时，设置引用位标志
- 缺页时，计算从上次缺页时间$t_{last}$到现在$t_{current}$的时间间隔
  - 如果$t_{current}-t_{last}>T$，则置换所有在$[t_{last}, t_{current}]$时间内**没有被引用**的页。
  - 如果$t_{current}-t_{last}<T$，则增加缺失页到常驻集中。

#### III. Belady现象

- 现象： 采用FIFO等算法时，可能出现分配的物理页面数增加，缺页次数反而升高的异常情况。
- 原因：
  - FIFO算法的置换特征与进程访问内存的动态特征矛盾
  - 被置换出去的页面并不一定是进程近期不会访问的。

### 3. uCore虚拟内存机制的实现

#### I. 虚拟内存管理

- 结构体变量`check_mm_struct`用于管理虚拟内存页面，其结构体如下

  ```cpp
  // the control struct for a set of vma using the same PDT
  struct mm_struct {
      list_entry_t mmap_list;        // 按照虚拟地址顺序双向连接的虚拟页链表
      struct vma_struct *mmap_cache; // 当前使用的虚拟页地址，该成员加速页索引速度。
      pde_t *pgdir;                  // 虚拟页对应的PDT
      int map_count;                 // 虚拟页个数
      void *sm_priv;                 // 用于指向swap manager的某个链表,在FIFO算法中，该双向链表用于将可交换的已分配物理页串起来
  };
  ```

- 当分配出新的虚拟页时，程序会执行`insert_vma_struct`函数，此时虚拟页`vma_struct`就会被插入`mm_struct::mmap_list`双向链表中。
- 若程序首次访问该内存而触发缺页中断时，程序会在缺页处理程序中为该虚拟页划分出一块新的物理页。同时，还会更新`mm_struct::pgdir`上的对应页表条目，之后该页的内存访问即可正常执行。
- 在FIFO页面置换算法中，初始时，`mm_struct`中的`sm_priv`会被设置为`pra_list_head`。而`pra_list_head`是一个双向链表的起始结点，该双向链表用于将**可交换的已分配物理页**串起来。

#### II. 页面置换

- `swap_manager`与`pmm_manager`类似，都设置了一个用于管理某个功能的模块。

  ```cpp
  struct swap_manager
  {
       const char *name;
       /* Global initialization for the swap manager */
       int (*init)            (void);
       /* Initialize the priv data inside mm_struct */
       int (*init_mm)         (struct mm_struct *mm);
       /* Called when tick interrupt occured */
       int (*tick_event)      (struct mm_struct *mm);
       /* Called when map a swappable page into the mm_struct */
       int (*map_swappable)   (struct mm_struct *mm, uintptr_t addr, struct Page *page, int swap_in);
       /* When a page is marked as shared, this routine is called to
        * delete the addr entry from the swap manager */
       int (*set_unswappable) (struct mm_struct *mm, uintptr_t addr);
       /* Try to swap out a page, return then victim */
       int (*swap_out_victim) (struct mm_struct *mm, struct Page **ptr_page, int in_tick);
       /* check the page relpacement algorithm */
       int (*check_swap)(void);
  };
  ```

- 若使用FIFO页面置换算法，则在缺页中断程序中，程序只会**换入**目标物理页，而不会主动换出。

  只有在分配空闲物理页时，若`pmm_manager->alloc_pages(n)`失败，则程序才会执行一次页面换出，以腾出空闲的物理页，并重新分配。

- `swap_in`函数只会将目标物理页加载进内存中，而不会修改页表条目。所以相关的标志位设置必须在`swap_in`函数的外部手动处理。而`swap_out`函数会先执行`swap_out_victim`，找出最适合换出的物理页，并将其换出，最后刷新TLB。需要注意的是`swap_out`函数会在函数内部设置PTE，当某个页面被换出后，PTE会被设置为所换出物理页在硬盘上的偏移。

  ```cpp
  cprintf("swap_out: i %d, store page in vaddr 0x%x to disk swap entry %d\n", i, v, page->pra_vaddr/PGSIZE+1);
  *ptep = (page->pra_vaddr/PGSIZE+1)<<8;
  free_page(page);
  ```

  当PTE所对应的物理页存在于内存中，那么该PTE就是正常的页表条目，可被CPU直接寻址用于转换地址。但当所对应的物理页不在内存时，该PTE就成为`swap_entry_t`，保存该物理页数据在外存的偏移位置。相关代码如下：

  ```cpp
  /*
   * swap_entry_t
   * --------------------------------------------
   * |         offset        |   reserved   | 0 |
   * --------------------------------------------
   *           24 bits            7 bits    1 bit
   * /
   /* *
   * swap_offset - takes a swap_entry (saved in pte), and returns
   * the corresponding offset in swap mem_map.
   * */
  #define swap_offset(entry) ({                                       \
                 size_t __offset = (entry >> 8);                        \
                 if (!(__offset > 0 && __offset < max_swap_offset)) {    \
                      panic("invalid swap_entry_t = %08x.\n", entry);    \
                 }                                                    \
                 __offset;                                            \
            })
  ```

- 同时，不是所有物理页面都可以置换，例如内核关键代码和数据等等，所以在分配物理页时，需要对于那些可被置换的物理页执行`swap_map_swappable`函数，将该物理页加入到`mm_struct::sm_priv`指针所指向的双向链表中，换入和换出操作都会操作该链表（插入/移除**可交换的已分配**物理页）。

- 数据结构`Page`和`vma_struct`分别用于管理物理页和虚拟页，其结构如下：

  ```cpp
  // 用于描述某个虚拟页的结构
  struct vma_struct {
      struct mm_struct *vm_mm; // 管理该虚拟页的mm_struct
      uintptr_t vm_start;      // 虚拟页起始地址，包括当前地址  
      uintptr_t vm_end;        // 虚拟页终止地址，不包括当前地址（地址前闭后开）  
      uint32_t vm_flags;       // 相关标志位
      list_entry_t list_link;  // 用于连接各个虚拟页的双向指针
  };
  
  // 数据结构Page相关成员的用途已在uCore-2中介绍过，这里只提它新增的两个成员pra_*
  struct Page {
      int ref;
      uint32_t flags;
      unsigned int property;
      list_entry_t page_link;
      list_entry_t pra_page_link;     // 用于连接上一个和下一个*可交换已分配*的物理页
      uintptr_t pra_vaddr;            // 用于保存该物理页所对应的虚拟地址。
  };
  ```

  - 当分配某个虚拟页`vma_struct`时，程序会在`insert_vma_struct`函数中设置其`vm_mm`成员为某个`mm_struct`，这样便于后续的管理。
  - 在函数`pgdir_alloc_page`中，程序会设置`Page`的`pra_vaddr`成员，将其设置为当前物理页所对应的虚拟地址，之后便可通过`Page->pra_vaddr->pte`一条链，直接找到当前**物理页**地址所对应的PTE条目。同时，也可通过`pra_vaddr`来确定对应外存的相对偏移`page->pra_vaddr/PGSIZE+1`。
  - `Page::page_link`用于将空闲物理页连接至双向链表中，而`page::pra_page_link`用于将**可交换的已分配**物理页连接至另一个双向链表中，注意两者的用途是不同的。

## 练习解答

### 1. 练习0

> **填写已有实验**

- 抄就对了。

### 2. 练习1

> **给未被映射的地址映射上物理页**
>
> 完成do_pgfault（mm/vmm.c）函数，给未被映射的地址映射上物理页。设置访问权限 的时候需要参考页面所在 VMA 的权限，同时需要注意映射物理页时需要操作内存控制 结构所指定的页表，而不是内核的页表。

实验代码如下

```cpp
int do_pgfault(struct mm_struct *mm, uint32_t error_code, uintptr_t addr) {
    int ret = -E_INVAL;
    // 获取触发pgfault的虚拟地址所在虚拟页
    struct vma_struct *vma = find_vma(mm, addr);

    pgfault_num++;
    // 如果当前访问的虚拟地址不在已经分配的虚拟页中
    if (vma == NULL || vma->vm_start > addr) {
        cprintf("not valid addr %x, and  can not find it in vma\n", addr);
        goto failed;
    }
    // 检测错误代码。这里的检测不涉及特权判断。
    switch (error_code & 3) {
    default:
        // 写，同时存在物理页，则写时复制
        // 需要注意的是，default会执行case2的代码，也就是判断是否有写权限。
    case 2:
        // 读，同时不存在物理页
        // 同时如果当前操作是写入，但所在虚拟页不允许写入
        if (!(vma->vm_flags & VM_WRITE)) {
            cprintf("do_pgfault failed: error code flag = write AND not present, but the addr's vma cannot write\n");
            goto failed;
        }
        break;
    case 1: /* error code flag : (W/R=0, P=1): read, present */
        // 读，同时存在物理页。那就不可能会调用page fault，肯定哪里有问题，直接failed
        cprintf("do_pgfault failed: error code flag = read AND present\n");
        goto failed;
    case 0: /* error code flag : (W/R=0, P=0): read, not present */
        // 写，同时不存在物理页面
        // 如果当前操作是读取，但所在虚拟页不允许读取或执行
        if (!(vma->vm_flags & (VM_READ | VM_EXEC))) {
            cprintf("do_pgfault failed: error code flag = read AND not present, but the addr's vma cannot read or exec\n");
            goto failed;
        }
    }
    // 设置页表条目所对应的权限
    uint32_t perm = PTE_U;
    if (vma->vm_flags & VM_WRITE) {
        perm |= PTE_W;
    }
    addr = ROUNDDOWN(addr, PGSIZE);
    ret = -E_NO_MEM;
    pte_t *ptep=NULL;

    /* LAB3 EXERCISE 1: YOUR CODE */
    // 查找当前虚拟地址所对应的页表项
    if ((ptep = get_pte(mm->pgdir, addr, 1)) == NULL) {
        cprintf("get_pte in do_pgfault failed\n");
        goto failed;
    }
    // 如果这个页表项所对应的物理页不存在，则
    if (*ptep == 0) {
        // 分配一块物理页，并设置页表项
        if (pgdir_alloc_page(mm->pgdir, addr, perm) == NULL) {
            cprintf("pgdir_alloc_page in do_pgfault failed\n");
            goto failed;
        }
    }
    else {
        /* LAB3 EXERCISE 2: YOUR CODE */
        // 如果这个页表项所对应的物理页存在，但不在内存中
        // 如果swap已经初始化完成
        if(swap_init_ok) {
            struct Page *page=NULL;
            // 将目标数据加载到某块新的物理页中。
            // 该物理页可能是尚未分配的物理页，也可能是从别的已分配物理页中取的
            if ((ret = swap_in(mm, addr, &page)) != 0) {
                cprintf("swap_in in do_pgfault failed\n");
                goto failed;
            }
            // 将该物理页与对应的虚拟地址关联，同时设置页表。
            page_insert(mm->pgdir, page, addr, perm);
            // 当前缺失的页已经加载回内存中，所以设置当前页为可swap。
            swap_map_swappable(mm, addr, page, 1);
            page->pra_vaddr = addr;
        }
        else {
            cprintf("no swap_init_ok but ptep is %x, failed\n",*ptep);
            goto failed;
        }
   }
   ret = 0;
failed:
    return ret;
}
```

- 请描述页目录项（Page Directory Entry）和页表项（Page Table Entry）中组成部分对ucore实现页替换算法的潜在用处。
  - PTE结构与其标志位用途，请移步[uCore Lab2 - 虚拟页表结构](/2020/08/uCore-2#d-虚拟页表结构)
- 如果ucore的缺页服务例程在执行过程中访问内存，出现了页访问异常，请问硬件要做哪些事情？
  - 将发生错误的线性地址（虚拟地址）保存至CR2寄存器中。
  - 压入`EFLAGS`，`CS`, `EIP`，错误码和中断号至当前内核栈中。
  - 保存上下文。
  - 执行新的缺页中断程序。
  - 恢复上下文。
  - 继续执行上一级的缺页服务例程。

### 3. 练习2

> **补充完成基于FIFO的页面替换算法**
>
> 完成vmm.c中的do_pgfault函数，并且在实现FIFO算法的swap_fifo.c中完成map_swappable和swap_out_victim函数。

- `FIFO`中，当新加入一个物理页时，我们只需将该物理页加入至链表首部即可。当需要换出某个物理页时，选择链表末尾的物理页即可。

- 相关实现如下

  ```cpp
  static int
  _fifo_map_swappable(struct mm_struct *mm, uintptr_t addr, struct Page   *page, int swap_in)
  {
      list_entry_t *head=(list_entry_t*) mm->sm_priv;
      list_entry_t *entry=&(page->pra_page_link);

      assert(entry != NULL && head != NULL);
      //record the page access situlation
      /*LAB3 EXERCISE 2: YOUR CODE*/
      //(1)link the most recent arrival page at the back of the pra_list_head qeueue.
      list_add(head, entry);
      return 0;
  }

  static int
  _fifo_swap_out_victim(struct mm_struct *mm, struct Page ** ptr_page, int in_tick)
  {
       list_entry_t *head=(list_entry_t*) mm->sm_priv;
           assert(head != NULL);
       assert(in_tick==0);
       /* Select the victim */
       /*LAB3 EXERCISE 2: YOUR CODE*/
       //(1)  unlink the  earliest arrival page in front of pra_list_head qeueue
       //(2)  assign the value of *ptr_page to the addr of this page
       list_entry_t *le = head->prev;
       assert(head!=le);
       struct Page *p = le2page(le, pra_page_link);
       list_del(le);
       assert(p !=NULL);
       *ptr_page = p;

       return 0;
  }
  ```

- 如果要在ucore上实现"extended clock页替换算法"请给你的设计方案，现有的swap_manager框架是否足以支持在ucore中实现此算法？如果是，请给你的设计方案。如果不是，请给出你的新的扩展和基此扩展的设计方案。并需要回答如下问题
  - 现有的swap_manager框架可以支持在ucore中实现此算法，具体见**扩展练习1**。
  - 需要被换出的页的特征是什么？
    - `PTE_P`(Present)和`PTE_D`(Dirty)位均为0。
  - 在ucore中如何判断具有这样特征的页？
    - 获取线性地址所对应的页表项，之后使用位运算判断`PTE_P`和`PTE_D`。
  - 何时进行换入和换出操作？
    - 缺页时换入。
    - 物理页帧满时换出，不过需要注意dirtybit的处理。可以在修改dirty的时候写入外存，或者可以在最终要删除该物理页时再写入外存。后者有利于多个写操作的合并，降低缺页代价，但此时的页替换算法却退化成普通的clock算法，而不是extended clock算法了。

### 4. 扩展练习

#### Challenge 1

> **实现识别dirty bit的 extended clock页替换算法**

- 在`FIFO`的基础上，实现`swap_out_victim`函数即可。

- 该函数中查找一块可用于换出的物理页，最多只需要遍历三次：
  - 第一次查找 !PTE_A & !PTE_D，同时重置当前页的PTE_A，为第二次遍历的条件打基础
  - 第二次查找 !PTE_A & !PTE_D， 同时重置当前页的PTE_D，为第三次遍历的条件打基础
  - 第三次查找，肯定能找到

- 这里需要注意对于`PTE_D`的操作，若第一次、第二次遍历都找不到符合要求的物理页，则必须对`PTE_D`下手，重置该标志位。还有一点需要注意，在每次修改PTE标志位后，都需要重置TLB缓存。

- `swap_out_victim`相关代码如下（偷了个小懒，每次遍历链表都是从头开始，同时其余函数沿用`FIFO`）：

  ```cpp
  static int
  _extend_clock_swap_out_victim(struct mm_struct *mm, struct Page ** ptr_page, int in_tick)
  {
      list_entry_t *head=(list_entry_t*) mm->sm_priv;
          assert(head != NULL);
      assert(in_tick==0);

      // 第一次查找 !PTE_A & !PTE_D，同时重置当前页的PTE_A
      // 第二次查找 !PTE_A & !PTE_D， 同时重置当前页的PTE_D
      // 第三次查找，肯定能找到
      for(int i = 0; i < 3; i++)
      {
          list_entry_t *le = head->prev;
          assert(head!=le);
          while(le != head)
          {
              struct Page *p = le2page(le, pra_page_link);
              pte_t* ptep = get_pte(mm->pgdir, p->pra_vaddr, 0);
              // 如果满足未使用未修改这两个条件，则直接分配
              if(!(*ptep & PTE_A) && !(*ptep & PTE_D))
              {
                  list_del(le);
                  assert(p !=NULL);
                  *ptr_page = p;
                  return 0;
              }
              // 如果在第一次查找中，访问到了一个已经使用过的PTE，则标记为未使用。
              if(i == 0)
                  *ptep &= ~PTE_A;
              // 如果在第二次查找中，访问到了一个已修改过的PTE，则标记为未修改。
              else if(i == 1)
                  *ptep &= ~PTE_D;

              le = le->prev;
              // 遍历了一回，肯定修改了标志位，所以要刷新TLB
              tlb_invalidate(mm->pgdir, le);
          }
      }
      // 按照前面的assert与if，不可能会执行到此处，所以return -1
      return -1;
  }
  
  struct swap_manager swap_manager_fifo =
  {
       .name            = "extend_clock swap manager",
       .init            = &_fifo_init,
       .init_mm         = &_fifo_init_mm,
       .tick_event      = &_fifo_tick_event,
       .map_swappable   = &_fifo_map_swappable,
       .set_unswappable = &_fifo_set_unswappable,
       .swap_out_victim = &_extend_clock_swap_out_victim,
       .check_swap      = &_fifo_check_swap,
  };
  ```

#### Challenge 2

> **实现不考虑实现开销和效率的LRU页替换算法**

遇到了一个较为麻烦的问题：如何在正常访问内存时设置`swap_manager`中相关链表上物理页的LRU，留坑......
