# CS143 编译原理学习

## 简介

这次学习编译原理所使用的课程是Stanford的CS143编译原理课程，其中配套了Program Assignment Lab

## 1. lexer - 词法分析

- 编写相应的词法分析器`flex`脚本，就可以输出一个对应的C++源码，将之编译即可得到lexer
- `flex`脚本中，我们主要编写的是正则表达式的匹配规则，匹配对应的字符串，返回一个个的`token`。  
- lexer通过正则语法，将对应读入的词转换为一个个的`token`
- 在词法分析这个过程中，可以过滤出比较明显的错误，例如
  - 引号、注释符的闭合
  - 非源码中可以出现的字符，比如`ASCII < 32`的各种控制符号以及特殊的转义字符等等
  - 过长的字符串
  - 不在规定范围内的转义字符，例如`\\`转义反斜杠不该出现在字符串双引号之外
  - 以及其他的通过读取字符就可以找到的各种简单错误
- 一个简单的例子
  
  ```cpp
  /* 编写正则表达式 */
  DIGIT     [0-9]
  LETTER    [a-zA-Z]
  ID        ({LETTER}|{DIGIT}|_)
  TYPEID    [A-Z]{ID}*
  /* 匹配到对应的符号则执行方括号内的操作 */
  {TYPEID} {
    /* 将当前识别到的typeid添加进字符表中，使其不添加重复字符串 */
    cool_yylval.symbol = idtable.add_string(yytext);
    /* 将当前识别到的token返回 */
    return TYPEID;
  }
  ```

- 正则表达式的规则很容易理解，但是正则表达式并不能直接用来解析字符串，所以还要引入一种适合转化为计算机程序的模型 —— __有穷自动机__(finite automation，FA)
  > flex的核心就是自动机。
  - 有穷自动机首先包含一个有限状态的集合，还包含了从一个状态到另外一个状态的转换。有穷自动机看上去就像是一个有向图，其中状态是图的节点，而状态转换则是图的边。此外这些状态中还必须有一个初始状态和至少一个接受状态。
  - 有穷自动机处理输入字符串的流程
    - 初始情况下，自动机处于初始状态
    - 读取字符串的第一个字符，这时自动机会查询当前状态上与输入字符相匹配的边，并沿这条边转换到下一个状态。
    - 继续输入下一个字符，重复第二步，查询当前状态上的边并进行状态转换。
    - 当字符串全部输入后，如果自动机正好处于接受状态上，就说该自动机 __接受了这一字符串__。
  - 有穷自动机分为两种
    - 确定性有穷自动机（DFA）。特点：从每一个状态只能发出一条具有某个符号的边 —— 不能出现同一个符号出现在同一状态发出的两条边上。
    - 非确定性有穷自动机(NFA)。 特点：允许从一个状态发出多条具有相同符号的边，甚至允许发出标有ε（表示空）符号的边，即NFA可以不输入任何字符就自动沿ε边转换到下一个状态。
      > NFA与DFA __等价__，且NFA 可以转化为 DFA

- flex所生成的代码，其大致流程为：
  - 初始化相关内存与状态
  - 从上一次读取的位置开始，循环读取单个字节，并根据`yy_nxt`状态表与`yy_current_state`当前状态，跳转至下一个状态
  - 待无法跳转至下一个状态时（即匹配完成或出错），根据当前状态，在`yy_accept`动作表中确定`yy_act`动作
  - 根据`yy_act`执行特定操作（用户定义的操作或异常捕获）
  > 注：状态表等是自动机中的重中之重。
