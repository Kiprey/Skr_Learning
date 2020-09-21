# uCore实验 - Lab6

## 简介

- 这里是笔者在完成`uCore` Lab 6时写下的一些笔记
- 内容涉及处理机调度的一些相关实现。

## 知识点

### 1. CPU资源的时分复用

- 进程切换：CPU资源的当前占用者切换
  - 保存当前进程在PCB中的执行上下文（CPU状态）
  - 恢复下一个进程的执行上下文
- 处理机调度
  - 从就绪队列中**挑选**下一个占用CPU运行的进程。
  - 从多个可用CPU中**挑选**就绪进程可使用的CPU资源。
- 调度程序：挑选就绪进程的内核函数
  - 调度策略：依据什么原理挑选进程/线程
  - 调度时机：什么时候进行调度
    - 内核运行调度程序的条件
      - 进程从运行状态切换到等待状态
      - 进程被终结了
    - 非抢占系统：当前进程主动放弃CPU时
    - 可抢占系统
      - 中断请求被服务例程响应完成时
      - 当前进程被抢占
        - 进程的时间片耗尽
        - 进程从等待状态切换到就绪状态

### 2. 调度准则

- 比较调度算法的准则
  - CPU使用率：CPU处于忙状态的**时间百分比**
  - 吞吐量：单位时间内完成的**进程数量**
  - 周转时间：进程从初始化到结束（包括等待）的**总时间**
  - 等待时间：进程在就绪队列中的**总时间**
  - 响应时间：从提交请求到产生响应所花费的**总时间**
- 调度策略的目标
  - **减少响应时间**：及时处理用户的输入，尽快将输出反馈给用户
  - **减少平均响应时间的波动**：在交互系统中，可预测性比高差异低平均更重要。

  > 低延迟调度改善用户的交互体验。
  >
  > 响应时间是操作系统的计算延迟。

- 调度策略的吞吐量目标
  - **增加吞吐量**
    - 减小开销（例如上下文切换的开销）
    - 系统资源的高效利用（例如CPU和IO设备的并行使用）
  - **减少每个进程的等待时间**
  - 保证**吞吐量不受用户交互的影响**

  > 吞吐量是操作系统的计算带宽。

- 调度的公平性目标
  - 保证每个进程**占用相同的CPU时间**
  - 保证每个进程的**等待时间**相同
  - 公平通常会增加**平均响应时间**

### 3. 调度算法

#### a. 先来先服务算法（First Come First Served, FCFS）

> 依据进程进入就绪状态的先后顺序排序

- 优点：简单
- 缺点：
  - 平均等待时间波动较大（短进程可能排在长进程后面）
  - IO资源和CPU资源的利用效率可能较低

     > CPU密集型进程会导致IO设备闲置时，IO密集型进程也在等待。（CPU和IO设备可并行执行）

#### b. 短进程优先算法（SPN）

> 选择就绪队列中执行时间**最短**进程占用的CPU进入运行状态。就绪队列按预期的执行时间来排序。

- 优点：短进程优先算法具有**最优**平均周转时间。
- 缺点：
  - 可能导致**饥饿**。例如连续的短进程流会使长进程无法获得CPU资源。
  - **需要预估下一个CPU计算的持续时间**

    > 一种方法是，用**历史**执行时间预估**未来**执行时间

> 短剩余时间优先算法（SRT）：SPN算法的可抢占改进

#### c. 最高响应比优先算法（HRRN）

> 选择就绪队列中响应比R值最高的进程
>
> 其中$R=(w+s)/s$, s：执行时间；w：等待时间

- 在短进程优先算法基础上的改进
- 不可抢占
- 关注进程的等待时间
- 防止无限期推迟

#### d. 时间片轮转算法（RR，Round-Robin）

- 时间片：分配处理机资源的基本时间单位
- 算法思路：
  - 时间片结束时，按FCFS算法切换到下一个就绪进程。
  - 每隔n-1个时间片，进程执行一个时间片。
- 时间片长度选择
  - 时间片长度过长，则**等待时间太长**，极端情况下退化成FCFS。
  - 时间片长度过短，则**反应较为迅速**，但产生大量进程上下文切换，影响系统吞吐量。
  - 需要选择一个合适的时间片长度，以维持上下文切换开销处于1%状态。

