# 天问之路

> 这里将定期记录着一些与`Sakura`师傅以及一群小伙伴共同学习的内容与进度。

- 第1-2周，[STL](week1-2/)
  - 第1周（2020.5.18-2020.5.23）：学习基础的概念，例如traits之类的
  - 第2周（2020.5.23-2020.5.29）：数据结构抄到list
  
- 第3-6周，[编译原理](week3-6/)
  - 第3周（2020.5.29-2020.6.05）：词法分析与语法分析
  - 第4周（2020.6.05-2020.6.14）：逆向CS143提供的标准语义分析器
  - 第5周（2020.6.14-2020.6.21）：抄PA5目标代码生成，研究了一下flex和bison
  - 第6周（2020.6.21-2020.6.28）：整了一份Compiler总结，恶补Makefile、完成LLVM IR PASS assignment1
  
- 第7-8周，[LLVM IR Pass 代码优化](week7-8/)
  - 第7周（2020.6.28-2020.7.05）：完成LLVM IR Pass的所有三个Assignment，看了点GC的算法篇
  - 第8周（2020.7.05-2020.7.12）：大一暑期实训（1/2周）
  
- 第9周（2020.7.12-2020.7.19）：[AFL_LLVM_mode源码分析](https://kiprey.github.io/2020/07/AFL-LLVM-Mode/)

- 第10-11周，[CSAPP](week9-19/CSAPP-Lab/)
  - 第10周（2020.7.19-2020.7.26）：CSAPP LAB做到第五个Lab-[Cache Lab](https://kiprey.github.io/2020/07/csapp-lab-writeup/#5-Cache-Lab)
  - 第11周（2020.7.26-2020.8.02）：完成[CSAPP全部Lab](https://kiprey.github.io/2020/07/csapp-lab-writeup/)
  
- 第12-19周， [uCore Lab](week9-19/uCore) (第15-18周期末复习)
  - 第12周（2020.8.02-2020.8.09）：完成[Ucore-Lab1](https://kiprey.github.io/2020/08/uCore-1/)，正在研究Lab2
  - 第13周（2020.8.09-2020.8.16），完成uCore-[lab2](https://kiprey.github.io/2020/08/uCore-2/) [lab3](https://kiprey.github.io/2020/08/uCore-3/)
  - 第14周（2020.8.16-2020.8.23）：信安国赛 + 朗诵比赛准备。完成[Lab4](https://kiprey.github.io/2020/08/uCore-4/)和[Lab5](https://kiprey.github.io/2020/08/uCore-5/)（除了Challenge）。Lab5 Challenge的写时复制代码已经完成。
  - 第15-18周（2020.8.23-2020.9.20）：大一下期末复习 + 期末考
  - 第19周（2020.9.20-2020.9.27）：uCore完成[Lab6](https://kiprey.github.io/2020/09/uCore-6/)、[Lab7](https://kiprey.github.io/2020/09/uCore-7/)，uCore-Lab8视频刚开始看，笔记还没来得及做
  
- 第20周 - ~, Chrome / v8 学习
  
  > 从第20周开始，所有涉及的资料以及分析等等均在[个人博客](https://kiprey.github.io)上发布。
  
  - 第20周（2020.9.27-2020.10.04）：[uCore-Lab8](https://kiprey.github.io/2020/09/uCore-8/)终于结束，PlaidCTF2020 Mojo还在挣扎，看了点调试器的文章
  
  - 第21周（2020.10.04-2020.10.11）：[Mojo](https://kiprey.github.io/2020/10/mojo/)完成，CVE-2019-5826在看blackhat稿子
  
  - 第22周（2020.10.11-2020.10.18）：[CVE-2019-5826 POC分析](https://kiprey.github.io/2020/10/CVE-2019-5826/)完成， CVE-2020-6549 render UAF 才看一点点
  
  - 第23周（2020.10.19-2020.10.25）：完成 [CVE-2020-6549 render UAF的POC分析](https://kiprey.github.io/2020/10/CVE-2020-6549/)，完善当前study list，以及阅读了一下C++ case，了解chrome的一些基本结构与方法。
  
  - 第24周（2020.10.26-2020.11.01）：完成[CVE-2020-6541分析](https://kiprey.github.io/2020/10/CVE-2020-6541/)
    - 10.27：看了一点CVE-2020-6541的内容，尝试追溯其调用链
    - 10.28：该漏洞研究完成，笔记已上传
    - 10.29：下拉V8代码并编译（代理设置搞了半天才好）
    - 10.30：忙学校的事情
    - 10.31：忙评奖的事情
    - 11.01：打湖湘杯

  - 第25周（2020.11.02-2020.11.08）JSPromise  type confusion学习
    - 11.02：装了一下[issue1784](https://bugs.chromium.org/p/project-zero/issues/detail?id=1784)的调试环境
    - 11.03：阅读了一个下午的issue1784，发现装环境的方向错了，再装了一个chrome release
    - 11.04：仔细审计v8源码，终于理解JS的type confusion
    - 11.05：上课
    - 11.06：尝试找到blink-in-js调试方式，无果
    - 11.07：审计一天，遇到了相当多的问题，暂时无法解决。不过基本上已经了解大部分关于Promise的内容，达到了目的，可以开始下一步的学习。
    - 11.08：肝课程作业

  - 第26周（2020.11.09-2020.11.15）： CodeQL学习、寒假实习面试
    - 11.09：CodeQL配置环境
    - 11.10：大二寒假实习面试
    - 11.10：CodeQL重新配置环境
    - 11.11：完成Chromium下载链接爬虫
    - 11.12：阅读CodeQL脚本
    - 11.13：阅读CodeQL doc，了解其基本语法
    - 11.14-11.15：完成日常课程作业

    > 试用了一段时间，发现日报模式可能不太适合我，因此切回周报模式。

  - 第27周（2020.11.16-2020.11.22）：下拉chromium 24gb的源码、期中复习（1/2）
  
  - 第28周（2020.11.23-2020.11.29）：期中复习（2/2）
  
  - 第29周（2020.11.30-2020.12.6）：完成学校4份实验+6份报告
  
  - 第30周（2020.12.7-2020.12.13）：CodeQL完成所有基础语法的学习，编写了检测V8callback的ql来练手
  
  - 第31周（2020.12.14-2020.12.20）：CodeQL编写了DispatchEvent的QL，仍然存在一些问题；完成学校5份实验7份报告
  
  - 第32周（2020.12.21-2020.12.27）：看了几个github securityLab关于CodeQL的几个例子，写了篇博客；同时还完善了扫描DispatchEvent的QL代码
  
  - 第33周（2020.12.28-2021.1.3）：学习GoogleCTF2018(Final) Just-In-Time，刚配置好环境
  
  - 第34-36周（2021.1.4-2021.1.24）：期末复习+考试；完成V8 turboFan的学习。
  
  - （doing）第37-41周（2021.1.25-2021.2.28）：
  
    - 静态编译器研发实习（关于静态指针分析方向）
    - 完成CVE-2021-3156 sudo 提权漏洞分析。
    - 学习 V8 Gabage Collection
    - 学习 V8 历史漏洞
