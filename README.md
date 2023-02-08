# 天问之路

> ~~这里将定期记录着 一些与`Sakura`师傅以及一群小伙伴共同学习的内容与进度。~~
>
> 从 Sakura 师傅的天问之路毕业后，这里将定期记录着本人接下来的学习历程，叠加起来算是从大一年下半学期开始。
>
> 该周报大概每隔半个月 push 一次至 github 上。
---

## 第1-2周（2020.5.18-2020.5.29）

> 主要任务：[STL](week1-2/)

- 第1周（2020.5.18-2020.5.23）：学习基础的概念，例如traits之类的
- 第2周（2020.5.23-2020.5.29）：数据结构抄到list

## 第3-6周（2020.5.29-2020.6.28）

> 主要任务：[编译原理](week3-6/)

- 第3周（2020.5.29-2020.6.05）：词法分析与语法分析
- 第4周（2020.6.05-2020.6.14）：逆向CS143提供的标准语义分析器
- 第5周（2020.6.14-2020.6.21）：抄PA5目标代码生成，研究了一下flex和bison
- 第6周（2020.6.21-2020.6.28）：整了一份Compiler总结，恶补Makefile、完成LLVM IR PASS assignment1

## 第7-8周（2020.6.28-2020.7.12）

> 主要任务：[LLVM IR Pass 代码优化](week7-8/)

- 第7周（2020.6.28-2020.7.05）：完成LLVM IR Pass的所有三个Assignment，看了点GC的算法篇
- 第8周（2020.7.05-2020.7.12）：大一暑期实训（1/2周）

## 第9周（2020.7.12-2020.7.19）

