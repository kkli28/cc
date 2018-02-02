
//next() 用于词法分析，获取下一个标记，它将自动忽略空白字符
//program() 语法分析的入口，分析整个C语言程序
//expression(level) 用于解析一个表达式
//eval() 虚拟机的入口，用于解释目标代码

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

//源代码相关
int token;              //当前token
char *src, *old_src;    //源代码字符串的指针
int poolsize;           //默认的 text/data/stack段 大小
int line;               //当前处理的源代码行号

//内存分区
int *text,          //代码段
    *old_text,      //代码段转储
    *stack;         //栈
char *data;         //数据段

//各种寄存器
int *pc,            //程序计数器
    *sp,            //指针寄存器
    *bp,            //基址寄存器
    ax,             //通用寄存器
    cycle;          //TODO: 

//标识符、符号表
int token_val;      //当前标识符的值
int token;          //当前标识符
int *current_id,    //当前分析的ID
    *symbols;       //符号表

//虚拟机指令集
enum {  
    LEA,        //0  操作bp寄存器
    IMM,        //1  取立即数
    JMP,        //2  跳转
    CALL,       //3  跳转到地址
    JZ,         //4  0则跳转
    JNZ,        //5  非0则跳转
    ENT,        //6  make new call frame。即保存当前栈指针，并在栈上保留一定空间用以存放局部变量
    ADJ,        //7  remove arguments from frame。将调用子函数时压入栈中的数据清除。
    LEV,        //8  类似pop，恢复函数调用前的现场并返回。
    LI,         //9  将ax（存放地址）所指的int载入ax中
    LC,         //10 将ax（存放地址）所指的char载入ax中
    SI,         //11 将ax中的int存入地址中，要求栈顶存放地址
    SC,         //12 将ax中的char存入地址中，要求栈顶存放地址。
    PUSH,       //13 将ax寄存器中的数入栈
    OR,         //14 或
    XOR,        //15 异或
    AND,        //16 与
    EQ,         //17 相等
    NE,         //18 不相等
    LT,         //19 小于
    GT,         //20 大于
    LE,         //21 小于等于
    GE,         //22 大于等于
    SHL,        //23 左移
    SHR,        //24 右移
    ADD,        //25 加
    SUB,        //26 减
    MUL,        //27 乘
    DIV,        //28 除
    MOD,        //29 模
    OPEN,       //30 调用内置函数open
    READ,       //31 调用内置函数read
    CLOS,       //32 调用内置函数close
    PRTF,       //33 调用内置函数printf
    MALC,       //34 调用内置函数malloc
    MSET,       //35 调用内置函数memset
    MCMP,       //36 调用内置函数memcmp
    EXIT        //37 调用内置函数exit
};

//标记类型
enum {
    Num=128,        //数字
    Fun,            //函数
    Sys,            //TODO: 
    Glo,            //全局变量
    Loc,            //局部变量
    Id,             //变量名
    Char,           // char
    Else,           // else
    Enum,           // enum
    If,             // if
    Int,            // int
    Return,         // return
    Sizeof,         // sizeof
    While,          // while
    Assign,         // =
    Cond,           // ?
    Lor,            // ||
    Lan,            // &&
    Or,             // |
    Xor,            // ^
    And,            // &
    Eq,             // ==
    Ne,             // !=
    Lt,             // <
    Gt,             // > 
    Le,             // <= 
    Ge,             // >= 
    Shl,            // <<
    Shr,            // >>
    Add,            // +
    Sub,            // -
    Mul,            // *
    Div,            // /
    Mod,            // %
    Inc,            // ++
    Dec,            // --
    Brak            // [
    
    //TODO: 下方的符号可不用常量标识，直接用本身
    /*
    Rb,             // ]
    Lp,             // (
    Rp,             // )
    Ti,             // ~
    Sem,            // ;
    Lbr,            // {
    Rbr,            // }
    Comma,          // ,
    Colon           // :
    */
};

//标识符域
enum {
    Token,          //标识符ID
    Hash,           //标识符的哈希值，用于标识符的快速比较
    Name,           //标识符本身的字符串
    Class,          //标识符的类别，如数字、全局变量、局部变量等。
    Type,           //标识符的类型，如int型、char型、指针型
    Value,          //标识符的值，如果是函数则存放其的地址

    //Bxxxx: 当局部标识符的名字与全局标识符相同时，用作保存全局标识符的信息
    BType,
    BClass,
    BValue,
    IdSize          //一个标识符类型的存储长度
};

//变量、函数类型
enum { CHAR, INT, PTR };

//main函数
int* idmain;