#### e. 多级队列调度算法（MQ）

- 就绪队列被划分为多个独立的子队列，每个队列拥有自己的调度策略
- 队列间的调度
  - 固定优先级。例如先处理前台，后处理后台。但可能会导致饥饿。
  
  - 时间片轮转。每个队列都得到一个确定的能够调度其进程的CPU总时间。
  
    > 例如80%CPU时间用于前台，20%CPU时间用于后台。

#### f. 多级反馈队列算法（MLFQ）

- 进程可在不同队列间移动的多级队列算法。

  > 时间片大小随优先级级别的增加而增加。
  >
  > 例如进程在当前时间片内没有完成，则降到下一个优先级。

- 特征：CPU密集型进程优先级下降的很快，IO密集型进程停留在高优先级。

#### g. 公平共享调度（FSS, Fair Share Scheduling）

FSS控制用户对系统资源的访问

- 一些用户组比其他用户组更重要。
- 保证不重要的组无法垄断资源
  - 未使用的资源按比例分配
  - 没有达到资源使用率目标的组获得更高的优先级。

### 4. 实时操作系统

- 实时操作系统的定义：正确性依赖于其时间和功能两方面的操作系统
- 实时操作系统的性能指标：
  - 时间约束的及时性（deadline）
  - 速度和平均性能相对不重要
- 实时操作系统的特性：时间约束的**可预测性**
- 实时任务：
  - 任务：一次计算/文件读取/信息传递等等。
  - 任务属性：完成任务所需的资源以及定时参数。
- 周期实时任务：一系列相似的任务
  - 任务有规律的重复
  - 周期p = 任务请求间隔$(0<p)$
  - 执行时间e = 最大执行时间$(0< e <p)$
  - 使用率$U = e/p$
- 软时限和硬时限
  - 硬时限（hard deadline）
    - 错过任务时限将会导致**灾难性或非常严重的后果**
    - **必须**验证，在最坏的情况下能够满足时限
  - 软时限（soft deadline）
    - **通常**能满足任务时限。如有时不能满足，则降低要求
    - 尽力保证满足任务时限。
- 实时调度
  - 速率单调调度算法（RM, Rate Monotonic）
    - 通过周期安排优先级
    - 周期越短优先级越高
    - 执行周期越短的任务。
  - 最早截止时间优先算法（EDF，Earliest Deadline First）
    - 截止时间越早优先级越高
    - 执行截止时间最早的任务

### 5. 多处理器调度

- 多处理器调度的特征
  - 多个处理机组成一个多处理系统
  - 处理机间可负载共享
- 对称多处理器（SMP，Symmetric multiprocessing）调度
  - 每个处理器运行自己的调度程序
  - 调度程序对共享资源的访问需要进行同步
- 对称多处理器的进程分配
  - 静态进程分配
    - 进程从开始到结束都被分配到一个固定的处理机上执行
    - 每个处理机都有自己的就绪队列
    - 调度开销小
    - 各处理机可能忙闲不均（例如*一核工作，七核在看* XD）
  - 动态进程分配
    - 进程在执行中可分配到任意空闲处理机执行
    - 所有处理机共享一个公共的就绪队列
    - 调度开销大
    - 各处理机的负载是均衡的

### 6. 优先级反置

> 优先级反置（Priority Inversion），是操作系统中出现的**高优先级进程**长时间等待**低优先级进程**所**占用的资源**的现象。
>
> 基于优先级的可抢占调度算法存在优先级反置。

- 优先级继承（Priority Inheritance）
  - 占用资源的**低优先级**进程**继承**申请资源的**高优先级**进程的优先级。
  - 只在占有资源的低优先级进程**被阻塞时**，才提高占有资源进程的优先级。
- 优先级天花板协议（Priority ceiling protocol）
  - 占用资源进程的优先级和所有可能的申请该资源的进程的最高优先级相同。
  - 不管是否发生等待，都提升占用资源进程的优先级。
  - 优先级高于系统中所有被锁定的资源的优先级上限，任务执行临界区时就不会被阻塞。

