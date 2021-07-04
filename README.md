# 天问之路

> 这里将定期记录着一些与`Sakura`师傅以及一群小伙伴共同学习的内容与进度。

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
- 合作开发 AST-Fuzz
  - 学习 Fuzzilli 的部分思路
  - 完成 AST 转 AST Json 的 dump 操作。
  - 完成 parse 和 dump 功能的批量单元测试功能，并对该功能进行了大量数据集的测试。

## 第55周（2021.5.31-2021.6.6）

继续开发 AST-Fuzz

- 学习了点 DIE 的部分思路。
- 完成 print 的批量测试。
- 讨论
  - 了解fuzz种子的清洗方式
  - 了解fuzz种子的评估方式
  - 明确fuzz的整体分析与变异架构
  - 确定了接下来的开发方向与思路
- 初步完成 analyze 模块。

## 第56周（2021.6.7-2021.6.13）

AST-Fuzz

- 学习讨论类型系统 Type System 的实现
- 完成类型系统的设计，并在原先设计的基础之上，考虑套娃的情况，完成类型系统的闭环。
- 完成类型系统+类型分析的实现，通过大规模测试。
- 阅读DIE源码，寻求优秀项目的变异方式。

> 类型分析可真难写呀......

## 第57周（2021.6.14-2021.6.20）

AST-Fuzz

- 讨论类型系统变异方式
- 将JavaScript Builtin类型添加进类型系统中
- 扩展VarType，额外处理类型为 Object | Constructor 的情况。
- 重新测试并修复类型系统的bugs。

> 2021.6.19-2021.6.29 HNU 期末周，复习+考试

## 第58周（2021.6.21-2021.6.27）

- HNU 期末周， 复习 && 考试
- AST-Fuzz
  - 设置 VarType 获取 FunctionSignature 时，对于 Builtin 类型返回 nullptr
  - 完善 MethodDefinition 以及 getRandVar 接口
  - 完善 TypeSystem 的接口
  - 完善 codegen 中NewExpr, CallExpr以及 MemberExpr的generator
  - 完善 NewExpr, CallExpr以及 MemberExpr的 mutate
  - 编写 mutate 测试套件（多线程）
  - 修复若干 Nullptr Dereference bugs、 UAF bugs
  - 修复 CallExpr && NewExpr 获取 ArgType 时的越界 bug。

  > 最近写了 N 多 security bug，还是太菜了，裂开......

## 第59周（2021.6.28-2021.7.4）

- HNU期末周
- AST-Fuzz
  - 修复 mutate 套件中多线程的互斥锁bug，以及 python exit 函数的误用bug

    > 鬼知道 python 中的 exit 函数只会退出单个线程......整个程序退出用 `os._exit(1)`

  - 依旧是修复了大量 Null Pointer Dereference + Use After Free bugs
  - 依旧对mutate进行了大批量测试
  - 将 AST 套件与 AFL 组合
  - 对 AST 套件添加异常处理逻辑，而不再是原先的 assert，以提高debug效率。
  - 在AFL中编写语料正确率统计代码
  - 初步完善 Node 结点 Trim 过程
  - 完善 Readme 中 运行说明以及测试套件使用说明
  - 删除废弃的 AST 代码，并完善CMakelist至最新的代码路径
  - 编写类型系统的文档

## TODO List

- Top Priority
  - AST-Fuzz 开发
- High Priority
  - Web Server bug fix && document
  - CS144 Computer Network Lab + 《计算机网络》
- Medium Priority
  - Garbage Collection Study
  - Address Sanitizer LLVM 分析
