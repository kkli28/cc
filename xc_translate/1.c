
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
    //do nothing
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

    //为程序分配内存
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

    program();          //进行解析

    eval();             //执行程序

    return 0;           //退出程序
}