## 练习解答

### 0) 练习0

> 填写已有实验

先将Lab5中的相关代码照搬过来，然后修改`alloc_proc`的初始化，以及系统中断里的**时钟中断**这两处即可。

```cpp
static struct proc_struct * alloc_proc(void) {
    struct proc_struct *proc = kmalloc(sizeof(struct proc_struct));
    if (proc != NULL) {
        proc->state = PROC_UNINIT;
        proc->pid = -1;
        proc->runs = 0;
        proc->kstack = 0;
        proc->need_resched = 0;
        proc->parent = NULL;
        proc->mm = NULL;
        memset(&(proc->context), 0, sizeof(struct context));
        proc->tf = NULL;
        proc->cr3 = boot_cr3;
        proc->flags = 0;
        memset(proc->name, 0, PROC_NAME_LEN);
        // Lab5 code
        proc->wait_state = 0;
        proc->cptr = proc->optr = proc->yptr = NULL;
        // Lab6 新增code
        proc->rq = NULL;
        list_init(&(proc->run_link));
        proc->time_slice = 0;
        proc->lab6_run_pool.left = proc->lab6_run_pool.right = proc->lab6_run_pool.parent = NULL;
        proc->lab6_stride = 0;
        proc->lab6_priority = 1;
    }
    return proc;
}
```

```cpp
case IRQ_OFFSET + IRQ_TIMER:
    ticks++;
    assert(current != NULL);
    sched_class_proc_tick(current);
    break;
```

### 1) 练习1

> 使用 Round Robin 调度算法（不需要编码）

