# 虚拟机

虚拟机包含了寄存器以及相关的内存段，其主要功能就是对各种寄存器与内存段的操作，
以及执行指令。

各种指令的实现：

```C++
inst = *pc;
++pc;

>> IMM

立即数存在pc寄存器中
if(inst == IMM) {
    ax = *pc;
    ++pc;
}

>> LC

char型变量的地址在ax中
if(inst == LC) {
    ax = *(reinterpret_cast<char*>(ax));
}

>> LI

int型变量的地址在ax中
if(inst == LI) {
    ax = *(reinterpret_cast<int*>(ax));
}

>> SC

char型变量的地址在sp所指位置，其值在ax中
if(inst == SC) {
    char* addr = reinterpret_cast<char*>(*sp);
    ++sp;
    *addr = ax;
}

>> SI

int型变量的地址在sp所指位置，其值在ax中
if(inst == SI) {
    int* addr = reinterpret_cast<int*>(*sp);
    ++sp;
    *addr = ax;
}

>> PUSH

将ax中的值入栈
if(inst == PUSH) {
    *(--sp) = ax;
}

>> JUMP

对pc直接赋值
if(inst == JUMP) {
    pc = reinterpret_cast<int*>(*pc);
}

>> JZ

零则跳转
if(inst == JZ) {
    if(ax != 0) pc = pc+1;
    else pc = reinterpret_cast<int*>(*pc);
}

>> CALL

函数调用，先保存返回后将要执行的指令，再跳转
if(inst == CALL) {
    *(--sp) = reinterpret_cast<int>(pc+1);
    pc = reinterpret_cast<int*>(*pc);
}

>> ENT

进入函数后，执行函数前，创建函数调用帧
if(inst == ENT) {
    *(--sp) = reinterpret_cast<int>(bp);
    bp = sp;
    sp = sp - *pc;
    ++pc;
}

>> ADJ

函数返回时参数出栈
if(inst == ADJ) {
    sp = sp + *pc;
    ++pc;
}

>> LEV

函数返回，恢复函数调用帧
if(inst == LEV) {
    sp = bp;
    bp = reinterpret_cast<int*>(*sp);
    ++sp;
    pc = reinterpret_cast<int*>(*sp);
    ++sp;
}

>> LEA

获取基于bp寄存器的位置
if(inst == LEA) {
    ax = reinterpret_cast<int>(bp + *pc);
    ++pc;
}

>> <binary operator>

二元操作符，其第一个操作数在sp所指位置，第二个操作数在ax中。结果存放于ax中
if(inst == OR) {
    ax = (*sp | ax);
    ++sp;
}

if(inst == XOR) {
    ax = (*sp ^ ax);
    ++sp;
}

if(inst == AND) {
    ax = (*sp & ax);
    ++sp;
}

if(inst == EQ) {
    ax = (*sp == ax);
    ++sp;
}

if(inst == NE) {
    ax = (*sp != ax);
    ++sp;
}

if(inst == LT) {
    ax = (*sp < ax);
    ++sp;
}

if(inst == LE) {
    ax = (*sp <= ax);
    ++sp;
}

if(inst == GT) {
    ax = (*sp > ax);
    ++sp;
}

if(inst == GE) {
    ax = (*sp >= ax);
    ++sp;
}

if(inst == SHL) {
    ax = (*sp << ax);
    ++sp;
}

if(inst == SHR) {
    ax = (*sp >> ax);
    ++sp;
}

if(inst == ADD) {
    ax = (*sp + ax);
    ++sp;
}

if(inst == SUB) {
    ax = (*sp - ax);
    ++sp;
}

if(inst == MUL) {
    ax = (*sp * ax);
    ++sp;
}

if(inst == DIV) {
    ax = (*sp / ax);
    ++sp;
}

if(inst == MOD) {
    ax = (*sp % ax);
    ++sp;
}

>> PRTF

输出函数，参数放于栈上
if(inst == PRTF) {
    int* temp = sp + pc[1];
    ax = printf(reinterpret_cast<char*>(temp[-1]),
    temp[-2], temp[-3], temp[-4], temp[-5], temp[-6]);
}

>> EXIT

退出程序，其退出编号存放于栈顶
if(inst == EXIT) {
    printf("exit(%d)", *sp);
    return *sp;
}
```