> 主要任务：[AFL_LLVM_mode源码分析](https://kiprey.github.io/2020/07/AFL-LLVM-Mode/)

## 第10-11周（2020.7.19-2020.8.02）

> 主要任务：[CSAPP](week9-19/CSAPP-Lab/)

- 第10周（2020.7.19-2020.7.26）：CSAPP LAB做到第五个Lab-[Cache Lab](https://kiprey.github.io/2020/07/csapp-lab-writeup/#5-Cache-Lab)
- 第11周（2020.7.26-2020.8.02）：完成[CSAPP全部Lab](https://kiprey.github.io/2020/07/csapp-lab-writeup/)

## 第12-19周（2020.8.02-2020.9.27）

> 主要任务：[uCore Lab](week9-19/uCore) (第15-18周期末复习)

- 第12周（2020.8.02-2020.8.09）：完成[Ucore-Lab1](https://kiprey.github.io/2020/08/uCore-1/)，正在研究Lab2
- 第13周（2020.8.09-2020.8.16），完成uCore-[lab2](https://kiprey.github.io/2020/08/uCore-2/) [lab3](https://kiprey.github.io/2020/08/uCore-3/)
- 第14周（2020.8.16-2020.8.23）：信安国赛 + 朗诵比赛准备。完成[Lab4](https://kiprey.github.io/2020/08/uCore-4/)和[Lab5](https://kiprey.github.io/2020/08/uCore-5/)（除了Challenge）。Lab5 Challenge的写时复制代码已经完成。
- 第15-18周（2020.8.23-2020.9.20）：大一下期末复习 + 期末考
- 第19周（2020.9.20-2020.9.27）：uCore完成[Lab6](https://kiprey.github.io/2020/09/uCore-6/)、[Lab7](https://kiprey.github.io/2020/09/uCore-7/)，uCore-Lab8视频刚开始看，笔记还没来得及做

> 从第20周开始，所有涉及的资料以及分析等等均在[个人博客](https://kiprey.github.io)上发布。

## 第20周（2020.9.27-2020.10.04）

- [uCore-Lab8](https://kiprey.github.io/2020/09/uCore-8/) 终于结束，PlaidCTF2020 Mojo还在挣扎，看了点调试器的文章

## 第21周（2020.10.04-2020.10.11）

- [Mojo](https://kiprey.github.io/2020/10/mojo/)完成，CVE-2019-5826在看blackhat稿子

## 第22周（2020.10.11-2020.10.18）

- [CVE-2019-5826 POC分析](https://kiprey.github.io/2020/10/CVE-2019-5826/)完成， CVE-2020-6549 render UAF 才看一点点

## 第23周（2020.10.19-2020.10.25）

- 完成 [CVE-2020-6549 render UAF的POC分析](https://kiprey.github.io/2020/10/CVE-2020-6549/)，完善当前study list，以及阅读了一下C++ case，了解chrome的一些基本结构与方法。

## 第24周（2020.10.26-2020.11.01）

- 完成[CVE-2020-6541分析](https://kiprey.github.io/2020/10/CVE-2020-6541/)
  - 10.27：看了一点CVE-2020-6541的内容，尝试追溯其调用链
  - 10.28：该漏洞研究完成，笔记已上传
  - 10.29：下拉V8代码并编译（代理设置搞了半天才好）
  - 10.30：忙学校的事情
  - 10.31：忙评奖的事情
  - 11.01：打湖湘杯

## 第25周（2020.11.02-2020.11.08）

> JSPromise  type confusion学习

- 11.02：装了一下[issue1784](https://bugs.chromium.org/p/project-zero/issues/detail?id=1784)的调试环境
- 11.03：阅读了一个下午的issue1784，发现装环境的方向错了，再装了一个chrome release
- 11.04：仔细审计v8源码，终于理解JS的type confusion
- 11.05：上课
- 11.06：尝试找到blink-in-js调试方式，无果
- 11.07：审计一天，遇到了相当多的问题，暂时无法解决。不过基本上已经了解大部分关于Promise的内容，达到了目的，可以开始下一步的学习。
- 11.08：肝课程作业

## 第26周（2020.11.09-2020.11.15）

> CodeQL学习、寒假实习面试

- 11.09：CodeQL配置环境
- 11.10：大二寒假实习面试
- 11.10：CodeQL重新配置环境
- 11.11：完成Chromium下载链接爬虫
- 11.12：阅读CodeQL脚本
- 11.13：阅读CodeQL doc，了解其基本语法
- 11.14-11.15：完成日常课程作业

> 试用了一段时间，发现日报模式可能不太适合我，因此切回周报模式。

## 第27周（2020.11.16-2020.11.22）

- 下拉chromium 24gb的源码、期中复习（1/2）

## 第28周（2020.11.23-2020.11.29）

- 期中复习（2/2）

## 第29周（2020.11.30-2020.12.6）

- 完成学校4份实验+6份报告

## 第30周（2020.12.7-2020.12.13）

- CodeQL完成所有基础语法的学习
- 编写了检测V8callback的ql来练手

## 第31周（2020.12.14-2020.12.20）

- CodeQL编写了DispatchEvent的QL，仍然存在一些问题
- 完成学校5份实验7份报告

## 第32周（2020.12.21-2020.12.27）

- 看了几个github securityLab关于CodeQL的几个例子，写了篇博客
- 同时还完善了扫描DispatchEvent的QL代码

## 第33周（2020.12.28-2021.1.3）

- 学习GoogleCTF2018(Final) Just-In-Time，刚配置好环境

## 第34-36周（2021.1.4-2021.1.24）

- 期末复习+考试
- 完成V8 turboFan的学习。

## 第37-41周（2021.1.25-2021.2.28）

- 静态编译器研发实习（关于静态指针分析方向 之 FlowSensitive，以及学习构建控制依赖图算法 CDG）
- 完成CVE-2021-3156 sudo 提权漏洞的简单分析。
- 了解了点 V8 的 Gabage Collection
- 学习 V8 历史漏洞
  - CVE-2019-13764 TypeInductionVariablePhi in v8 JIT分析
  - V8 CVE-2019-5755 MinusZero类型缺失漏洞 in turboFan
  
## 第42周（2021.3.1-2021.3.7）

- 分析了 CVE-2018-16065 in V8 EmitBigTypedArrayElementStore
- 做了个针对 BurpSuite v2.0 beta 的 exploit
  
## 第43周（2021.3.8-2021.3.14）

- 做了个针对 BurpSuite pro v2020.2 的内置 chromium 的 exploit（由于开启沙箱因此最终无法利用），学习了一下 v8 指针压缩的漏洞利用
- 学习 IDAPython 插件的编写，学习 ida 指令回溯的算法实现，完善firmeye 插件在 x86 架构下的实现。
  
## 第44周（2021.3.15-2021.3.21）

- 学习 IDAPython 插件的编写，完成Analysis以及输出部分，更加便于使用。
  
## 第45-46周（2021.3.22-2021.4.4）

- 了解整数溢出相关的漏洞模式
- 阅读代码审计、程序结构的一些特点
- 面试腾讯玄武实验室 生态组 客户端安全
- 阅读了一部分 [WebServer](https://github.com/linyacool/WebServer) 的源码，学习其中的开发技巧
  
## 第47周-第48周（2021.4.5-2021.4.18）

> 接下来3-5周有点摸，主要是学校课业压力有点大，时间太碎片化了。。。
>
> 最主要还是完成学校 N 份报告 + 调研 + 视频作业 + 期中考试复习（都是时间吞噬者）。。。

- WebServer 源码又看了一部分，主要把 base 里面多线程相关的内容理解了一下
- 学习了一点点关于 V8 Concat 的漏洞，这两天得再调试一下。
- 简单了解了一下推特上那个 chrome 0.5 day 的漏洞，关于 v8 backend。
- 完成所有面试，腾讯玄武实验室-生态安全组-客户端安全，offer到手。
  
## 第49周（2021.4.19-2021.4.25）

- HNU算法设计课程复习 + 算法设计期中考试 + 课程报告（1/2）

- 学习某某软件（for android） 的 n day exploit 利用

  > 编译 v8 for debug 时只去除  abort  in `V8_Fatal` 会有坑，DCHECK没去除干净，会引发 SIGSEV  T_T。。。

- 试着对某些漏洞补丁，通过 Regress / POC 来构造 exp。

  > 不过大多数都构造不出来 T_T，在原地踏步。正所谓看**答案一看就懂，做题一做就废**。
  >
  > 时间还是太短，积累的还是不够。

- **技术提升遇到瓶颈**。与教练聊了一下，明确了自己当前存在的问题，对接下来学习的重点有了一点大概的方向。

  > 问题：**钻的不够深、探的不够广，时间不太充裕**，且学校课程/活动使可用时间进一步**碎片化**。

- 买了一本《计算机网络》，准备先开坑用 C++ 实现一个自己的 WebServer。只读别人的代码没有什么收获，还是边写边学效果会更好一点。

  > WebServer 这个任务将长期进行下去，打持久战 QwQ.

  - 目前 WebServer 实现了**互斥锁**和**条件变量**的封装
  - 实现了简单的 **线程池机制**

## 第50周（2021.4.26-2021.5.2）

> 这周干的比较杂，主要是因为时间太碎片化了。

- WebServer 部分
  - 主要完成了基本多线程进行监听套接字的分发，以及完成 read/write 函数的包装，使其支持部分异常处理。
  - 阅读Linux manual 学习各类 socket 函数的用法,将上述内容使用 md 记录了笔记.

- 研究了一下 WSL 如何支持 i386 架构程序的运行，并水了一篇[博客](https://kiprey.github.io/2021/04/i386_WSL64/)。
- 研究 JS 引擎 v8 的 IR 图各个符号的用途。
- 上课摸鱼水《计算机网络》，看了大概几十页。
- 帮一个学长写了个[脚本](week20-now/domainInfo.py)，用以判断特定域名是 A 类型还是CNAME类型，同时判断该域名是否已过期。

## 第51周（2021.5.3-2021.5.9）

- 计算机网络，学习了应用层一章，熟悉了HTTP报文格式，以及运输层 UDP 相关内容。
- 计算机网络 ~~抄了点~~ 写了点笔记。
- 基本完成 WebServer-1.0版本，已经可以对 HTTP 报文实现基础的处理和返回数据。剩余一些细节仍然需要琢磨一下。
- 传了一下 WebServer-1.0版本的笔记，这份笔记仍然需要随着代码精雕细琢一下。

> 期中考试周结束，接下来的时间或许可以多一点点。

## 第52周（2021.5.10-2021.5.16）

- 计算机网络，学习到第四章网络层 IPv4部分。前三章 ~~文摘~~ 笔记已上传至[blog](https://kiprey.github.io/2021/05/cnatda-1/)。

  > 上课摸鱼学计网，下课回去整笔记。计网笔记整理**相当相当**耗时间，可能是因为内容实在太多太细了。
  >
  > 计网目前已经看完了大约45%，**预计**一个月内可以粗略看完（即选择性忽略一些目前用不到的内容）并且整理完所有笔记，**预计**。XD
  >
  > Wireshark实验还没做，这个可以以后慢慢来。

- WebServer-1.0版本完结（[CommitID:6473f5 - github](https://github.com/Kiprey/WebServer/tree/6473f5d512097f235ab209b13b53e28d7946a0f6)）。1.0版本的 ~~技术文档~~ 笔记已上传至[blog](https://kiprey.github.io/2021/05/WebServer-1/).

- epoll 多并发

  - 把 epoll 模型以及三个相关函数细致的了解了一下，写了点笔记
  - 看了几个使用 epoll 的例子
  - WebServer中实现了 epoll 函数相关的封装，接下来是使用。
  
  > 争取在接下来**一周**内，完成 epoll 相关多并发的学习，整一个 **WebServer-1.1 & 笔记**出来，尽量结束掉**网络编程**该部分的学习，开始整新花样。

## 第53周（2021.5.17-2021.5.23）

这周一直在肝 WebServer-1.1。WebServer-1.1 在原先 1.0 版本的基础上大量重构了代码，相对于旧版本来说，新版本主要更新了以下内容：

- 替换并发方式，从**多线程并发** 更换为 **epoll 并发**
- HTTP报文处理添加 POST 和 HEAD 方式的处理
- 支持自定义 WebServer 的 www 目录路径
- 使用 timerfd API，对每个 HTTP/1.1 Keep-Alive 的 TCP 链接设置了超时时间，超时后若还没有请求，则强制关闭该连接。
- 支持 Post 请求使用 CGI 程序。其中CGI程序可以是 shell 脚本、python脚本、ELF可执行文件等等。
- 支持自定义 www 目录路径，不再限制为当前工作目录。
- 支持更多的 Http 错误报文。

时间不太够，因此笔记还没写，还有一些bug还没修，多线程多进程调试整的人都要秃了。。。。

WebServer-1.1到目前位置还没有彻底完成，但这个任务从下周开始先暂时挂起（因为要把大块的时间留给更有意义的事情），等到了时间较为碎片的期末周再回来继续完善。

## 第54周（2021.5.24-2021.5.30）

- 计算机网络 完成网络层的学习
- 合作开发 AST-Fuzz - dump & parse + 测试套件

## 第55周（2021.5.31-2021.6.6）

继续开发 AST-Fuzz - print & analyze + 测试套件

## 第56周（2021.6.7-2021.6.13）

AST-Fuzz - type system & analyze

## 第57周（2021.6.14-2021.6.20）

AST-Fuzz - 扩展类型系统

> 2021.6.19-2021.6.29 HNU 期末周，复习+考试

## 第58周（2021.6.21-2021.6.27）

- HNU 期末周， 复习 && 考试
- AST-Fuzz - 完善类型系统 + 测试套件 + 零碎 mutate
  
  > 最近写了 N 多 security bug，还是太菜了，裂开......

## 第59周（2021.6.28-2021.7.4）

- HNU期末周
- AST-Fuzz - AST
  - many many many bugs fixed
  - port to AFL
  - docs

## 第60周（2021.7.5-2021.7.11）

- HNU 前半段小学期（1/2）（最占时间）
- AST-Fuzz
  - docs
  - 模板代码生成
  - 梳理类型系统
  - 实现了一点其他分析

## 第61周（2021.7.12-2021.7.18）

- HNU 前半段小学期（2/2）（最占时间）
- AST-Fuzz
  - 继续修bug
  - 修语义
  - 写了很多模板
- 飞北京，准备被各位巨佬吊打

## 第62-63周（2021.7.19-2021.8.1）

- 腾讯玄武实验室-生态安全组-客户端安全 快乐实习
- AST-fuzz
  - 重构 shit mountain
  - 完善 builtin
  - ......

## 第64周（2021.8.2-2021.8.8）

- 还是快乐实习
- AST-fuzz v1.0 封版 50%

  > 整了2.5个月，大火花和巨轮都聊出来了，真不容易......

> 接下来趁着实习期间难得可以摸到 iMac，好好学一手 MacOS。

## 第65周（2021.8.9-2021.8.15）

- 快乐实习 plus
- AST-fuzz v1.0 封版100%

  > 起飞

## 第66-68周（2021.8.16-2021.9.5）

- 快乐实习 plus plus
- 分析 crash，报了两个fuzz出来品相比较 lj 的洞
- 实习结束，准备返校

## 第69周（2021.9.6-2021.9.12）

- HNU 夏季实训（3/4）
- 写了一些零碎的脚本
- 继续学习 fuzz，简单看了一下 IR Fuzz，同时正在学习 HITBSecConf 上的一个有趣 fuzz 议题
- 练练 pwn 题，太久没写题了，0解选手

## 第70周（2021.9.13-2021.9.19）

- HNU 夏季实训（4/4）

- 学习 Trapfuzz 的思路，理解其代码，并与教练一起搭建起一个 fuzz 环境，跑出一个 adobe 空指针漏洞。

- 修复 IR fuzz 中关于 Python 深拷贝浅拷贝的一个巨坑，该巨坑让我们丢失了一个 crash 样本（简直痛心）

  >Python 函数默认参数**千万别是可变对象**。

- 学习 TrapFuzz 中 Linux 部分相关的内容，包括 gdb 脚本以及 gdb patch 的方式。

  > gdb python 接口：https://sourceware.org/gdb/onlinedocs/gdb/Python.html#Python

- 对 IR fuzz 添加**构造畸形 unicode 字符串**功能

  > 这里有两种方式构造畸形 unicode 字符串，一种是直接在代码中放上已经构造好的畸形unicode，但这样会使得 IR fuzz 在处理 string 时容易产生错误。
  >
  > 再一种就是生成诸如 **'\xXX'** 这样的字符。这种字符将在 JS 代码层面展示的很好，而具体的字符串将在 adobe 解析所构造的 JS 代码时动态产生 unicode 字符。

- 理了理关于 CS 架构程序的 fuzz 思路。

## 第71周（2021.9.20-2021.9.26）

> 突然发现很久没有写博客了。以后除了研究比较大块的内容以外，其他的一点点笔记或思路就直接记录在 learn list 里。
>
> 不然一小点笔记就开一篇新博文感觉有点浪费（逃）

- **Favocado [论文](https://www.ndss-symposium.org/ndss-paper/favocado-fuzzing-the-binding-code-of-javascript-engines-using-semantically-correct-test-cases/)与[源代码](https://github.com/favocado/Favocado)学习**。

  整体上分为两部分，分别是

  - 生成语法和语义正确的测试用例
  - 减少fuzz时的输入空间

  对于第一部分，

  1. Favocado解析 API 信息。获取 binding code 的完整语义信息，包括但不限于方法参数与返回值类型、个数等等
  2. 之后在预定义的语句格式中随机选择 JS 语句格式，并利用当前的binding code 语义信息以及所维护的上下文类型信息等等进行填空，以尽量满足语法语义的正确性。

  对于第二部分：根据 API 的关联性，构建不同的 binding code 以及各类 native API 之间的关联性，并划分等价类。之后的变异就基于等价类来变异，这样可以降低无关类型的输入，大幅度降低输入空间。

  如果不想啃论文可以直接看看这个简约版 - [白泽带你读论文 | Favocado - 知乎](https://zhuanlan.zhihu.com/p/378952042)

- 简单瞄了几种 hook 技术，只是看了看没写代码

  > inline hook 有点有趣，想找个机会研究一下，可惜最近有点忙。
  
- 打算看看 fuzz 的 [结构感知](https://github.com/google/fuzzing/blob/master/docs/structure-aware-fuzzing.md)，试着写写 CTF 菜单题的 fuzz 模板  

## 第72周（2021.9.27-2021.10.3）

- 接上面，使用 protobuf 搭配 AFL++ (qemu mode & QASAN）做了个简易 CTF fuzz。

  因为只是抱着研究学习的目的来做它，所以实际上用起来可能会比较难用（笑）

- 报了一个 Debug 模式下才会触发的 SQLite UAF，被谷歌毙了（哭泣）。

- 简单读了读 [Coming : a Tool for Mining Change Pattern Instances from Git Commits](https://arxiv.org/pdf/1810.08532.pdf) 论文。这篇论文大体上介绍了一个从 Git 仓库历史提交信息中**获取指定代码模式信息**的工具。

  它可以：

  1. 遍历所有历史 commit 信息
  2. 分析相邻 commit  的细粒度 diff 更改
  3. 在这些 diff 更改中检测 change pattern instance
  4. 计算代码更改频率
  5. 将分析结果用 JSON 格式输出等等
  
  该论文所对应的项目代码在这里： [SpoonLabs/coming：A tool for mining commits from Git repositories and diffs to automatically extract code change pattern instances and features with ast analysis](https://github.com/SpoonLabs/coming)。

- 在课程上花费的时间有亿点点多，而且这周状态也不太好，自我检讨一下。

- Kernel pwn CTF 入门，配环境踩坑配了两天；同时也在阅读 *Linux Device Drivers* 这本书。

## 第73周（2021.10.4-2021.10.10）

- 接上面，Kernel Pwn CTF 简单入了个小门，写了点记录但还没写完，还差一点 ROP 利用，先不传了。
- 这周摸了，没怎么学技术，在写一堆红色材料，写不完了......

  > 生活就像是操作系统，总会有事情会抢占掉当前运行的进程。

- 之前报的一个 facebook OOB read vulnerability 准备发 bounty 了。
  虽然不多，但毕竟是第一笔 bug bounty，感觉相当不错。

## 第74周（2021.10.11-2021.10.17）

- 完善了剩下的 Kernel Pwn CTF 入门笔记 - [传送门](https://kiprey.github.io/2021/10/kernel_pwn_introduction/)

- 继续写红色材料......

- 简单练了几题算法题

- 阅读一个有趣的论文 [FUZZIFICATION: Anti-Fuzzing Techniques](https://www.usenix.org/system/files/sec19fall_jung_prepub.pdf)

  - 三方面来降低 fuzz 效率

    - speedbump:

      - 首先使用给定 testcase 来识别 cold path(正常执行很少或几乎不访问的路径)，并在 code path 中**插入 delay 语句**以较大幅度提高程序运行时间

        > 注：大部分情况下，普通用户几乎很少会进入 cold path，但 fuzz 就是为了探测 cold path 中的 bug，因此会经常进入。

      - 插入 delay 语句后与先前定义的执行开销进行对比。如果低于预定开销则继续注入 delay 语句，高于则减少注入的 delay 语句

      - 抗分析：为了防止被简单的 patch 掉，这里使用 CSmith 生成动态算术运算代码，而不是常规的 sleep。

        同时为了防止被 deadcode elimination 优化掉，这里还修改 CSmith 以生成**具有数据依赖和原始代码依赖**的代码，具体一点就是涉及到了**全局变量的修改**。

    - branchtrap:

      - 根据 ROP 思维实现的代码重用，在大量函数内部插入**输入敏感**的跳转，显著改变执行路径，诱导基于coverage的fuzz更多关注无 bug 路径（因为发现了**“新”**路径）
      - 在 cold path 里引入大量**确定性分支**，迅速占满 fuzz 的 coverage bitmap，使得 fuzz 大量产生 hash 冲突，影响或减缓发现新路径的过程。

    - antihybrid：

      - 使用特定模板引入**隐式数据流依赖**，提高数据流污点分析的开销与难度

        例如简单的 int 赋值操作，硬是要拿个循环跑。

      - 插入大量假符号以触发符号执行中的路径爆炸

        例如将 if 条件判断中的简单判断语句，替换成**两个操作数进行 CRC 校验后的值的比较语句**，额外引入了 CRC 校验代码，即大量假符号

      - 缺点：容易被攻击者使用**代码模式检测方式**检测出来，因为模板是不变的

  - 这里有个别人整理的总结可以简单看看(比我这里写的详细不少) - [《fuzzification》论文阅读 - CSDN](https://blog.csdn.net/qq_40398985/article/details/103586567)

## 第75周（2021.10.18-2021.10.24）

- 准备校级评优材料

- 将 IR-Fuzz 融合进 AFL，同时也融合进 AST-fuzz 中以提高 fuzz 质量

- 最近新报的一个漏洞被 facebook 毙了，可惜。不过最早报的那个漏洞流程快走完了，快乐。

- 重启 WebServer，几乎修复所有已知错误，并完善了连接爆满的错误处理，完善了日志输出的方式。

  > 就差最后一个 bug 还没调通：一个多进程x多线程的条件竞争漏洞，怎么调也调不出来，有点难顶。

## 第76周（2021.10.25-2021.10.31）

- WebServer 最后一个 bug 终于调通了，**并非**条件竞争漏洞。

  > 珍爱生命，请对每个创建文件描述符的地方使用 O_CLOEXEC

  至此，WebServer 彻底结项。

- 阅读了 [MemFix: Static Analysis-Based Repair of Memory Deallocation Errors for C](http://prl.korea.ac.kr/~junhee/papers/FSE18.pdf) 论文，感觉有之前大二寒假实习中，学习流敏感指针分析的味道了......

  同时也尝试复现并跑通上面这篇论文里的所有测试与实验。

- 阅读论文 [Low-Tech Steganography for Covert Operations](https://www.researchgate.net/publication/330243139_Low-Tech_Steganography_for_Covert_Operations)，主要讲解了一个使用低级隐写技术（即无需任何高速计算机就可完成的隐写技术）来巧妙隐藏一些秘密文本。

  > 这个就不写笔记了，论文很简单，读起来非常快。

- 阅读论文 [AddressSanitizer: A Fast Address Sanity Checker](https://www.usenix.org/system/files/conference/atc12/atc12-final39.pdf) 论文，了解了早期 Asan 技术的相关设计方式。

## 第77周（2021.11.1-2021.11.7）

- 写了一个 fuzz crash 分类工具，思路是通过 ptrace 将 crash 时的栈帧 hash 成一条哈希值，相同哈希值的 crash 被分为同一类 crash（[ptrace version src](https://github.com/Kiprey/CrashUniquer)）

  > 该思路源于 trapfuzz。

- 阅读 Address Sanitizer LLVM 3.1 最早期的源代码，笔记上传至博客上。

- 将一个新的 IR-Fuzz 融合进 ast-fuzz，同时修复一些遗留bug。

- 进军 CS144 计算机网络实验，共Lab0- Lab7 八个实验，开始给自己充充电。

  本周已完成 Lab0、Lab1。

- 报了一堆不知道fb认不认的洞上去，坐等消息。

## 第78周（2021.11.8-2021.11.14）

- CS144 计网实验 Lab2 - Lab3

  > Lab4已经跑通全部测试样例，就差对真实网络TCP请求的调试。

- 漏洞被毙了，噩耗。不过又尝试开始新的挖洞方向。

## 第79周（2021.11.15-2021.11.21）

- HNU 期中考试周（1/2）

- CS144 计网实验 Lab4（TCP实现组装）、Lab5（网络接口实现）、Lab6 （IP路由实现）、Lab7

  > 至此，CS144 计网实验彻底结项。

- IR-Fuzz 启航

## 第80周（2021.11.22-2021.11.28）

- HNU 期中考试周（2/2）
- syzkaller 入门使用
- 阅读论文 [SHARD: Fine-Grained Kernel Specialization with Context-Aware Hardening](https://www.cs.purdue.edu/homes/pfonseca/papers/sec21-shard.pdf)

- 重新实现了一个 Crash 分类工具，基于 gdb 和 trap-fuzz 原理 - [CrashUniquer](https://github.com/Kiprey/CrashUniquer)。
- 修补 fuzz bug，调试语义

> 最近期中周，课程作业有亿点点多....

- 整了一下 protobuf + libfuzzer
  - 发现了AFL++的一个 bug
  - 对 libprotobuf-mutator 项目完全研究了一下，了解了其具体变异实现
  - 重新对先前的研究进行改进

## 第81周（2021.11.29-2021.12.5）

- 阅读论文 [HEALER: Relation Learning Guided Kernel Fuzzing](http://www.wingtecher.com/themes/WingTecherResearch/assets/papers/healer-sosp21.pdf)
- 阅读论文 [VScape: Assessing and Escaping Virtual Call Protections](https://www.usenix.org/conference/usenixsecurity21/presentation/chen-kaixiang)
- 阅读论文 [CollAFL: Path Sensitive Fuzzing](https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=8418631)
- 阅读论文 [Counterfeit Object-oriented Programming](https://www.syssec.ruhr-uni-bochum.de/media/emma/veroeffentlichungen/2015/03/28/COOP-Oakland15.pdf)
- 阅读论文[SoFi: Reflection-Augmented Fuzzing for JavaScript Engines](https://dl.acm.org/doi/10.1145/3460120.3484823)

- 简单看了看 [上下文敏感的 AFL++ 插桩技术](https://github.com/AFLplusplus/AFLplusplus/blob/stable/instrumentation/README.ctx.md)
- 粗略阅读 Linux-2.6.24 源码中关于共享内存、信号量的实现

## 第82周（2021.12.6-2021.12.12）

- 阅读论文 `VulDeeLocator: A Deep Learning-based Fine-grained Vulnerability Detector`
- 刷了六题 reversing.kr
- 各类课程实验、大作业
- 加入老师的课题组，参与寒假实习

## 第83周（2021.12.13-2021.12.19）

- 在 Windows VMware 上配置了一个 MacOS

  > 真不容易......

- 去深圳打 CCF CCSP 国赛，陪跑。

  > 可惜了......
  
- 准备两项考试

## 第84周（2021.12.20-2021.12.26）

- 密码学课程设计
  - 使用 OpenSSL 实现 DH 协议认证 + 消息完整性检测 + 来源验证
  - 使用 OpenSSL 创建公钥私钥以及证书，实现了一个简易 SSL 层 Echo Server 交互
  
  > 密码学真有意思。
  
- 学习 MacOS 的 Mach IPC

  - 阅读 *OS Internal Vol II 中的 IPC 机制
  - 尝试编写代码，深入理解 Mach API。

- 刷 pwn college

- 做了其他细碎的事情

## 第85周（2021.12.27-2022.1.2）

- 继续阅读网上的博客和 *OS internal 学习 mach IPC。

- 课程设计/课程报告

- 刷了点 pwn college ，学到 ROP 了

  > 带 JIT 的 yan85 可真有趣。

- 元旦快乐！

## 第86周（2022.1.3-2022.1.9）

- HNU 期末考试周（1/2）
- 简单入门了一下 MacOS XPC，看了点 MacOS Sandbox 基础知识
- 完成 35c3ctf 中 pillow 题的学习，这一题是 MacOS IPC 相关的一道沙箱逃逸题目
- 简单了解一下 xpcspy，主要基于 frida hook 来获取目标进程的 XPC 消息（突然想起之前的腾讯面试题）

## 第87周（2022.1.10-2022.1.16）

- HNU 期末考试周（2/2）
- 准备开始寒假实习科研，研究 macOS 内核漏洞挖掘
  - 阅读论文 《IMF: Inferred Model-based Fuzzer》
  - 阅读论文《iDEA: Static Analysis on the Security of Apple Kernel Drivers》，学习了一些关于 IOKit 相关的结构与用法
  - 阅读论文《SyzGen: Automated Generation of Syscall Specification of
    Closed-Source macOS Drivers》
- pwn college 继续

## 第88周（2022.1.17-2022.1.23）

- 复现 IMF 中的实验。受限于 MacOS 版本，复现实验时遇到了一些版本上的问题。同时完整梳理了一下 IMF 的内容，写了写笔记。还顺便给 IMF 修了个 Bug。
- 做了一些 pwn college，快整完了
- 玩玩 rwctf，只会最菜的 baby 题。等着赛后复盘。

## 第89周（2022.1.24-2022.1.30）

- 复盘 RWCTF 中的 Who Move My Block 以及 QLaas。学习使用 Codeql 对 nbd 进行审计。

- IMF 实验抽空结束掉了，准备复现 SyzGen 的实验，又安装了一个 MacOS 10.15 的虚拟机。

  > 1T 固态就这么快被各个虚拟机给吃完了......

- 完成 RWCTF 中的 FLAG 题的复盘。

## 第90周（2022.1.31-2022.2.6）

- 新年快乐！
- 复盘 RWCTF 中的 hso 题，这题和 pj0 那个 iMessage 0-click RCE 有着高相关性（而且有亿点点难）。
- 摸了，参加一些聚会和酒席

## 第91周（2022.2.7-2022.2.13）

- 完成 RWCTF hso 题的笔记编写

- 开始做 Syzgen 实验的复现。复现到一半发现 Windows 机器下 VMware MacOS 不能网络调试另一台 VMWare MacOS，真是太折腾了......

  找学姐借了一台 macbook pro 远程 teamviewer 控制来做实验......

## 第92周（2022.2.14-2022.2.20）

- 协助做 Fuzzing 论文的整理

- 完成 SyzGen 实验的复现

  - 基本跑通了 SyzGen的代码
  - 因为 xcode 编译出的驱动不能在 VM 上跑折腾了好久......
  - 完成 SyzGen 复现论文的文档编写

周报实在水不下去了，这里简单记录一下 `xcode 编译出的驱动不能在 VM 上跑` 这个的~~踩坑~~解决过程：

- 初始时，kextload 时提示 `kext start fail(result: 0x5)`，查看 log 时发现在 kextutil 报错前有些语句：
  
    ```log
    2022-02-18 06:35:53.512950-0800 0x250 Default 0x0 0 0 kernel.development: (47E46FA4-9B3F-38FA-9600-4F71D76491E3) <compose failure [UUID]>)
  ```
  
  除此之外没有 hook_start（自定义 kext 的名称为 `hook`，其 start 函数为 hook_start） 函数中 print 出的 `[hook_start] start kext` 这种信息，因此**初步认为 kext 在执行 start 前就挂了**。
  
- 一系列踩坑暂且不表，包括但不限于重新装了一台 10.15.4 版本的 MacOS 等等。
  
- 后来 lldb 直接调试 XNU 中的 `OSKext::load` 方法，发现其实 hook_start 已经执行了，printf 函数也跑了，但是 log 就是没有正常的输出，包括 dmesg 里也没有信息，这就奇了怪了。
  
- 之后我在 kext start 里增加了个循环，循环调用 printf 50次，之后再跑一次。这下才知道，原来之前说的那个报错 `<compose failure [UUID]>` 就是对应于输出的日志，只是可能因为其他缘故所以不能正常输出;
  
  然后在 dmesg 里也能看到输入的日志了，这应该是因为日志相关的缓存机制吧。
  
    > 太折腾了......

调试时还看到 angr 的有趣之处：给 angr 加装个 lldb proxy，这样 angr 就可以通过这个 lldb proxy 访问 kernel 中的任何内存数据，等价于把整个 kernel 做个 memory snapshot 再打包给 angr 做符号执行。这个设计非常的有意思。

## 第93周（2022.2.21-2022.2.27）

- 简单看了下 unicorefuzz。

- 仔细研究了一下 e9patch 的论文，了解其内部机理，顺便写了下笔记留待以后分享。

- 项目需求，机器学习入门。

- 读了一下 HFL 和 MoonShine 的论文，了解了一下它们在 kernel fuzz 中是如何解决某一种问题的方案。

- Codegate CTF 摸了会，对着题目学习如何编写 syzkaller template

  有道题 `forgotten` 很有意思：

  >kernel driver 为当前进程创建 vma 时，往 `vma->vm_private_data` 里塞了一个指向内核对象 entry 的指针。
  >当进程 fork 一份时，新进程也会完整复制这个 vma，使得**有两个进程持有了指向 entry 的指针**。
  >随后当新进程死亡时，entry 对象被释放。但是**另一个进程仍然持有指向 entry 的指针**，造成 kernel uaf。

## 第94周（2022.2.28-2022.3.6）

- syzkaller 源码阅读。主要关注 syzkaller 如何解析 syzlang，以及其变异策略（一步一步来嘛）

- 阅读论文 `NTFUZZ: Enabling Type-Aware Kernel Fuzzing on Windows with Static Binary Analysis`

  这篇论文提出了一种方法：从那些 documented 的 API 函数，通过静态分析技术一步步往下推断出 undocumented 的 syscall API 参数类型，并对其进行 fuzz。

  里面涉及的一些关于静态分析的东西还是有点模糊，不太能看懂。

- 阅读论文 `Scalable Fuzzing of Program Binaries with E9AFL`。

  e9afl 是一个可对无符号二进制程序插桩实现覆盖率反馈的工具，插桩后的程序可以直接用于 AFL 中进行 fuzz。相对于其他针对纯二进制文件进行 fuzz 的方法，它的优势在于插桩后的 overhead 还能保证在较低水平，同时还保证较高的精度。

- 修复了一下 github page 无法更新的缘故，原来是自己上传的 md 中 yaml 格式出现了问题，导致 github 部署时解析错误。

  这就使得我的博客处于薛定谔的状态，更了，但没完全更.....

- 修复了先前复现 SyzGen 实验时没完全跑起来的覆盖率检测，被提供的文档给坑了。

- Facebook CVE++

- 尝试通过 SyzGen 测试一些驱动，看看带有 breakpoint coverage 和不带有时的 fuzz，其效率相差的如何。

  > 测试的时候跑出了某驱动的一个空指针漏洞。（没想到还真能在复现时跑出漏洞.....）
  
  效率相差大概是将近 10x 左右，而且随着覆盖率的加大，带有 breakpoint coverage 的 syzkaller 执行速度会越来越慢。
  
  > 10x 算低的了，这还是因为 trace 的是单个驱动的覆盖率。
  
- 整理了一下周报，将一些较为大块的笔记挪到博客上了，使得周报更简洁一点。

## 第95周（2022.3.7-2022.3.13）

- 求助了学长，完成 NTFuzz 论文的阅读，理解了其中静态分析的大部分内容。

- 简单看了看 IOKit UserClient 接口逆向，为下周的逆向工作做准备。

- 继续 syzkaller 源码阅读，编写笔记梳理整个逻辑流程。

  > 目前已经完成 syz-extract、syz-sysgen 的源码笔记编写。
  >
  > syz-manager、 syz-fuzzer 以及 syz-executor 由于内容较多，联系紧密，因此其笔记编写不能在短时间内完成，只能慢慢利用碎片时间来磨。

- 看了点 afl-net，学习一下它的基本用法与实现原理

## 第96周（2022.3.14-2022.3.20）

- AFL + AFLNet 培训
- 开始辅助逆向一些闭源 IOKit Driver。目前初期只是边逆向边看开源 driver 代码，学习驱动的代码模式。
- 课程实验

## 第97周（2022.3.21-2022.3.27）

- IOKit 逆向，积攒了一些逆向 IOKit Driver 的经验
- 看了点 syzkaller 代码，慢慢磨吧。然后还简单的做了一个 syzkaller 源码导读分享
- 完善先前的 ast-fuzz。尝试从已有的 jackalope 项目中提取出一个独立的 AFL-fuzz，用来持续为 AST-fuzz 提供语料。
- HNU 封校了，其余杂事只会更多......

## 第98周（2022.3.28-2022.4.3）

- 修复 afl-jackalope 里的内存泄露，和 Jackalope 作者简单交流了一下。
- 逆向开源 IOKit driver 接口，尝试用 syzgen 干跑来复现这些接口。
- 尝试复现 linux dirty pipe 漏洞。
- 课程实验、信安国赛暂且不表（最花时间的地方）

## 第99周（2022.4.4-2022.4.10）

- 逆向 IOKit 驱动。开源 IOKit Driver 接口中有大部分都是无法在 Mac m1 上调用，只有 IOHIDFamily 中的几个接口可以调。
- 这周主要在刷算法题，为保研机试做准备。
- 课程...

## 第100周（2022.4.11-2022.4.17）

- 做了一篇 e9patch + e9AFL 的论文分享

- 尝试进军 stanford cs346 课程，学习数据库管理系统的实现（1/4）

  > cs346 貌似已经在 2015 年（将近7年前）便停止了授课，但是这个代码还是值得好好写写的。

- 复习期中考试，接下来这几周为期中考试周。

## 第101周（2022.4.18-2022.4.24）

- 刷刷算法题

- 期中考试（1/2）

- 把信安国赛的项目整个逻辑都理清楚了，发现思路越来越接近参考的论文了....

  写了一份报名书，这就是文书工作了。
  
- 用 syzgen 跑跑开源驱动，不过由于开源驱动数量过于少，因此跑出来的效果不太好评估。

## 第102周（2022.4.25-2022.5.1）

- 期中考试（2/2） + 课程实验等
- 尝试研究如何解决在 MacOS panic 时获取 panic log，简单探讨了一下，有几种方式可以试试

  1. patch PESavePanicInfo 函数，将该函数劫持至自编写 DIY 驱动函数，这样在 panic 时就能自动执行自己的驱动函数，获取到 Panic log
  2. MacOS crash 掉后，会自动将 panic log 存放在 NVRAM，或许可以通过串口把 NVRAM 中的数据读出来，然后通过树莓派转发至 syzkaller
  3. 起 debugger，当 macos panic 掉时让 debugger 捕获 panic loc，缺点是每次重启 macos 时都需要重新 attach debugger，比较麻烦
  4. 查阅 《*OS Internal Volume I》，发现或许可以设置一下启动参数，使得 macos 在 panic 时自动发送 panic log 给指定的静态 IP。这个操作可在 Intel mac 上成功实现，但是在 M1 上不太行，可能还得再调试。
- 读了一下 [Modern C++ Tutorial](https://changkun.de/modern-cpp/zh-cn/00-preface/)。太久没有学新东西会让我感到一点焦虑，原地踏步的样子可不好受 :(

> 接下来这五个月共22周会开始准备保研相关的事宜，因此周报里有趣的内容就有点少了。再加上课程与课设会进一步挤占自己的空闲时间，很多有意思的代码都可能得往后挪挪。

## 第103周（2022.5.2-2022.5.8)

- 这周主要在忙计算机作品赛的材料。还有 N 个课程设计需要完成。还给作品用 vue 改了个前端（别说还挺好看）

## 第104周（2022.5.9-2022.5.15）

- 完成一堆课设。
- 开始写论文，主要负责 Related work（捂脸），这周糊出了一个 related work 初稿。
- 用 SyzGen 跑了一些对比试验，用于论文做对比实验用。

## 第105周（2022.5.16-2022.5.22）

- related work 加了很多东西进去。想写好 related work 也不是一件容易的事情......

- SyzGen 努力挣扎了一下，还是没解决收集不到 trace 的问题，不知道是不是因为收集环境的问题。

- 花了点时间解决掉几个课设：

  - 写一个 Linux 驱动来收集 execve 的 log，增强审计功能。其核心思路是使用 kprobes hook 掉 sys_execve，然后收集当前用户权限等上下文信息，打包做成一个 log 信息发给守护进程中转存到磁盘里。

  - 写了一个数据库系统的前端，使用 vue 框架

  - 写了 SQL 注入攻击课设的网页前端，也是用 vue 完成。

    > vue 真有意思。

## 第106周（2022.5.23-2022.5.29）

- 别说了，还是课设

  > 这学期课设总感觉是最多最难的。

- SyzGen 跑了一些对比实验。

  > tmux 真好用，解决了 SSH 断开后进程终止的烦恼。

- 刷了一些算法题

- 计算机设计大赛省一到手，上推国赛了

  > 没想到这还真能拿奖.....

## 第107周（2022.5.30-2022.6.5）

- 简单记录一下跑论文实验时遇到的坑点......SyzGen 需要换一个 VM 版本来跑对比试验，因为 10.15.7 版本中的 kext 并非 fat binary，但是论文工具是针对 arm 的，故最终换了一个 11.5.2 版本的 VM，重新开跑试验。

  - 坑1：SyzGen 只支持 intel kext，不支持 fat binary。

    解决方法：使用 `lipo xxx -thin x86_64 -output xxx_64` 将 intel 架构的 kext 解压出来。
  
    有意思的是，解压出来的 intel kext 竟然在内核中的布局和二进制一致。

  - 坑2：通过调试 11.5.2 MacOS 发现 kextstat 中**显示**的驱动所占空间大小，竟然会小于实际装载的驱动大小。
  
    解决方法：可以通过 `kmutil inspect --show-fileset-entries` 来查看驱动各个段的装载位置，把判断 kextstat length 那块代码 fix 掉就可以继续工作。
  
  - 坑3：SyzGen 在分析 11.5.2 MacOS 上的 UserClient function table，分析出来的模板结构竟然大批量倒退，换句话说 infer_type 后的模板结构还没有 default 的模板好。
  
    解决方法：这个因为时间关系还真解决不了（捂脸），只能通过肉眼搭配 diaphora 来人工比对 10.15.7 与 11.5.2 之间 kext 的变动，之后选用那些在 10.15.7 上分析生成且接口与 11.5.2 变动不大的模板来做 fuzz。
  
    > 肉眼 bindiff 真的很累，而且眼睛都花了。
  
- 参加 HNU 学代会

- 尝试给论文补一点 background。

> 该周报大概每隔半个月 push 一次至 github 上。

## 第108周（2022.6.6-2022.6.12）

- 再给论文补了一些 Background，同时辅助修改论文，直至论文投稿（6.8）
- 准备 N 多夏令营材料
- 复习准备期末考试（1/2）
- 把 Redbase 捡起来些，简单写了些文档。

## 第109周（2022.6.13-2022.6.19）

- 期末考试（2/2）
- 开始狂填夏令营材料
- 刷 n 多算法题（对着洛谷的[题单广场](https://www.luogu.com.cn/training/list)刷，冲）

## 第110周（2022.6.20-2022.6.26）

- 小学期开始（1/2），开始有课程作业要做了......

- 还在刷洛谷的基础题单（还是学了些有趣的算法 + 数据结构）。

- 正在用 panda-re 搭建另一个项目的 linux 环境。

- 接上条，搭建环境过程中发现有个 `progress` 命令很有趣，可以自动查找当前系统中运行的 coreutils 命令（用户可手动指定其他命令）并监测其进度 - [github](https://github.com/Xfennec/progress)，代码量共 1.2 k。

  > coreutils 命令包含 cp、mv、dd、tar、gzip/gunzip、cat 等。

  显示的内容包含 pid、进程名称、当前操作文件、当前处理到的进度、总进度大小、速度和剩余时间等等。

  花了点时间读了一下源码，简单了解一下内部原理：

  1. 通过遍历`/proc` 来获取到目标进程的 pid，并遍历 `/proc/<pid>/fd` 保存目标进程所打开的最多 512 个文件描述符。保存的这些文件描述符必须链接至其他文件（readlink），否则将会在遍历时被忽略掉，例如像这样：

     ```bash
     $ sudo ls -al /proc/64/fd
     lrwx------ 1 kiprey kiprey 0 6月  26 10:50 0 -> /dev/tty2
     lrwx------ 1 kiprey kiprey 0 6月  26 10:50 1 -> /dev/tty2
     lr-x------ 1 kiprey kiprey 0 6月  26 10:49 12 -> /usr/share/zsh/functions/Completion.zwc
     lr-x------ 1 kiprey kiprey 0 6月  26 10:49 14 -> /usr/share/zsh/functions/Completion/Base.zwc
     ```

  2. 接下来，遍历 `/proc/<pid>/fdinfo/<fd>` 来获取这些所保存文件描述符的 fdinfo。

     fdinfo 中主要有几种信息，这里只感兴趣 `pos` 和 `flags`，分别是文件指针偏移量以及当前文件打开权限。

     其中 flags 用于参数筛选，除此之外没有任何用处。

     ```bash
     $ cat /proc/221371/fdinfo/10
     pos:    0
     flags:  02104002
     mnt_id: 1955
     tty-index:      4
     ```

     在遍历完这些 fdinfo 后，每个目标进程都只保存下 **size 最大**的那个 `fd`。

     这里的 size，指的是当前所遍历到 fd 的目标文件大小（通过 ioctl BLKGETSIZE64 获取）。

  3. 等待一秒钟，之后再循环遍历一次 fdinfo。之后可以根据**两次遍历相同 fd 的 fdinfo**，其 **pos 量的增加大小和两次 fdinfo 的时间戳间隔**，来计算出大致吞吐量。

  过程大致如上所示，简单总结一下关键的几个点：

  1. pid 和 进程名，均可通过 `/proc` 获取

  2. 将进程中 size 最大的 fd 视为当前操作的文件，并根据 readlink 获取到具体的文件名。

  3. 总大小根据当前所操作文件的 size 来决定；

     当前进度根据 fdinfo 中的 pos 来获取；
  
     吞吐量根据间隔时间 pos 量的改变来计算出。

## 第111周（2022.6.27-2022.7.3）

- HNU 小学期（2/2）

- 夏令营投递陆陆续续过了几个院校初筛，但也收到了拒信。下周开始就是夏令营高峰期了，这两周在准备面试和专业课。今年的保研形式相对于之前几年会更加严峻，也不知自己能去成哪所院校读研，只能尽人事听天命。

- 配置 windows fuzzer 环境，研究了一下 Intel-PT & Intel EPT，把 Intel-PT 特性部署在 VM 中。

  Intel-PT 是 Intel CPU 的一种硬件特性，如果希望部署的 VM 可以使用 Intel-PT 的话，当前主机的 **Intel CPU 必须是 Ice Lake 微架构**（可以用 cpuz / cpuid 来查看），这种架构通常只在 Intel 至强系列上才有。

  若需要在 KVM 中启用 Intel-PT，则除了 CPU 硬件条件满足上述的 IceLake 以外，还需要做一些设置：

  ```bash
  rmmod kvm-intel
  modprobe kvm-intel pt_mode=1
  ```

  之后启 KVM 时：

  ```bash
  qemu-system-x86_64 --enable-kvm -cpu host ...
  ```

## 第112-113周（2022.7.4-2022.7.17）

两周夏令营高峰期的面试，一个字，**寄**。

只拿到了**中科大**网安和**人大**信院的**学硕** offer，南大计算机学硕和北大软微专硕的 waiting list。

顺手写了篇 1w+ 字的经验贴，等 9 月 28 日保研填报系统彻底结束后再贴到博客上。

预推免再战，球球了想上岸 （捂脸）

## 第114周（2022.7.18-2022.7.24）

- 尝试修复 SyzGen 在 MacOS 11.5.2 上跑的 bug

  答：解决不了，原因是 angr 在进行符号执行时，无法触发先前设置在 mem_read 上的断点，因此不会调用回调函数来记录读取的位置，没法为后续的分析做准备。

  已经在发 issue 问 SyzGen 了，希望能有点收获。

- 刷刷洛谷为预推免做准备。

## 第 115 周（2022.7.25-2022.8.7）

- 尝试一周速通 Rust

  > 一个有趣的问题：学语言时在自己*不需要*的时候学好呢，还是在*需要*的时候学更好？这是先前留在 TODO 里的问题。
  >
  > 本人的做法：需要的时候学。因为确实要用上了（捂脸）。

  速通 rust，看这些就够了：[《Rust 程序设计语言》](https://kaisery.github.io/trpl-zh-cn) + [Rust语言圣经(Rust Course)](https://course.rs/about-book.html)

  > 发现 Rust 有着我非常看重的几点特性：
  >
  > 1. 绝对的速度。其运行速度和 C/C++ 有的一比。
  >
  > 2. 较高的安全性。Rust 设置了较多编译时的检查机制，若存在不安全操作，则将在编译阶段报错。
  >
  > 3. 静态语言。各个变量的类型在编写时就确定好，这样可以防止执行到一半报错某某类型不匹配，同时代码补全也会和 C++ 一样好用。
  >
  >    这里点名 Python，每次执行到一半才告诉我某某类型不匹配 ...
  >
  > 4. 无需内存管理。这点与 C++ 相比简直省心不少，每次写 C++ 代码。代码规模一大，内存管理就容易出问题。
  >
  > 5. 执行时无需 rust 环境。和 C++ 一样，编译一次后可以直接在其他平台上运行，无需配置运行时环境（点名 Python 和 Java）。
  >
  > 6. 拥有丰富的库。例如网络请求库，正则表达式库，Windows 图形库等丰富的第三方库。
  >
  > 7. 高频迭代。一门好的语言就需要实时更新迭代。
  >
  > 8. 编译与运行无需处理各种问题。（这里点名 Java 的各种包依赖！）
  >
  > 其中不妨看得出，1、3、5 是 C++ 中所拥有的特性，4、6 是 Python / Java 等语言的特性。
  >
  > 不过 rust 学下去发现这语法确实有忆点点晦涩，得多写写代码才能理解。

- SyzGen 找到 bug 了。起因是 SyzGen 大量将一个特定内核地址硬编码进代码中，而这个内核地址在不同的 MacOS 版本上是不同的，因此需要全局替换修改。接下来跑了一些 SyzGen 的实验。

## 第 116 周（2022.8.8-2022.8.14）

- 实习需求：用 Rust 在仿照 syz-manager 做一个 win fuzzer manager。目前已经完成了 VM 的管理与 Fuzzer 的启动等基本逻辑，接下来还要再实现一下 Manager 和 Fuzzer 的 RPC 通信，用来传输 stats 和同步语料。
- 生日快乐！
- Defcon Final 划水（真就划水，还是感觉自身实力不足，0贡献......）

## 第 117 周（2022.8.15-2022.8.21）

- win fuzzer manager 实现完 Crash 的捕获逻辑，准备调试与测试。在写这个的过程中也对 Rust 的使用有了更进一步的体会。整个 manager 的编写加上 Rust 语言的学习共计1个月。
- 准备预推免材料。
- SyzGen 再跑跑实验。

## 第118周（2022.8.22-2022.8.28）

- HNU 小学期 (1/2)

- 混了场 CTF Zone & MapleCTF（捂脸，还是技术不太够，没啥贡献）

- 趁着忙里偷闲，

  - 探究了 Thread 与 Canary 之间的关系 - [浅析 Linux 程序的 Canary 机制 - Kiprey's Blog](https://kiprey.github.io/2022/08/thread_canary/)。

  - 尝试学习 C++ 的 name demangling 规则，但是这方面的材料太少了，没找到什么有用的信息。

    demangling 规则不在 C++ 规范中，是由各个编译器自己决定实现的，通常使用的是 [itanium-ABI](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling)。这个 ABI 提供了一个用于 demangle 的函数：

    ```cpp
    namespace abi {
      extern "C" char* __cxa_demangle (const char* mangled_name,
               char* buf,
               size_t* n,
               int* status);
    }
    ```

    该函数的实现不在 glibc 中，而是在 `binutils`中内部的`libiberty\cp-demangle.c` 文件里，我们所熟悉的 `c++filt` 程序就是在 binutils 中实现。最核心的 demangle 函数为：

    ```cpp
    /* Internal implementation for the demangler.  If MANGLED is a g++ v3 ABI
       mangled name, return strings in repeated callback giving the demangled
       name.  OPTIONS is the usual libiberty demangler options.  On success,
       this returns 1.  On failure, returns 0.  */
    
    static int
    d_demangle_callback (const char *mangled, int options,
                         demangle_callbackref callback, void *opaque)
    ```

    该函数执行了 demangle 的完整过程，但 demangle 过程确实过于复杂，看不太懂，暂且搁置（捂脸）。

  - 复盘 Defcon 30 Quals 中的 `constricted` 题。该题需要 pwn 掉一个使用 rust 编写的 JS 引擎 - [Defcon-30-Quals rust-pwn constricted 复盘笔记 - Kiprey's Blog](https://kiprey.github.io/2022/08/defcon30quals_constricted/)。

> 之前在 TODO 里留下了一个疑问：学一门新的编程语言是**用到再学**，还是**先学再用**呢？根据我这段时间对 rust 的使用，我更偏向于**用到再学**，因为在动手实践中学习会把基础打得更加的牢固。

## 第119周（2022.8.29-2022.9.4）

- HNU 小学期（2/2）
- 复盘 Defcon 30 Quals 的  `smuggler's cove` ，这是一道 JIT Spray in Lua JIT 的题。
- have fun with Balsn CTF

## 第 120 周（2022.9.5-2022.9.11）

- 本周主要在刷刷算法题，提交了几个学校的报名信息，为预推免做准备。

- 本周收到了上交 G.O.S.S.I.P 安全研究组的实习邀请，开心！ :)

  只是目前本人已经在其他地方实习了，只能忍痛拒绝 T_T。
  
- win fuzzer 的 manager 实现彻底结束，合并进 master 分支。

- 通过了梦校的初筛，下周要开始准备复试了。

## 第121周（2022.9.12-2022.9.18）

准备梦校预推免。

- 机考寄！以前刷的各种题目各种洛谷完全没用，机考拼的真就是基础......

  这次预推免机考难度比夏令营大了一倍，那些用夏令营分数抵掉的同学简直太香了呜呜。

- 上岸！拿到梦校初步录取通知了，保研长跑接近尾声。

## 第122-123周 （2022.9.19-2022.10.2）

科研任务 + 保研结束。

去处：**清华大学网络科学与网络空间研究院，网络空间安全硕士**

给自己点个小赞。不过保完研后的生活将会非常忙碌了......

## 第124周（2022.10.3-2022.10.9）

- 研究了一手 Dirty Cred，写了篇[笔记](https://kiprey.github.io/2022/10/dirty-cred/)放博客上。（后面还得补一个 PPT......）

- 科研任务跑实验，跑出来了一个 win10 21h2 上的 crash（可惜不是我的）

- 启航 fuzzer


得想想后面要怎么安排了，接下来也要开始整毕设了，到时候问问导师看看。

## 第125周（2022.10.10-2022.10.16）

> 这周摸了

- 合作搞了一个  fuzz （1/3 进度）
- 在组内做了一次磕磕绊绊的论文分享（Dirty-Cred）
- 和清华那边的老师讨论了一下毕设方向，大概率是搞 iot 车机、车联网这块了。接下来的内容估计也是会围绕毕设展开。
- 考了个科一。

## 第126周（2022.10.17-2022.10.23)

- fuzz (2/3)

- 读了一下 libAFL 论文，学习了 libAFL 的整体架构与自定义使用。

  > libAFL 把整个 Fuzzer 各个部件全部打散，使得每一个部件都可以自定义，提供了一个通用模糊测试框架。（论文还上了 CCS2022 好像是，足以证明 AFLpp 团队的强大，膜拜一手）

- 探索目前的科研方向，打算先用 libAFL 实现一个轻量级框架玩玩。

## 第127周（2022.10.24-2022.10.30）

仔细想想这周好像也没干什么。

- 写了一些 fuzzer 代码，因为时间的问题只写完了一小个部件
- 跑出了几个洞顺便报上去了
- 学习 rust toy-rpc crate 的实现方式，打算将其并入当前准备实现的轻量级框架中

## 第128周（2022.10.31-2022.11.6）

- 继续写 Fuzzer 代码，本周点亮技能树：**node C++ plugin** hello world!
- 调研了一下 NIST 的 Vulnerability Data Ontology 标准草案，做了个PPT。
- 开摸！

## 第129周（2022.11.7-2022.11.13）

- 联系老师聊了一下关于毕设的内容
- 科目二
- 挖洞，赚了好多刀，再接再厉。
- 课设，需要在 Linux 2.6.18 版本上编写内核驱动。在创建调试环境时踩了一些坑。
  - busybox **不要**在本机编译 static binary，而是直接下载网上的预编译版本。因为在本机编译所使用的 glibc 库会和低版本 kernel 不兼容， 编译出来的 busybox 在 `__libc_start_main` 中就直接触发空指针崩溃了。
  - busybox 的 _install 文件夹下**一定要创建一个 init 可执行脚本**，没有该脚本则会提示 VFS mount failed 等错误。
- 用 rust 写的框架勉强算是完成了一个小功能点。

## 第130周（2022.11.14-2022.11.20）

本周摸了，主要在挖洞 + 写 vulhub 爬虫 + 党员材料 + 优秀班集体材料收集。

本周的挖洞收益比我预期的高了不少。

> 回顾了一下周报，感觉自己好像近几周没有干什么有意思的事情，得警惕温水煮青蛙。

## 第131周（2022.11.21-2022.11.27）

把fuzzing框架的一个功能点做完了，目前接下来是想看看能不能从这个功能点开始，对fuzzer内部的状态进行观测与修改。

本周突发40度高烧，休息了几天，各位在科研/工作/学习之余也要多注意身体。

## 第132周（2022.11.28-2022.12.4）

- 报洞，并且继续跟进完善 fuzz 模型
- 做做 vulhub 爬虫

## 第133-136周（2022.12.5-2023.1.1）

目前的生活已经趋于稳定，整体上精力一直在这三个内容来回流转：

1. 写 fuzz 挖洞
2. 做横向课题
3. 搞研究写代码

后续如果暂无有趣的内容，则先不再添加重复内容至周报上。水周报水的心虚（捂脸

> 12.26 补充：因为上面这些内容无论是哪一个都无法把更深的细节写在周报上（保密需求），因此这里的周报确实是不知道要写些什么了。
>
> 期待接下来的生活能有新变化，来让我继续在周报上添加有趣的内容。 :)

12.30 这周准备本科毕设开题，没想到目前做的这个工作大家看的比我想象的还更重一点，因此还需要我更加努力去完成这一切（挥拳）。

> 同时自己也在犹豫这个周报要怎么继续写下去。一方面我希望自己可以有个持续输出的地方，但另一方面有些重复性或保密性质的内容确实不太好写上去。所以后面可能采取跳周的方式来实现？

## 2022年年终总结

移步 [2022 年终总结 - Kiprey's Blog](https://kiprey.github.io/2023/01/2022-summary/)。

## 第137周（2023.1.2-2023.1.8）

本周是科研最后一周（不是说这周过了就不继续推进科研了，笑），本周过后不再开组会不交周报，感动。

> 一周三个组会真的有点顶不住.......

这周和学长准备了一下清华的学期总结，讲讲 windows 内核漏洞挖掘综述，纯靠学长带带了。

这周还和 r3 一起打了打 Realworld CTF，不过因为这两天刚好撞车学期总结没法集中精力（以及技术太菜了.....，捂脸）

后面在考虑要分配更多的时间到 CTF 上面打打。虽然自己不太想搞堆风水（没有精力去持续跟进 glibc 堆机制），但是很多题目都会设计一些非常有意思的考点，因此还是想把 CTF 这个短板补上，从其他地方去获取更多的正反馈。（顺带继续维护个人博客）

> 恨不得自己一天有 48h，笑。

## 第138周（2023.1.9-2023.1.15）

- 本周在 **ubuntu 22.04** 下的**中文环境**中偶遇 **wget(v1.20.3)** 触发 segment fault。通过对二进制文件进行逆向，发现是一个在*特殊语言环境*下会触发的整数溢出漏洞，**当下载时间超过 10min** 时即可在生成*终端下载用时文字*时稳定触发。可惜 ubuntu 配备的 wget 版本太老了，这个洞早被报了修了.....

  不过正是因为该漏洞需要特殊的语言环境，这个漏洞顽强存活了很长一段时间（2015.12-2019.12，漏洞提交者是中国人）。一个有趣的问题：开发者是否可以利用*不同语言环境下字符串长度的不同*这个特性，来提交可能带有漏洞的代码，并在未来某个时间点通过修改 *语言文件特定语句的翻译* 来达到*在特定地区启用漏洞*的效果呢？

  感兴趣的师傅也可以调试这个洞看看，还挺有意思。复现条件：

  1. **ubuntu 22.04 / wget v1.20.3**
  2. ubuntu 配置语言环境为**中文**
  3. 使用 wget 下载文件**用时超过 10min**（无任何 wget 参数要求，可自行使用 `--limit-rate` 来限制下载速度）

- 协助写了 short paper 的一部分内容。

- idek CTF，独立解出一道基于 C++20 协程机制的竞争 pwn 题（共4解）

  > C++20 这个协程机制确实有点意思，不过对于普通开发者而言还是有点难用，目前只适合让库开发者来使用。

## 第139周（2023.1.16-2023.1.22）

- 写 fuzz
- 抽空把上周那道协程 pwn 题的 writeup 写了，尝试去竞争那个 writeup 奖（可惜看样子应该是没抢到）
- 过年啦，祝大家新年快乐！

## 第140-142周（2023.1.23-2023.2.12）

年后这三周总体上围绕着过年各种亲戚拜访、酒席等年后活动，同时穿插着 fuzzer 的编写和科研方向的探索，以及现在准备回校。

fuzzer 这块走了个非常大的弯路，不过幸好最终还是回到正轨上了。

顺便研究了一手 ossfuzz 打造的 fuzz-introspector，它的作用机理是对 llvm-cov 所生成出来的 trace 进行分析，以便于安全人员推断 fuzz 瓶颈。