- 请理解并分析sched_class中各个函数指针的用法，并结合Round Robin 调度算法描ucore的调度执行过程
  - `sched_class`中各个函数指针的用法
    - `sched_class`的定义如下

      ```cpp
      // The introduction of scheduling classes is borrrowed from Linux, and makes the
      // core scheduler quite extensible. These classes (the scheduler modules) encapsulate
      // the scheduling policies.
      struct sched_class {
          // the name of sched_class
          const char *name;
          // Init the run queue
          void (*init)(struct run_queue *rq);
          // put the proc into runqueue, and this function must be called with rq_lock
          void (*enqueue)(struct run_queue *rq, struct proc_struct *proc);
          // get the proc out runqueue, and this function must be called with rq_lock
          void (*dequeue)(struct run_queue *rq, struct proc_struct *proc);
          // choose the next runnable task
          struct proc_struct *(*pick_next)(struct run_queue *rq);
          // dealer of the time-tick
          void (*proc_tick)(struct run_queue *rq, struct proc_struct *proc);
          /* for SMP support in the future
           *  load_balance
           *     void (*load_balance)(struct rq* rq);
           *  get some proc from this rq, used in load_balance,
           *  return value is the num of gotten proc
           *  int (*get_proc)(struct rq* rq, struct proc* procs_moved[]);
           */
      };
      ```

    - 其中，`const char *name`指向了**当前调度算法的名称**字符串
    
    - `void (*init)(struct run_queue *rq)`用于**初始化**传入的就绪队列。RR算法中只初始化了对应`run_queue`的`run_list`成员。
    
  ```cpp
      static void
      RR_init(struct run_queue *rq) {
          list_init(&(rq->run_list));
          rq->proc_num = 0;
      }
  ```
  
    - `void (*enqueue)(struct run_queue *rq, struct proc_struct *proc)`用于将某个进程**添加**进传入的队列中。RR算法除了将进程添加进队列中，还重置了相关的时间片。
  
      ```cpp
      static void
      RR_enqueue(struct run_queue *rq, struct proc_struct *proc) {
          assert(list_empty(&(proc->run_link)));
          list_add_before(&(rq->run_list), &(proc->run_link));
          if (proc->time_slice == 0 || proc->time_slice > rq->max_time_slice) {
              proc->time_slice = rq->max_time_slice;
        }
          proc->rq = rq;
          rq->proc_num ++;
      }
      ```
    
    - `void (*dequeue)(struct run_queue *rq, struct proc_struct *proc)`用于将某个进程从传入的队列中**移除**。以下是RR算法的实现
    
      ```cpp
      static void
      RR_dequeue(struct run_queue *rq, struct proc_struct *proc) {
          assert(!list_empty(&(proc->run_link)) && proc->rq == rq);
          list_del_init(&(proc->run_link));
          rq->proc_num --;
      }
      ```
    
    - `struct proc_struct *(*pick_next)(struct run_queue *rq)`用于在传入的就绪队列中**选择**出一个最适合运行的进程（**选择进程但不将从队列中移除**）。在RR算法中每次都只选择队列最前面那个进程。
  
      ```cpp
      static struct proc_struct *
      RR_pick_next(struct run_queue *rq) {
          list_entry_t *le = list_next(&(rq->run_list));
          if (le != &(rq->run_list)) {
              return le2proc(le, run_link);
          }
          return NULL;
      }
      ```
    
    - `void (*proc_tick)(struct run_queue *rq, struct proc_struct *proc)`。该函数会在时间中断处理例程中被调用，以减小当前运行进程的剩余时间片。若时间片耗尽，则设置当前进程的`need_resched`为1。
    
      ```cpp
      static void
      RR_proc_tick(struct run_queue *rq, struct proc_struct *proc) {
          if (proc->time_slice > 0) {
              proc->time_slice --;
          }
          if (proc->time_slice == 0) {
              proc->need_resched = 1;
          }
      }
      ```
  - 结合`Round Robin`调度算法描uCore的调度执行过程
    - 首先，uCore调用`sched_init`函数用于初始化相关的就绪队列。
    - 之后在`proc_init`函数中，建立第一个内核进程，并将其添加至就绪队列中。
    - 当所有的初始化完成后，uCore执行`cpu_idle`函数，并在其内部的`schedule`函数中，调用`sched_class_enqueue`将**当前进程**添加进就绪队列中（因为当前进程要被切换出CPU了）
      然后，调用`sched_class_pick_next`获取就绪队列中可被轮换至CPU的进程。如果存在可用的进程，则调用`sched_class_dequeue`函数，将该进程移出就绪队列，并在之后执行`proc_run`函数进行进程上下文切换。
    - 需要注意的是，每次时间中断都会调用函数`sched_class_proc_tick`。该函数会减少当前运行进程的剩余时间片。如果时间片减小为0，则设置`need_resched`为1，并在时间中断例程完成后，在`trap`函数的剩余代码中进行进程切换。
  
      ```cpp
      void trap(struct trapframe *tf) {
          if (current == NULL)
              trap_dispatch(tf);
          else {
              struct trapframe *otf = current->tf;
              current->tf = tf;
  
              bool in_kernel = trap_in_kernel(tf);
              // 执行对应的中断处理例程
              trap_dispatch(tf);
              // 恢复对应的trapframe
              current->tf = otf;
              // 如果当前中断的是用户进程
              // 注意这里体现出用户进程的可抢占性
              if (!in_kernel) {
                  if (current->flags & PF_EXITING)
                      do_exit(-E_KILLED);
                  // 如果在中断处理例程中设置need_resched为1，则在此处切换进程
                  if (current->need_resched)
                      schedule();
              }
          }
      }
      ```
  
- 请在实验报告中简要说明如何设计实现”多级反馈队列调度算法“，给出概要设计，鼓励给出详细设计
  - 多级反馈队列算法与时间片轮换算法类似，但又有所区别。该算法需要设置多个`run_queue`，而这些`run_queue`的`max_time_slice`需要按照优先级依次递减。
  - 在`sched_init`函数中，程序先初始化这些`run_queue`，并依次从大到小设置`max_time_slice`。

    > 例如队列一的`max_time_slice`为7，队列二的`max_time_slice`为5，队列三的`max_time_slice`为3。
  
  - 而执行`sched_class_enqueue`时，先判断当前进程是否是新建立的进程。如果是，则将其添加至最高优先级（即时间片最大）的队列。如果当前进程是旧进程（即已经使用过一次或多次CPU，但进程仍然未结束），则将其添加至下一个优先级的队列，因为该进程可能是IO密集型的进程，CPU消耗相对较小。
  
    > 如果原先的队列已经是最低优先级的队列了，则重新添加至该队列。
  
  - `sched_class_pick_next`要做的事情稍微有点多。首先要确认下一次执行的该是哪条队列里的哪个进程。为便于编码，我们可以直接指定切换至队列中的**第一个**进程（该进程是**等待执行时间**最久的进程）。

    但队列的选择不能那么简单，因为如果只是简单的选择执行**第一个队列**中的进程，则大概率会产生**饥饿**，即低优先级的进程长时间得不到CPU资源。所以，我们可以设置每条队列占用**固定时间/固定百分比**的CPU。例如在每个队列中添加一个`max_list_time_slice`属性并初始化，当该队列中的进程**总运行时间**超过当前进程所在队列的`max_list_time_slice`（即**最大运行时间片**），则CPU切换至下一个队列中的进程。

