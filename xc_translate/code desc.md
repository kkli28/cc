
```
>> if (op==IMM) { ax=*pc++; }

IMM指令，取立即数。其操作数(NUM)放在其后方，既若*pc==IMM，则*(pc+1)==NUM

>> if (op==LC) { ax=*(char*)ax; }

LC指令，加载char。数的地址存放在ax中，并将取得的数存放于ax中。
因此先将ax转化为char*，再取其所指向的值，放入ax。

>> if (op==LI) { ax=*(int*)ax; }

LI指令， 加载int。

>> if (op==SC) { *(char*)*sp++=ax; }

SC指令，保存char。*sp中存放变量的地址，ax存放变量待保存的值。
先将*sp转化为char*，再对指向的内存赋值。sp++相当于退栈的操作。

>> if (op==SI) { *(int*)*sp++=ax; }

SI指令，保存int。

>> if (op==PUSH) { *--sp=ax; }

PUSH指令，ax的值入栈。因为栈是向低地址增长，且*sp为之前的栈顶，因此
需要先sp-1再赋值。

>> if (op==JMP) { pc=(int*)*pc; }

JMP指令，无条件跳转。pc所指的内存存放的是跳转的地址，因此设pc的值为
该内存单元的值。

>> if (op==JZ) { pc=ax? pc+1: (int*)*pc; }

JZ指令，0则跳转。当ax中的值为0时，则pc执行跳转，否则按序执行。

>> if (op==JNZ) { pc=ax? (int*)*pc: pc+1; }

JNZ指令，非0则跳转。当ax中的值非0，则跳转，否则按序执行。

>> if (op==CALL) { *--sp=(int)(pc+1); pc=(int*)*pc; }

CALL指令，函数调用。先将pc的值保存在栈上，再执行跳转。

>> if (op==ENT) { *--sp=(int)bp; bp=sp; sp=sp-*pc++; }

ENT指令，进入函数。先将bp入栈，bp再指向原栈顶，sp为函数调用中局部变量预留空间。

>> if (op==ADJ) { sp=sp+*pc++; }

ADJ指令，将函数调用参数出栈。由LEV进行函数调用返回，再由ADJ将调用参数出栈，可完全恢复调用现场。

>> if (op==LEV) { sp=bp; bp=(int*)*sp++; pc=(int*)*sp++; }

LEV指令，离开函数。从bp中恢复函数调用前的sp，再bp出栈，再pc出栈，
恢复调用现场。（函数调用前入栈的那些参数此时在栈顶，需要ADJ进行出栈）。

>> if (op==<OP>) { ax=*sp++ <OP> ax; }

<OP>代表各种操作符，他们都有两个操作数，第一个操作数在栈顶，
第二个操作数在ax寄存器中。所有操作都会将参数出栈，并且计算结果
保存在ax寄存器中。

>> if (op==EXIT) { printf("exit(%d)", *sp); return *sp; }

EXIT指令，调用内置函数exit()退出程序。其参数放在栈顶。

>> if (op==OPEN) { ax=open((char*)sp[1], sp[0]); }

OPEN指令，调用内置函数open()打开文件。文件描述符存于ax寄存器中。

>> if (op==CLOS) { ax=close(*sp); }

CLOS指令，调用内置函数close()关闭文件。文件描述符存于栈顶。

>> if (op==READ) { ax=read(sp[2], (char*)sp[1], *sp); }

READ指令，调用内置函数read()读取文件。读取字节数、读取文件、
文件描述符分别存放在栈顶、次栈顶、次次栈顶。

>> if (op==PRTF) { tmp=sp+pc[1]; 
>>     ax=printf((char*)tmp[-1], tmp[-2], tmp[-3],
>>     tmp[-4], tmp[-5], tmp[-6]); }

PRTF指令，调用内置函数printf()输出内容。最多支持6个参数。

>> if (op==MALC) { ax=(int)malloc(*sp); }

MALC指令，调用内置函数malloc()分配内存。参数在栈顶，内存指针在ax寄存器中。

>> if (op==MSET) { ax=(int)memset((char*)sp[2], sp[1], *sp); }

MSET指令，调用内置函数memset()设置内存的值。

>> if (op==MCMP) { ax=memcmp((char*)sp[2], (char*)sp[1], *sp); }

MCMP指令，调用内置函数memcmp()进行内存值比较。
```