//获取下一个标记
void next(){
    char *last_pos;
    int hash;

    while(token=*src){
        ++src;
        
        //遇到换行符，行号+1
        if(token=='\n') ++line;

        //跳过宏定义
        else if(token=='#'){
            while(*src!=0 && *src!='\n') ++src;
        }

        //标识符与符号表
        else if((token>='a' && token<='z') || (token>='A' && token<='Z') || token=='_'){
            last_pos=src-1;
            hash=token;
            while((*src>='a' && *src<='z') || (*src>='A' && *src<='Z') || (*src>='0' && *src<='9') || (*src=='_')){
                hash=hash*147+*src;
                ++src;
            }

            current_id=symbols;
            while(current_id[Token]){
                if(current_id[Hash]==hash && memcmp((char*)current_id[Name], last_pos, src-last_pos)){
                    token=current_id[Token];
                    return;
                }
                current_id=current_id+IdSize;
            }

            //存放新的ID
            current_id[Name]=(int)last_pos;
            current_id[Hash]=hash;
            token=current_id[Token]=Id;
            return;
        }

        //数字
        else if(token>='0' && token<='9'){
            //parse数字，有三种类型: dec(123), hex(0x123), oct(017)
            token_val=token-'0';

            //十进制型
            if(token_val>0){
                while(*src>='0' && *src<='9'){
                    token_val=token_val*10+*src++-'0';
                }
            }

            else{

                //十六进制
                if(*src=='x' || *src=='X'){
                    token=*++src;
                    while((token>='0' && token<='9') || (token>='a' && token<='f') || (token>='A' && token<='F')){
                        token_val=token_val*16+(token&15)+(token>='A'?9:0);
                        token=*++src;
                    }
                }

                //八进制
                else{
                    while(*src>='0' && *src<='7'){
                        token_val=token_val*8+*src++-'0';
                    }
                }
            }

            token=Num;
            return;
        }
        
        //字符串
        //形如"abc"，则last_pos指向第一个字符，并且将abc存入data
        //形如"\a"，则跳过\，将a存入data
        //形如'\aaaa'，则只取a，作为Num
        //若为'\n'，则取 \n 作为Num
        else if(token=='"' || token=='\''){
            last_pos=data;
            while(*src!=0 && *src!=token){
                token_val=*src++;
                if(token_val=='\\'){
                    token_val=*src++;
                    if(token_val=='n') token_val='\n';
                }
                if(token=='"') *data++=token_val;
            }

            src++;
            if(token=='"') token_val=(int)last_pos;
            else token=Num;
            return;
        }

        //注释 或 /
        else if(token=='/'){
            if(*src=='/'){
                while(*src!=0 && *src!='\n') ++src;
            }
            else{

                //除号
                token=Div;
                return;
            }
        }

        //== 或 =
        else if(token=='='){
            if(*src=='='){
                ++src;
                token=Eq;
            }
            else token=Assign;
            return;
        }

        //++ 或 +
        else if(token=='+'){
            if(*src=='+'){
                ++src;
                token=Inc;
            }
            else token=Add;
            return;
        }

        //-- 或 -
        else if(token=='-'){
            if(*src=='-'){
                ++src;
                token=Dec;
            }
            else token=Sub;
            return;
        }

        //!= 或 !
        else if(token=='!'){
            if(*src=='='){
                ++src;
                token=Ne;
            }
            return;
        }

        //<= 或 << 或 <
        else if(token=='<'){
            if(*src=='='){
                ++src;
                token=Le;
            }
            else if(*src=='<'){
                ++src;
                token=Shl;
            }
            else token=Lt;
            return;
        }

        //>= 或 >> 或 >
        else if(token=='>'){
            if(*src=='='){
                ++src;
                token=Ge;
            }
            else if(*src=='>'){
                ++src;
                token=Shr;
            }
            else token=Gt;
            return;
        }

        //|| 或 |
        else if(token=='|'){
            if(*src=='|'){
                ++src;
                token=Lor;
            }
            else token=Or;
            return;
        }

        //&& 或 &
        else if(token=='&'){
            if(*src=='&'){
                ++src;
                token=Lan;
            }
            else token=Add;
            return;
        }

        //^
        else if(token=='^'){
            token=Xor;
            return;
        }

        //%
        else if(token=='%'){
            token=Mod;
            return;
        }

        //*
        else if(token=='*'){
            token=Mul;
            return;
        }

        //[
        else if(token=='['){
            token=Brak;
            return;
        }
        
        //?
        else if(token=='?'){
            token=Cond;
            return;
        }

        //~ ; { } ( ) ] , :
        else if(token=='~' || token==';' || token=='{' || token=='}' 
        || token=='(' || token==')' || token==']' || token==',' || token==':') {
            return;
        }

        //空字符则直接跳过，不返回
        //else { ; }
    }
}

/*
struct identifier{
    int token;
    int hash;
    char* name;
    int class;
    int type;
    int value;
    int Bclass;
    int Btype;

}
*/

//解析表达式
void expression(int level){
    //do nothing
}