### 2) 练习2

> 实现 Stride Scheduling 调度算法（需要编码）

#### a. Stride调度算法的相关介绍

uCore的Round-Robin算法可以保证每个进程得到的CPU资源是相等的，但我们希望调度器能够更加智能的为每个进程分配合理的CPU资源，让**每个进程得到的时间资源与它们的优先级成正比关系**。而Stride Scheduling调度算法就是这样的一种典型而简单的算法。

其中，该算法的有如下几个特点：

- 实现简单
- 可控性：可以证明Stride Scheduling对进程的调度次数正比于其优先级
- 确定性：在不考虑计时器事件的情况下，整个调度机制都是可预知和重现的。

而该算法的基本思想如下：

1. 为每个runnable的进程设置一个当前状态stride，表示该进程当前的调度权。另外定义其对应的pass值，表示对应进程在调度后，stride 需要进行的累加值。
2. 每次需要调度时，从当前 runnable 态的进程中选择 stride最小的进程调度。
3. 对于获得调度的进程P，将对应的stride加上其对应的步长pass（只与进程的优先权有关系）。
4. 在一段固定的时间之后，回到 2.步骤，重新调度当前stride最小的进程。

> 可以证明，如果令 P.pass = BigStride / P.priority 其中 P.priority 表示进程的优先权（大于 1），而 BigStride 表示一个预先定义的大常数，则该调度方案为每个进程分配的时间将与其优先级成正比。

不过这里有个点需要注意一下，随着进程的执行，stride属性值会一直在增加，那么就有可能造成整数溢出。当stride溢出后，不当的比较可能会造成错误。那应该怎么做呢？

这里有一个结论：`STRIDE_MAX – STRIDE_MIN <= PASS_MAX == BIG_STRIDE / 1` （注意最小的Priority为1）。所以我们只要将`BIG_STRIDE`限制在某个范围内，即可保证**任意两个stride之差都会在机器整数表示的范围之内**。

而又因为溢出数a减去非溢出数b的结果**仍然是正确**的，例如

```cpp
uint32_t a = ((uint32_t) -1); // 此时a为uint32_t的最大值
uint32_t b = 4;
cout << b - a; // 输出 5, 即 4 > ((uint32_t) -1)
```

所以，我们只需将`BIG_STRIDE`的值限制在一个`uint32_t`所能表示的范围(uint32_t为uCore所设置的stride值的类型)，这样就可避开stride的溢出。

```cpp
#define BIG_STRIDE ((uint32_t) -1)
```

由于`Stride Scheduling`算法涉及到大量的查找，故我们可以使用斜堆`skew_heap`数据结构来提高算法效率。该数据结构在uCore中已提供，我们无需关注其具体细节，直接调用即可。

#### b. 具体实现

- `stride_init`简简单单的一个初始化

  ```cpp
  static void
  stride_init(struct run_queue *rq) {
      list_init(&(rq->run_list));
      // 注意这里不要使用skew_heap_init(rq->lab6_run_pool)
      rq->lab6_run_pool = NULL;
      rq->proc_num = 0;
  }
  ```

  需要注意的是，初始化`rq->lab6_run_pool`时请直接赋值NULL即可，而不要使用`skew_heap_init`函数，因为`rq->lab6_run_pool`**只是一个指针，而不是一个对象**。

