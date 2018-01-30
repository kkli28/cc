
//next() 用于词法分析，获取下一个标记，它将自动忽略空白字符
//program() 语法分析的入口，分析整个C语言程序
//expression(level) 用于解析一个表达式
//eval() 虚拟机的入口，用于解释目标代码

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

int token;              //当前token
char *src, *old_src;    //源代码字符串的指针
int poolsize;           //默认的 text/data/stack段 大小
int line;               //当前处理的源代码行号

int *text,          //代码段
    *old_text,      //代码段转储
    *stack,         //栈
char *data;         //数据段

int *pc,            //程序计数器
    *sp,            //指针寄存器
    *bp,            //基址寄存器
    ax,             //通用寄存器
    cycle;          //TODO: 

enum {  
    LEA,        //操作bp寄存器
    IMM,        //取立即数
    JMP,        //跳转
    CALL,       //跳转到地址
    JZ,         //0则跳转
    JNZ,        //非0则跳转
    ENT,        //make new call frame。即保存当前栈指针，并在栈上保留一定空间用以存放局部变量
    ADJ,        //remove arguments from frame。将调用子函数时压入栈中的数据清除。
    LEV,        //类似pop，恢复函数调用前的现场并返回。
    LI,         //将ax（存放地址）所指的int载入ax中
    LC,         //将ax（存放地址）所指的char载入ax中
    SI,         //将ax中的int存入地址中，要求栈顶存放地址
    SC,         //将ax中的char存入地址中，要求栈顶存放地址。
    PUSH,       //将ax寄存器中的数入栈
    OR,         //或
    XOR,        //异或
    AND,        //且
    EQ,         //相等
    NE,         //不相等
    LT,         //TODO: 
    GT,         //TODO: 
    LE,         //TODO: 
    GE,         //TODO: 
    SHL,        //TODO: 
    SHR,        //TODO: 
    ADD,        //加
    SUB,        //减
    MUL,        //乘
    DIV,        //除
    MOD,        //模
    OPEN,       //调用内部函数open
    READ,       //调用内部函数read
    CLOS,       //TODO: 
    PRTF,       //TODO: 
    MALC,       //TODO: 
    MSET,       //TODO: 
    MCMP,       //TODO: 
    EXIT        //TODO: 
};

//获取下一个标记
void next(){
    token=*src;
    ++src;
    return;
}

//解析表达式
void expression(int level){
    //do nothing
}

//语法分析入口，分析整个C语言程序
void program(){

    //获取下一个token
    next();

    //输出所有的源代码字符
    while(token>0){
        printf("token is: %c\n", token);
        next();
    }
}

//虚拟机的入口
int eval(){
    int op, *tmp;

    
    while(1){

        //等效于MOVE指令
        /*
        为何在SI/SC指令中，地址存放在栈中，而LI/LC指令中，地址存放在ax中？
        因为默认计算结果放在ax中，而地址通常需要通过计算获得，故执行LI/LC时
        直接从ax取会更高效。PUSH指令只能将ax的值放到栈上，而不能以值作为参数。
        */
        if(op==IMM)     {ax=*pc++;}             //取立即数
        else if(op==LC) {ax=*((char*)ax);}      //取char
        else if(op==LI) {ax=*((int*)ax);}       //取int
        else if(op==SC) {*((char*)*sp++)=ax;}   //存储char
        else if(op==SI) {*((int*)*sp++)=ax;}    //存储int

        //PUSH/JUMP类
        else if(op==PUSH)   {*--sp=ax;}         //ax中的值入栈
        else if(op==JMP)    {pc=(int*)*pc;}     //跳转
        else if(op==JZ)     {pc=ax?pc+1: (int*)*pc;}    //ax中为0则跳转
        else if(op==JNZ)    {pc=ax?(int*)*pc : pc+1;}   //ax中为非0则跳转

        //子程序调用
        else if(op==CALL)   {*--sp=(int)(pc+1); pc=(int*)*pc;}      //子程序调用
        else if(op==ENT)    {*--sp=(int)bp; bp=sp; sp=sp-*pc++;}    //pc中的值？
        else if(op==ADJ)    {sp=sp+*pc++;}      //清除函数调用时压入栈中的数据
        else if(op==LEV)    {sp=bp; bp=(int*)*sp++;pc=(int*)*sp++;}     //恢复bp/sp/pc，退出函数调用
        else if(op==LEA)    {ax=(int)(bp+*pc++);}       //获取第一个参数的地址

        //运算符指令
        //每个运算符都是二元的，第一个参数放在栈顶，第二个参数放在ax中
        else if(op==OR)     ax=*sp++|ax;
        //TODO: FIRST
    }

    return 0;
}

//main
int main(int argc, char** argv){
    int i, fd;

    //跳过程序名本身
    --argc;
    ++argv;

    poolsize=256*1024;          //任意数值
    line=1;                     //当前为源程序第1行

    //打开argv所指的文件
    if((fd=open(*argv, 0))<0){
        printf("could not open(%s)\n", *argv);
        return -1;
    }

    //分配内存用于存放源代码
    if(!(src=old_src=malloc(poolsize))){
        printf("could not malloc(%d) for source area\n", poolsize);
    }

    //读取源文件代码
    if((i=read(fd, src, poolsize-1))<=0){
        printf("read() returned %d\n", i);
        return -1;
    }

    src[i]='\0';        //追加EOF字符
    close(fd);          //关闭文件

    //为虚拟机分配内存
    if(!(text=old_text=malloc(poolsize))){
        printf("could not malloc(%d) for text area\n", poolsize);
        return -1;
    }

    if(!(data=malloc(poolsize))){
        printf("could not malloc(%d) for data area\n", poolsize);
        return -1;
    }

    if(!(stack=malloc(poolsize))){
        printf("could not malloc(%d) for stack area\n", poolsize);
        return -1;
    }

    //初始化内存
    memset(text, 0, poolsize);
    memset(data, 0, poolsize);
    memset(stack, 0, poolsize);

    //因stack为void*, 先将其转化为int，再加上poolsize
    //stack便指向分配的内存的高地址处，再转化为int*赋值给bp和sp
    bp=sp=(int*)((int)stack+poolsize);
    ax=0;

    program();          //进行解析

    eval();             //执行程序

    return 0;           //退出程序
}