//语法分析入口，分析整个C语言程序
void program(){

    //Test
    return;

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

        //取一条指令
        op=*pc++;

        //等效于MOVE指令
        /*
        为何在SI/SC指令中，地址存放在栈中，而LI/LC指令中，地址存放在ax中？
        因为默认计算结果放在ax中，而地址通常需要通过计算获得，故执行LI/LC时
        直接从ax取会更高效。PUSH指令只能将ax的值放到栈上，而不能以值作为参数。
        */
        if(op==IMM)     { ax=*pc++; }               //取立即数
        else if(op==LC) { ax=*((char*)ax); }        //取char
        else if(op==LI) { ax=*((int*)ax); }         //取int
        else if(op==SC) { *((char*)*sp++)=ax; }     //存储char
        else if(op==SI) { *((int*)*sp++)=ax; }      //存储int

        //PUSH/JUMP类
        else if(op==PUSH)   { *--sp=ax; }                   //ax中的值入栈
        else if(op==JMP)    { pc=(int*)*pc; }               //跳转
        else if(op==JZ)     { pc=ax?pc+1: (int*)*pc; }      //ax中为0则跳转
        else if(op==JNZ)    { pc=ax?(int*)*pc : pc+1; }     //ax中为非0则跳转

        //子程序调用
        else if(op==CALL)   { *--sp=(int)(pc+1); pc=(int*)*pc; }        //子程序调用
        else if(op==ENT)    { *--sp=(int)bp; bp=sp; sp=sp-*pc++; }      //pc中的值？
        else if(op==ADJ)    { sp=sp+*pc++; }                            //清除函数调用时压入栈中的数据
        else if(op==LEV)    { sp=bp; bp=(int*)*sp++;pc=(int*)*sp++; }   //恢复bp/sp/pc，退出函数调用
        else if(op==LEA)    { ax=(int)(bp+*pc++); }                     //获取第一个参数的地址

        //运算符指令
        //每个运算符都是二元的，第一个参数放在栈顶，第二个参数放在ax中
        else if(op==OR)     ax=*sp++ | ax;
        else if(op==XOR)    ax=*sp++ ^ ax;
        else if(op==AND)    ax=*sp++ & ax;
        else if(op==EQ)     ax=*sp++ == ax;
        else if(op==NE)     ax=*sp++ != ax;
        else if(op==LT)     ax=*sp++ < ax;
        else if(op==LE)     ax=*sp++ <= ax;
        else if(op==GT)     ax=*sp++ > ax;
        else if(op==GE)     ax=*sp++ >= ax;
        else if(op==SHL)    ax=*sp++ << ax;
        else if(op==SHR)    ax=*sp++ >> ax;
        else if(op==ADD)    ax=*sp++ + ax;
        else if(op==SUB)    ax=*sp++ - ax;
        else if(op==MUL)    ax=*sp++ * ax;
        else if(op==DIV)    ax=*sp++ / ax;
        else if(op==MOD)    ax=*sp++ % ax;

        //内置函数
        else if(op==EXIT)   { printf("exit(%d)\n", *sp); return *sp; }    //退出
        else if(op==OPEN)   { ax=open((char*)sp[1], sp[0]); }           //打开文件
        else if(op==CLOS)   { ax=close(*sp); }                          //关闭文件
        else if(op==READ)   { ax=read(sp[2], (char*)sp[1], *sp); }      //读取文件
        else if(op==PRTF)   {                                           //输出
            tmp=sp+pc[1];
            ax=printf((char*)tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5], tmp[-6]);
        }
        else if(op==MALC)   { ax=(int)malloc(*sp); }                            //申请内存
        else if(op==MSET)   { ax=(int)memset((char*)sp[2], sp[1], *sp); }       //设置内存中的值
        else if(op==MCMP)   { ax=memcmp((char*)sp[2], (char*)sp[1], *sp); }     //内存值比较
        else {
            printf("unknown instruction : %d", op);
            return -1;
        }
    }
    return 0;
}

//main
int main(int argc, char** argv){
    int i, fd;

    //跳过程序名本身
    --argc;
    ++argv;

    //打开argv所指的文件
    if((fd=open(*argv, 0))<0){
        printf("could not open(%s)\n", *argv);
        return -1;
    }

    poolsize=256*1024;          //任意数值
    line=1;                     //当前为源程序第1行

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

    //添加内置类型
    src="char else enum if int return sizeof while"
        "open read close printf malloc memset memcmp exit void main";
    i=Char;
    while(i<=While){
        next();
        current_id[Token]=i++;
    }

    i=OPEN;
    while(i<=EXIT){
        next();
        current_id[Class]=Sys;
        current_id[Type]=INT;
        current_id[Value]=i++;
    }

    next(); current_id[Token]=Char; //void 类型
    next(); idmain=current_id;      //keep track of main

    //分配内存用于存放源代码
    if(!(src=old_src=malloc(poolsize))){
        printf("could not malloc(%d) for source area\n", poolsize);
    }

    //读取源文件代码
    if((i=read(fd, src, poolsize-1))<=0){
        printf("read() returned %d\n", i);
        return -1;
    }
    src[i]=0;       //添加EOF
    close(fd);

    //因stack为void*, 先将其转化为int，再加上poolsize
    //stack便指向分配的内存的高地址处，再转化为int*赋值给bp和sp
    bp=sp=(int*)((int)stack+poolsize);
    ax=0;

    program();          //进行解析

    eval();             //执行程序
    
    return 0;           //退出程序
}
