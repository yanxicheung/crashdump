# ELF

预处理器，编译器，汇编器，连接器

1. `ELF Header`描述了体系结构和操作系统等基本信息，并指出`Section Header Table`和`Program Header Table`在文件中的什么位置。

2. `Program Header Table`在加载过程中使用，在汇编和链接过程中没有用到，保存了所有`Segment`的描述信息。

3. `Section Header Table`在汇编和链接过程中使用，在加载过程中没有用到，保存了所有`Section`的描述信息。

4. `Section Header Table`和`Program Header Table`并不是一定要位于文件开头和结尾的，其位置由`ELF Header`指出。

目标文件需要链接器做进一步处理，所以一定有`Section Header Table`；可执行文件需要加载运行，所以一定有`Program Header Table`；而共享库既要加载运行，又要在加载时做动态链接，所以既有`Section Header Table`又有`Program Header Table`。



`readelf -h` 查看`ELF`文件头，32位系统对应`/usr/include/elf.h`中定义的结构体`Elf32_Ehdr`:

```shell
root@msp-OptiPlex-5040:/mnt/zpp/test_simple# readelf -hW SimpleSection.o
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              REL (Relocatable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          0 (bytes into file)
  Start of section headers:          1112 (bytes into file) #段表在ELF文件中的偏移位置0x458
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           64 (bytes)
  Number of section headers:         13
  Section header string table index: 10

```



`readelf -S` 查看段表(Section Header Table)结构，32位系统对应`/usr/include/elf.h`中定义的结构体`Elf32_Shdr`:

```shell
root@msp-OptiPlex-5040:/mnt/zpp/test_simple# readelf -SW SimpleSection.o
There are 13 section headers, starting at offset 0x458:

Section Headers:
  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            0000000000000000 000000 000000 00      0   0  0
  [ 1] .text             PROGBITS        0000000000000000 000040 00006b 00  AX  0   0  1
  [ 2] .rela.text        RELA            0000000000000000 000318 0000a8 18   I 11   1  8
  [ 3] .data             PROGBITS        0000000000000000 0000ac 000004 00  WA  0   0  4
  [ 4] .bss              NOBITS          0000000000000000 0000b0 000004 00  WA  0   0  4
  [ 5] .rodata           PROGBITS        0000000000000000 0000b0 000016 00   A  0   0  1
  [ 6] .comment          PROGBITS        0000000000000000 0000c6 000036 01  MS  0   0  1
  [ 7] .note.GNU-stack   PROGBITS        0000000000000000 0000fc 000000 00      0   0  1
  [ 8] .eh_frame         PROGBITS        0000000000000000 000100 000058 00   A  0   0  8
  [ 9] .rela.eh_frame    RELA            0000000000000000 0003c0 000030 18   I 11   8  8
  [10] .shstrtab         STRTAB          0000000000000000 0003f0 000061 00      0   0  1
  [11] .symtab           SYMTAB          0000000000000000 000158 000168 18     12  11  8
  [12] .strtab           STRTAB          0000000000000000 0002c0 000055 00      0   0  1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)
  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
  O (extra OS processing required) o (OS specific), p (processor specific)
```



`hexdump -C`





加载器



其他动态链接把`链接`这个过程从本来程序装载前推迟到了装载的时候，

在编译的时候，动态库不参与链接，但是在编译的时候，动态库也是链接的输入文件之一，为什么？

动态库中保存了完整的符号信息，动态库作为链接的输入文件，链接器就可以知道某个符号是动态库里面的动态符号，对该符号做特殊处理，是他成为对动态符号的引用。

```shell
root@msp-OptiPlex-5040:/mnt/zpp/my_git_repo/daydayup/Debug/Linux/example# ldd test
	linux-vdso.so.1 =>  (0x00007ffc96d83000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007ff9932fe000)
	/lib64/ld-linux-x86-64.so.2 (0x00007ff9936c8000)
```

`ld`是`Linux`下的动态链接器，动态连接器与普通共享对象一样被加载到了进程的地址空间，在系统运行可执行文件时，首先将控制权交给动态链接器，由它完成所有的动态链接工作后，再把控制权交给可执行文件，然后开始执行。

# 参考文献

1. [Linux SIGABRT和abort()函数](https://blog.csdn.net/rikeyone/article/details/89226508)
2. [Linux中/proc/pid/status信息含义](https://blog.csdn.net/ibless/article/details/85123386)
3. [软件调试栈回溯实验 ](https://www.freesion.com/article/58751124296/)
4. [unwind 栈回溯详解:libunwind](https://www.freesion.com/article/20551507313/)
5. [程序员的自我修养---链接、装载与库](https://book.douban.com/subject/3652388/)
6. [ELF文件详解 ---- 初步认识_技术探索者的博客-CSDN博客](https://blog.csdn.net/hhd1988/article/details/109375257)
7. [elf(5) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man5/elf.5.html)
8. [ELF文件格式 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/286088470)
9. [(1条消息) Linux ELF 详解2 -- Section Header & Section_懒惰的劳模的博客-CSDN博客](https://blog.csdn.net/helowken2/article/details/113757332)
10. [(1条消息) 可执行文件的装载与进程_zmrlinux的博客-CSDN博客_可执行文件的装载](https://blog.csdn.net/zmrlinux/article/details/47166385)
11. [(1条消息) linux 执行.out文件_Linux可执行文件与进程的虚拟地址空间_weixin_39807067的博客-CSDN博客](https://blog.csdn.net/weixin_39807067/article/details/111266063)
12. [(1条消息) ELF文件详解 ---- 初步认识_技术探索者的博客-CSDN博客](https://blog.csdn.net/hhd1988/article/details/109375257)
13. [ELF文件系统格式.pdf](./ELF文件系统格式.pdf)
14. [程序员的自我修养—链接、装载与库](https://book.douban.com/subject/3652388/)