- `stride_enqueue`和`stride_dequeue`与RR算法相差不大

  不过要注意的是，在插入或删除一个进程后，**一定要更新`rq->lab6_run_pool`指针！**

  ```cpp
  static void
  stride_enqueue(struct run_queue *rq, struct proc_struct *proc) {
      rq->lab6_run_pool = skew_heap_insert(rq->lab6_run_pool, &(proc->lab6_run_pool), proc_stride_comp_f);
      if (proc->time_slice == 0 || proc->time_slice > rq->max_time_slice) {
          proc->time_slice = rq->max_time_slice;
      }
      proc->rq = rq;
      rq->proc_num ++;
  }
  
  static void
  stride_dequeue(struct run_queue *rq, struct proc_struct *proc) {
      rq->lab6_run_pool = skew_heap_remove(rq->lab6_run_pool, &(proc->lab6_run_pool), proc_stride_comp_f);
      rq->proc_num --;
  }
  ```

- `pick_next`函数中涉及到了选取最小`Stride`值的进程，以及`stride`值的更新。

  由于uCore中的函数`proc_stride_comp_f`已经给出源码，结合对应斜堆代码的理解，我们可以得出：**stride值最小的进程在斜堆的最顶端**。所以`pick_next`函数中我们可以直接选取`rq->lab6_run_pool`所指向的进程。

  而`stride`值可以直接加上`BIG_STRIDE / p->lab6_priority`来完成该值的更新。不过这里有个需要注意的地方，除法运算是不能除以0的，所以我们需要在`alloc_proc`函数中将每个进程的`priority`都初始化为1.

  ```cpp
  static int
  proc_stride_comp_f(void *a, void *b)
  {
       struct proc_struct *p = le2proc(a, lab6_run_pool);
       struct proc_struct *q = le2proc(b, lab6_run_pool);
       int32_t c = p->lab6_stride - q->lab6_stride;
       if (c > 0) return 1;
       else if (c == 0) return 0;
       else return -1;
  }
  
  static struct proc_struct *
  stride_pick_next(struct run_queue *rq) {
      skew_heap_entry_t* she = rq->lab6_run_pool;
      if (she != NULL) {
          struct proc_struct* p = le2proc(she, lab6_run_pool);
          p->lab6_stride += BIG_STRIDE / p->lab6_priority;
          return p;
      }
      return NULL;
  }
  ```

- `stride_proc_tick`与RR算法一致，这里不再赘述

  ```cpp
  static void
  stride_proc_tick(struct run_queue *rq, struct proc_struct *proc) {
       if (proc->time_slice > 0) {
          proc->time_slice --;
      }
      if (proc->time_slice == 0) {
          proc->need_resched = 1;
      }
  }
  ```

### 3) 扩展练习

#### 1. Challenge 1

> 实现Linux CFS算法

CFS （完全公平调度器）实现的主要思想是维护为任务提供处理器时间方面的平衡（公平性）。它给每个进程设置了一个虚拟时钟vruntime。其中$vruntime = 实际运行时间 * 1024 / 进程权重$。

进程按照各自不同的速率在物理时钟节拍内前进，优先级高则权重大，其虚拟时钟比真实时钟跑得慢，但获得比较多的运行时间；CFS调度器总是选择虚拟时钟跑得慢的进程来运行，从而让每个调度实体的虚拟运行时间互相追赶，进而实现进程调度上的平衡。

CFS使用**红黑树**来进行快速高效的插入和删除进程。

> 具体实现与Stride Scheduling类似，只是稍微有些不同。咕咕咕~

参考链接：

- [linux内核分析——CFS（完全公平调度算法）](https://www.cnblogs.com/tianguiyu/articles/6091378.html)

- [Linux内核CFS调度器](https://www.cnblogs.com/XiaoliBoy/p/10410686.html)

#### 2. Challenge 2

> 在ucore上实现尽可能多的各种基本调度算法(FIFO, SJF,...)，并设计各种测试用例，能够定量地分析出各种调度算法在各种指标上的差异，说明调度算法的适用范围。

> 这个，告辞~