- 参考
  - [flex中文文档](http://eternalsakura13.com/2020/05/27/flex/)
  - __偏向代码分析__ 的 [flex生成代码分析](https://happyers.top/uncategorized/understanding-c-lexer-generated-by-flex/) - [github](https://github.com/pcy190/CompilerAnalysis/blob/master/flex_analysis.md)

## 2. parser - 语法分析

- 语法分析所使用的工具是`bison`，`bison`与`flex`类似，但又有所不同。
- `bison`所使用的是自底向上，左递归的分析方式。
- 在语法分析这个过程中，可以过滤出一些不符合语法的错误，例如token排列不符合条件，无法规约。  
  在这种情况下必须进行错误处理程序，将token弹出栈（或者其他操作）。
- 语法分析所得到的结果是一个不全面的AST抽象语法分析树。它只是一个由Token简单组合起来的树，仍然需要二次处理。
- 一个简单的例子
  
  ```cpp
  /* If no parent is specified, the class inherits from the Object class. */
  /* 定义：以下token规约后的符号名称为"class" */
  class :
    /* 若当前栈中的token满足下面这条式子 */
    CLASS TYPEID '{' dummy_feature_list '}' ';' /* ' */
      /* 进行规约。在规约的同时执行以下这条式子 */
      /* 注意，赋给$$的值，就是规约后新生成"class"的值 */
      { $$ = class_($2,idtable.add_string("Object"), $4, stringtable.add_string(curr_filename)); }
    | /* 或者，如果满足以下式子 */
    CLASS TYPEID INHERITS TYPEID '{' dummy_feature_list '}' ';'
      { $$ = class_($2,$4,$6,stringtable.add_string(curr_filename)); }
    | /* 或者，如果捕获到了错误 */
    error ';'
      {}
  ;
  ```

- 参考
  - __偏向状态表分析__ [bison生成代码的分析](https://happyers.top/compiler/bison-parser/) - [github](https://github.com/pcy190/CompilerAnalysis/blob/master/bison_analysis.md)

## 3. semant - 语义分析

- 语义分析要对语法分析所生成的不全面的AST进行二次处理，并捕获 __所有__ 剩余的错误。
- 在语义分析中，必须填补缺失的信息，例如每个节点的类型。
- 在此过程中，所捕获的错误包括但不限于
  - 类型不匹配
  - 变量名未定义
  - 类继承成环状关系
- 以cool语言为例，该语言的语义分析需要经过以下步骤
  - 将基础类（`Object`，`IO`，`String`，`Bool`），添加至classTable中
  - 将用户定义的类，添加进classTable中
  - 检查是否有不正确的类继承。即检查是否存在某个类继承了一个 __未声明或无法被继承__ 的类等等。
  - 将所有类按照继承关系建立 __继承树__ 。例如：

    ```text
              { -> [String]
              { -> [Int]
    [OBJECT] ={ -> [IO]     -> [B]
              { -> [Bool]
              { -> [A]
    ```
  
    `InheritanceNode`是类继承的基本单元。每个`InheritanceNode`都和一个cool语言中的类绑定。同时每个`InheritanceNode`中都存在一个指向`Environment`的指针成员。

  - 从根类（即从`Object`类）开始，自顶向下递归设置`InheritanceNode`为 __“可到达的”__。
  - 遍历全部类，判断是否有`InheritanceNode`是 __“不可到达的”__ 。 如果存在，则说明类继承关系中存在环，报错终止。（例如上图）
    > 注意：由于cool语言是单继承的，每个类都只能继承一个类。所以这种判断是可行的。
  - 递归建立`feature_table`。从 __继承树__ 的根开始（即`Object`类），自顶向下递归建立每个`InheritanceNode`中`Environment`的`feature_table`。该操作会将cool语言中，每个类内的`attribute`和`method`均添加进表中。  
    当父节点的`feature_table`建立完成后，子节点便会复制父节点的`Environment`，并在其上进行添加操作。这样，父节点的属性自然就包含于子节点的属性中。  
  - 检查是否存在`Main`类以及`main`方法。
  - 递归进行类型检查。从 __继承树__ 的根开始，__自顶向下__ 检查每个类的类型，检查是错误，以及为AST添加缺失的类型信息。  
    在每个类中，类型检查是 __自底向上__ 的，由节点较低的类型生成节点较高的类型，如此反复直至树根。其中需要捕获大量的错误，以及正确处理变量的作用域等等。
  - 语义分析结束，输出完整的AST。
- 语义分析的功能，归根到底就只有两点
  - 捕获所有错误。
  - 补充AST缺失的信息。  
- 语义分析需要对每个表达式都进行类型检查，所以类型检查代码的编写是十分重要的。每个表达式所确定的类型也必须严格按照对应编程语言手册里的相关规定来设计，不可随意更改。

## 4. cgen - 目标代码生成

- 以cool语言为例，在生成目标代码前，需要先读入AST树的相关信息，重建继承图，并自上而下的初始化相关的映射表格  
  在该初始化过程中，每个类会遍历其中的`feature`，并将其相关信息添加至对应的`SymbolTable`中  
  如果该`feature`是`method`，则还会额外自顶向下计算所需要的最小临时变量数量，并将其添加进表格中。
- 初始化过后就是生成代码，这个步骤会执行以下几个过程：
  - 声明全局变量。例如以下mips汇编代码：

    ```asm
      .data
      .align  2
      .globl  class_nameTab
      .globl  Main_protObj
      .globl  Int_protObj
      .globl  String_protObj
      .globl  bool_const0
      .globl  bool_const1
      .globl  _int_tag
      .globl  _bool_tag
      .globl  _string_tag

    _int_tag:
      .word 3
    _bool_tag:
      .word 4
    _string_tag:
      .word 5
      .globl _MemMgr_INITIALIZER
    ```
  
  - 声明GC器。例如以下汇编代码：

    ```asm
    _MemMgr_INITIALIZER:
      .word _NoGC_Init
      .globl _MemMgr_COLLECTOR
    _MemMgr_COLLECTOR:
      .word _NoGC_Collect
      .globl _MemMgr_TEST
    _MemMgr_TEST:
      .word 0
      .word -1
    ```
  
  - 将常量输出（例如：数字，字符串，布尔常量），例如以下部分汇编代码
    > 注意：数字常量包括`0`，字符串常量包括空字符串`""`
  
    ```asm
      .word -1              # eye catcher for _gc_check
    str_const8:             # 该字符串的标签
      .word 5               # （未知用途）
      .word 6               # （未知用途）
      .word String_dispTab  # 该类型可以使用的方法
      .word int_const2      # 字符串长度（其中的int_const2指向另一个数字常量）
      .ascii "Main"         # 字符串的ASCII码
      .byte 0               # 字符串末尾的\0终结符
      .align 2              # 对齐
    ```

  - 将所有类的名称输出。例如以下汇编代码：

    ```asm
    class_nameTab:        # 这一个个的str_const都指向特定的字符串
      .word str_const6    # str_const6  => "Object"
      .word str_const7    # str_const7  => "IO"
      .word str_const8    # str_const8  => "Main"
      .word str_const9    # str_const9  => "Int"
      .word str_const10   # str_const10 => "Bool"
      .word str_const11   # str_const11 => "String"
    ```

  - 将所有类中的object table输出（未知用途，删除后仍然可以执行）

    ```asm
    class_objTab:
      .word  Object_protObj
      .word  Object_init
      .word  IO_protObj
      .word  IO_init
      .word  Main_protObj
      .word  Main_init
      .word  Int_protObj
      .word  Int_init
      .word  Bool_protObj
      .word  Bool_init
      .word  String_protObj
      .word  String_init
    ```

  - 将每个类所含的方法输出（包括该类的继承类中的方法），例如以下汇编代码

    ```asm
    Main_dispTab:
      .word  Object.abort
      .word  Object.type_name
      .word  Object.copy
      .word  IO.out_string
      .word  IO.out_int
      .word  IO.in_string
      .word  IO.in_int
      .word  Main.main
    ```

  - 将每个类的类型信息输出。`protObj`中含有当前类的属性以及函数表。例如以下部分汇编代码

    ```asm
        .word  -1         # -1 header for the garbage collector(eye catcher for _gc_check)
    Main_protObj:         # label
      .word  2            # class tag
      .word  7            # total_attributes + DEFAULT_OBJFIELDS
      .word  Main_dispTab # 函数表
      .word  int_const0   # 第一个attribute是数字类型
      .word  str_const12  # 第二个attribute是字符串类型
      .word  bool_const0  # 第三个attribute是布尔类型
      .word  0            # 第四个attribute是其他类型，例如各种类
    ```
  
  - 声明全局代码段的相关信息，例如以下汇编代码

    ```asm
    #声明堆的相关信息
      .globl  heap_start
    heap_start:
      .word  0
    #声明text代码段
      .text
      .globl  Main_init
      .globl  Int_init
      .globl  String_init
      .globl  Bool_init
      .globl  Main.main
    ```
  
  - 输出每个类的初始化函数的代码，例如以下汇编代码

    ```asm
    String_init:
      addiu  $sp $sp -12
      sw  $fp 12($sp)
      sw  $s0 8($sp)
      sw  $ra 4($sp)
      addiu  $fp $sp 4
      move  $s0 $a0
      jal  Object_init
      move  $a0 $s0
      lw  $fp 12($sp)
      lw  $s0 8($sp)
      lw  $ra 4($sp)
      addiu  $sp $sp 12
      jr  $ra  
    ```

  - __输出每个类中的函数代码__（示例代码就不放出来了）
- 在给出的课程文件夹中，文件`trap.handler`是一个不可或缺的存在。相当一部分重要函数被定义在其中，例如`out_string`以及垃圾回收器`GC`  等等。  
  mips汇编的执行流程如下：
  - 首先执行`__start`
  - 在`__start`中，初始化垃圾回收器`GC`
  - 设置`_exception_handler`异常处理程序
  - 复制一份`Main`类，并将其保存至栈上
  - 调用`Main.init`进行初始化
  - 执行`Main.main`
  - 待函数返回后恢复栈
  - 输出终止信息并调用`exit`退出

  所以在生成main函数的代码时，不需要额外调用`abort`，只要将其视为一个普通的函数调用就好。
  >  `trap.handler`中的函数，其参数传递方式为寄存器传递，与约定俗成的压栈传递有所不同，在阅读源码时注意甄别。
- 在生成的mips汇编代码中，当调用另一个函数时，程序会
  - 待调用函数的参数全部压栈
  - 加载 __当前类__ 的`dispatch_table`
  - 加上某个特定偏移
  - 得到待调用函数的具体位置
  - `jalr`无条件跳转
- 在mips汇编代码中，当调用new时，会执行以下流程

  ```asm
  la  $a0 IO_protObj  # 将待生成对象的静态地址赋给$a0
  jal Object.copy     # 调用copy复制一份到堆内存中（参数：$a0指向待复制的类对象；返回：$a0指向堆内存中复制出的类对象）
  jal IO_init         # 调用init方法初始化（参数：$a0指向复制出的类对象
  ```

  即，整个过程结束时，`$a0`指向新建的类对象

- `self`指针（`$s0`）的改变过程
  - 当`Main`类被新建出来后，`$a0`便指向堆上所复制出`Main`类的地址
  - 执行`Main.init`初始化（参数`$a0`指向堆上复制出的类）。之后执行`Main.main`函数。
  - 在每个函数开头，`$a0`都存放当前函数所属类的地址。故我们可以认定，`$a0`是类成员函数的隐藏参数，与C++类中的`this`指针类似，都是指向此函数所属的类。
  - 在每个成员函数中，
    - 函数开始时，`$s0`会先将上一个类地址存进栈中，然后将当前类地址`$a0`赋给`$s0`，使`$s0`指向新类的地址。  
      > 为什么要将`self`放至`callee save`的寄存器中呢？请看下述代码：
  
        ```java
        class A{
          func() : Object {{
            1;
          }};
        };

        class Main {
          main():Object {{
                (new A).func();
          }};
        };
        ```

      > 当执行`A.func`函数时，其`old $s0`指向`Main`类，而传入的`$a0`指向`A`类。所以`$s0`需要在函数中保存。

    - 之后`$a0`便作为中间寄存器，参与当前函数中的表达式计算过程。
    - 在当前函数中调用函数，则会取得`$s0`的相对偏移处的类函数表`dispatch_table`，并根据特定偏移取得待调用函数的地址。若是取当前类中的成员变量，则同样使用`$s0`取得`protObj`中的成员信息。

      ```asm
      Main_protObj:
        .word 2
        .word 4
        .word Main_dispTab      # 函数表，offset = 8
        .word int_const2        # main中的一个成员变量，offset = 12

      Main_dispTab:
        .word Object.abort
        .word Object.type_name
        .word Object.copy
        .word IO.out_string
        .word IO.out_int        # 函数out_int，offset = 16
        .word IO.in_string
        .word IO.in_int
        .word Main.main

      Main.main:
        #...

        lw  $a0 12($s0)         # 获取Main类的特定成员变量地址
        sw  $a0 0($sp)          # 压栈，作为下一个函数的参数
        addiu $sp $sp -4
        move  $a0 $s0           # 设置$a0为self，便于在下面中定位函数

        #...

        lw  $t1 8($a0)          # 获取Main_dispTab地址
        lw  $t1 16($t1)         # 获取Main.out_int函数地址
        jalr  $t1               # 无条件跳转执行
      ```

    - 函数返回时，将`$s0`重新赋给`$a0`，并将`$s0`恢复至上一个类地址。
    - 这一整个流程下来，`$s0`和`$a0`的值不变，函数执行前与函数执行后的值一致相同。

- cool-mips中的GC
  - cool语言的`cgen`在传入不同的参数下可以选择使用的GC类型（也可以不选）

    ```cpp
    /* handle_flag.cc */
    Memmgr cgen_Memmgr = GC_NOGC;      // enable/disable garbage collection
    Memmgr_Test cgen_Memmgr_Test = GC_NORMAL;  // normal/test GC
    Memmgr_Debug cgen_Memmgr_Debug = GC_QUICK; // check heap frequently
    /* ... */
    case 'g':  // enable garbage collection
      cgen_Memmgr = GC_GENGC;
      break;
    case 't':  // run garbage collection very frequently (on every allocation)
      cgen_Memmgr_Test = GC_TEST;
      break;
    case 'T':  // do even more pedantic tests in garbage collection
      cgen_Memmgr_Debug = GC_DEBUG;
      break;


    /* cgen.cc */
    static char *gc_init_names[] =
      { "_NoGC_Init", "_GenGC_Init", "_ScnGC_Init" };
    static char *gc_collect_names[] =
      { "_NoGC_Collect", "_GenGC_Collect", "_ScnGC_Collect" };

    void CgenClassTable::code_select_gc()
    {
      //
      // Generate GC choice constants (pointers to GC functions)
      //
      str << GLOBAL << "_MemMgr_INITIALIZER" << endl;
      str << "_MemMgr_INITIALIZER:" << endl;
      str << WORD << gc_init_names[cgen_Memmgr] << endl;
      str << GLOBAL << "_MemMgr_COLLECTOR" << endl;
      str << "_MemMgr_COLLECTOR:" << endl;
      str << WORD << gc_collect_names[cgen_Memmgr] << endl;
      str << GLOBAL << "_MemMgr_TEST" << endl;
      str << "_MemMgr_TEST:" << endl;
      str << WORD << (cgen_Memmgr_Test == GC_TEST) << endl;
    }
    ```

  - 当`cgen_Memmgr == GC_GENGC`时，`_GenGC_Assign`函数只在`AttributeBinding::code_update`中被调用，以记录成员变量的赋值操作至`assignment table`
    > `assignment table` 用于分代垃圾回收。由于尚未学习GC的相关算法，故具体细节暂不追究。
  - 当`cgen_Memmgr_Debug == GC_DEBUG`时，`_gc_check`函数会被频繁调用，以检测对象的`eye catcher`是否存在。具体细节请查阅下述伪代码。
    > 这个check的用意，私以为是对特定变量进行检测，防止heap chunk的 __重叠/被覆盖__

    ```cpp
    // $a1 存放 堆上某个对象的起始地址
    def _gc_check($a1):
      if $a1 != 0:
        if (obj_eyecatch($a1) != -1): // obj_eyecatch: offset from object pointer to the eyecatcher  
          _gc_abort()
    ```

  - `_GenGC_Collect`方法会在`Object.copy`函数中内部调用，以分配一块堆内存

## 5. 思考题

- > 描述每种文法（LL(1),SLR, LR(1), LALR等...)的使用条件，和它是为了解决什么问题？

  - `LL(1)` :
    - 使用条件： 对于产生式A->α|β
      - 当α、β均不能推出ε时，FIRST(α) ∩ FIRST(β) = φ
      - α、β最多有一个能推出ε。同时当α、β中的某个符号推出ε时，另一个符号的FOLLOW集 ∩ FIRST(A) = φ
  - `SLR` :
    - 使用条件：不产生冲突
    - 解决的问题：`LR(0)`没有考虑文法符号的上下文环境，当进入任何一个规约状态时，总是会采取规约动作，这会造成 __归约-移入冲突__。而`SLR`限制了规约条件，也就是当进入规约状态时，只有当下一个符号属于规约项目的FOLLOW集时，才可以进行规约动作。
  - `LR(1)` :
    - 使用条件：需要忍受`LR(1)`自动机的高状态数所带来的影响。
    - 解决的问题： `SLR`只是简单判断下一个输入符号是否属于规约项目的FOLLOW集，只能排除不合理的规约而不能确保正确的规约。而`LR(1)`引入 __后继符__ 这个概念。后继符集合是FOLLOW集的子集，故可进一步限制了规约的条件。
  - `LALR` :
    - 使用条件：合并项集后所构造出的语法分析表中，不存在语法分析动作冲突。
    - 解决的问题： `LR(1)`根据展望符集合的不同，将原始的`LR(0)`项目分裂成了不同的`LR(1)`项目。这会增大`LR(1)`自动机的状态数。而`LALR`将相同核心的`LR(1)`项集合并为一个项集，从而减少自动机的状态数。

- > 可以将`!type_leq(type2, nd->get_name())`修改为`type_leq(nd->get_name(),type2)`吗？为什么？

  - 先试着改了然后编译，发现错了五个，很明显是改不了的
  - 那为什么是改不了的呢？我们需要从`type_leq`这两个函数的功能开始说起
  - `type_leq`的功能是，查找`supertype`是否位于`subtype`的传递链中。以下图为例：

      ```text
                { -> [A]
      [OBJECT] ={
                { -> [B] -> [C]
      ```


    - `type_leq(C, B)`会返回`true`
    - `type_leq(C, A)`返回`false`
  - 这就涉及到一个有趣的问题，当`A <= B`为`false`时，是否就一定是`A > B`呢？反之亦然呢？以上面的两条代码为例，我们可以很容易的得到：
    - 若`type_leq(B, C)`为`false`，则`type_leq(C, B)`为`true`
    - 若`type_leq(C, A)`为`false`，则`type_leq(A, C)`仍为`false`  
  - 很明显,`A`和`C`无论如何也没有从属关系，所以这两个类型是 __没办法比较__ 的。既然都没办法比较了，那就更无从谈起谁大谁小了。所以无论`A <= C` 还是 `C <= A` 都是`false`。
  - 所以, `type_lub`里的那条语句是无法替换的。
