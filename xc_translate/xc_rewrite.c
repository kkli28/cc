#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

//printf executed instructions / assembly and source
int debug;
int assembly;

int token;      //current token

//虚拟机指令
enum {

    //指令带参数
    LEA,        //0  操作bp寄存器
    IMM,        //1  加载立即数
    JMP,        //2  无条件跳转
    CALL,       //3  函数调用
    JZ,         //4  0则跳转
    JNZ,        //5  非0则跳转
    ENT,        //6  创建函数调用帧
    ADJ,        //7  退栈N个位置(用于移除函数调用进栈的参数)

    //指令不带参数
    LEV,        //8  恢复函数调用前的现场(类似POP)
    LI,         //9  将ax(存放地址)所指的int值载入ax
    LC,         //10 将ax(存放地址)所指的char值载入ax
    SI,         //11 将ax中的int存入*SP(存放地址)所指内存中
    SC,         //12 将ax中的char存入*SP(存放地址)所指内存中
    PUSH,       //13 将ax中的数入栈
    OR,         //14 |  或
    XOR,        //15 ^  异或
    AND,        //16 &  与
    EQ,         //17 == 相等
    NE,         //18 != 不相等
    LT,         //19 <  小于
    GT,         //20 >  大于
    LE,         //21 <= 小于等于
    GE,         //22 >= 大于等于
    SHL,        //23 << 左移
    SHR,        //24 >> 右移
    ADD,        //25 +  加
    SUB,        //26 -  减
    MUL,        //27 *  乘
    DIV,        //28 /  除
    MOD,        //29 %  模
    OPEN,       //30 调用内置函数open
    READ,       //31 调用内置函数read
    CLOS,       //32 调用内置函数close
    PRTF,       //33 调用内置函数printf
    MALC,       //34 调用内置函数malloc
    MSET,       //35 调用内置函数memset
    MCMP,       //36 调用内置函数memcmp
    EXIT        //37 调用内置函数exit
};

//token and classed
enum{ 
    //0~127为char字符有效值区间
    //为避免和有效字符同值，所以Num从128开始
    Num=128, Fun, Sys, Glo, Loc, Id,
    Char, Else, Enum, If, Int, Return, Sizeof, While, //关键字
    
    //Or为或(|) Lor为逻辑或(||) Cond为问号表达式里的问号(?)
    Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt,
    Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};

//fields of identifier
enum {
    Token,      //标记。如变量是Id，Loc等
    Hash,       //Hash值。用于快速比较
    Name,       //标识符本身的字符串。如int a中的a
    Type,       //类型。如int、char或者指针
    Class,      //类别。如数字、全局/局部变量
    Value,      //值。a=3，则为3，若是函数则是函数地址
    
    //Bxxx 当局部标识符和全局的同名时，全局的转移到这里
    BType,      //全局的标识符的Type
    BClass,     //...Class
    BValue,     //...Value
    IdSize      //当前标识符符号表开始位置递进IdSize即可到达下一个
};

//types of variable/function
enum { CHAR, INT, PTR };
int *idmain;    //main函数

//types of declaration
//TODO:  enum {Global...}

int *text,      //text segment
    *stack,     //stack
    *old_text;  //for dump text segment
char* data;     //data segment

//idmain
//TODO: int *idmain;

char *src, *old_src;    //pointer to source code string;
int poolsize;           //default size of text/data/stack

//virtual machine registers
int *pc, *bp, *sp, ax, cycle;

// current parsed ID
int *current_id,        //当前解析的ID
    *symbols;           //符号表
int token_val;          //当前token的值
int line;               //line number

//the type of declaration, make it global for convenience
//TODO: int basetype;

//function frame
//TODO: 0: arg1...

//index of bp pointer on stack
//TODO: int index_of_bp;

//获取下一个Token
//若获取成功，则return。若遇到空白符则继续获取，直到成功获取Token。
void next(){
    char *last_pos;         //记录标识符名字的起始位置
    int hash;               //标识符的hash值，用于快速比较

    while(token=*src){ //不是结束符，则继续
        ++src;

        //遇到 '\n'，则 [输出汇编代码,] 行号+1
        if(token=='\n'){

            //打印编译信息
            if (assembly) {
                printf("%d: %.*s", line, src-old_src, old_src);
                old_src = src;

                while (old_text < text) {
                    printf("%8.4s", & "LEA ,IMM ,JMP ,CALL,JZ  ,JNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PUSH,"
                                      "OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,"
                                      "OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT"[*++old_text * 5]);

                    if (*old_text <= ADJ)
                        printf(" %d\n", *++old_text);
                    else
                        printf("\n");
                }
            }
            ++line; //行号+1
        }
        
        //遇到 '#'，则跳过该行
        else if(token=='#'){
            while(*src!=0 && *src!='\n'){
                ++src;
            }
        }

        //遇到字符，则为标识符
        else if((token>='a' && token<='z') 
        || (token>='A' && token<='Z') || (token=='_')){
            //标识符的起始位置
            last_pos=src-1;
            hash=token;  //默认hash为当前token的值
            
            //跳过标识符剩余部分
            while((*src>='a' && *src<='z')
                || (*src>='A' && *src<='Z')
                || (*src>='0' && *src<='9') || (*src=='_')){
                    hash=hash*147+*src; //改变hash值
                    ++src;
            }

            //查找是否已存在同名标识符。线性查找
            current_id=symbols;

            //以内symbols将所有空间初始化为0，若current_id[Token]
            //为空，则说明后续没有标识符信息了
            //否则符号表还有标识符信息
            while(current_id[Token]){

                //hash不等，则不可能同名。可快速剔除不符合的从而提高比较性能
                //hash相等，有可能同名，需要再比较名字
                if(current_id[Hash]==hash && 
                    !memcmp((*char*)current_id[Name], last_pos, src-last_pos)){
                        //找到同名的，意味着当前语句是使用该标识符
                        //如已有int a; 之后a=3;
                        token=current_id[Token];
                        return;
                }

                //没有找到同名的，继续找下一个
                current_id=current_id+IdSize;
            }

            //没有找到同名的，就填入新的标识符信息
            //只填入Token、Name、Hash，其余信息由语法分析填入
            token=current_id[Token]=Id;
            current_id[Hash]=hash;
            current_id[Name]=(int)last_pos;
            return;
        }

        //遇到数字
        else if(token>='0' && token<='9'){
            //三种数字: oct(0123)  dec(123)  hex(0x123)
            token_val=token-'0';

            //首部字符大于0，只可能是十进制dec
            if(token_val>0){
                while(*src>='0' && *src<='9'){
                    token_val=token_val*10 + *src++ -'0';
                }
            }
            //首字符为0，则可能为八进制oct或十六进制hex
            else{
                //十六进制
                //A(1000001) a(1100001)尾部都是1，&15即可获取尾部
                //若字符大于等于A，则还需要加上9才是该字符对应的十进制值
                if(*src=='x' || *src=='X'){
                    token=*++src;
                    while((token>='0' && token<='9') 
                        || (token>='a' && token<='f')
                        || (token>='A' && token<='F')){
                            token_val=token_val*16
                                + (token&15)
                                + (token>='A'?9: 0);
                            token=*++src;  
                    }
                }
                //八进制
                else{
                    while(*src>='0' && *src<='7'){
                        token_val=token_val*8 + *src++ - '0';
                    }
                }
            }

            //立即数类型，不写入符号表
            //token=Num  token_val为其数值
            token=Num;
            return;
        }

        //遇到 '/'，可能是注释或除号
        else if(token=='/'){
            //注释
            if(*src=='/'){
                //跳过注释内容
                while(*src!=0 && *src!='\n'){
                    ++src;
                }
            }
            else{
                //除号
                token=Div;
                return;
            }
        }

        //遇到 " 或 '，则可能是字符(串)
        else if(token=='"' || token=='\''){
            //仅支持 '\n'，若是字符串，则将其放入data中
            last_pos=data;
            while(*src!=0 && *src!=token){
                token_val=*src++;
                //字符
                //只支持 '\n'，否则，'\x'就看作字符x
                if(token_val=='\\'){
                    token_val=*src++;
                    if(token_val=='n'){
                        token_val='\n';
                    }
                }

                //将字符串存入data
                if(token=='"'){
                    *data++=token_val;
                }
            }

            //字符串则返回其地址
            if(token=='"'){
                token_val=(int)last_pos;
            }
            //字符则返回Num
            else {
                token=Num;
            }
            return;
        }

        //遇到 '='，可能是 '==' 或 '='
        else if(token=='='){
            if(*src=='='){
                ++src;
                token=Eq;
            }
            else{
                token=Assign;
            }
            return;
        }

        //遇到 '+'，可能是 '+' 或 '++'
        else if(token=='+'){
            if(*src=='+'){
                ++src;
                token=Inc;
            }
            else{
                token=Add;
            }
            return;
        }

        //遇到 '-'，可能是 '-' 或 '--'
        else if(token=='-'){
            if(*src=='-'){
                ++src;
                token=Dec;
            }
            else{
                token=Sub;
            }
            return;
        }

        //遇到 '!'，可能是 '!' 或 '!='
        else if(token=='!'){
            if(*src=='='){
                ++src;
                token=Ne;
            }
            //若是 '!'，则其本身代表其Token类型
            return;
        }

        //遇到 '<'，可能是 '<' 或 <<' 或 '<='
        else if(token=='<'){
            if(*src=='<'){
                ++src;
                token=Shl;
            }
            else if(token=='='){
                ++src;
                token=Le;
            }
            else {
                token=Lt;
            }
            return;
        }

        //遇到 '>'，可能是 '>' 或 '>>' 或 '>='
        else if(token=='>'){
            if(*src=='>'){
                ++src;
                token=Shr;
            }
            else if(*src=='='){
                ++src;
                token=Ge;
            }
            else{
                token=Gt;
            }
            return;
        }

        //遇到 '|'，可能是 '|' 或 '||'
        else if(token=='|'){
            if(*src=='|'){
                ++src;
                token=Lor;
            }
            else {
                token=Or;
            }
            return;
        }

        //遇到 '&'，可能是 '&' 或 '&&'
        else if(token=='&'){
            if(*src=='&'){
                ++src;
                token=Lan;
            }
            else {
                token=And;
            }
            return;
        }

        //遇到 '^'
        else if(token=='^'){
            token=Xor;
            return;
        }

        //遇到 '%'
        else if(token=='%'){
            token=Mod;
            return;
        }

        //遇到 '*'
        else if(token=='*'){
            token=Mul;
            return;
        }

        //遇到 '['
        else if(token=='['){
            token=Brak;
            return;
        }

        //遇到 '?'
        else if(token=='?'){
            token=Cond;
            return;
        }

        //其余非空字符
        else if(
            token=='~' ||
            token==';' ||
            token=='{' ||
            token=='}' ||
            token=='(' ||
            token==')' ||
            token==']' ||
            token==',' ||
            token==':'){
            return;
        }

        //空白字符则跳过，不返回，继续读取下一个Token
    }
}

//function: match
//TODO: void match(int tk)...

//expression
void expression(int level){
    //do nothing
}

//function: statement
//TODO: void statement()...

//function: enum_declaration
//TODO: void enum_declaration()...

//function: function_parameter
//TODO: void function_parameter()...

//function: function_body
//TODO: void function_body()...

//function: function_declaration
//TODO: void function_declaration()...

//function: global_declaration
//TODO: void global_declaration()...

//program
void program(){
    /*
    next();
    while(token>0){
        printf("token is: %c\n", token);
        next();
    }
    */
}

//虚拟机
int eval(){
    int op;     //读取的指令
    int *tmp;
    cycle=0;    //记录指令条数

    //无限循环，直到程序结束
    while(1){
        ++cycle;

        //取一条指令
        op=*pc++;

        //输出调试信息(执行的指令)
        if (debug) {
            printf("%d> %.4s", cycle,
                   & "LEA ,IMM ,JMP ,CALL,JZ  ,JNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PUSH,"
                   "OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,"
                   "OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT"[op * 5]);
            if (op <= ADJ)
                printf(" %d\n", *pc);
            else
                printf("\n");
        }

        //移动数据相关(MOV)
        if(op==IMM)         { ax=*pc++;         }
        else if(op==LC)     { ax=*(char*)ax;    }
        else if(op==LI)     { ax=*(int*)ax;     }
        else if(op==SC)     { *(char*)*sp++=ax; }
        else if(op==SI)     { *(int*)*sp++=ax;  }
        else if(op==PUSH)   { *--sp=ax;         }
        else if(op==JMP)    { pc=(int*)*pc;     }
        else if(op==JZ)     { pc=ax? pc+1: (int*)*pc; }
        else if(op==JNZ)    { pc=ax? (int*)*pc: pc+1; }
        
        //函数调用相关
        else if(op==CALL)   { *--sp=(int)(pc+1); pc=(int*)*pc; }
        else if(op==ENT) {
            *--sp=(int)bp;  //保存bp
            bp=sp;          //bp指向原栈顶
            sp=sp-*pc++;    //栈上创建函数调用需要的空间
        }
        else if(op==ADJ)    { sp=sp+*pc++;      }
        else if(op==LEV)    { sp=bp; bp=(int*)*sp++; pc=(int*)*sp++; }
        else if(op==LEA)    { ax=(int)(bp+*pc++);    }

        //运算符
        else if(op==OR)     { ax=*sp++ | ax;    }
        else if(op==XOR)    { ax=*sp++ ^ ax;    }
        else if(op==AND)    { ax=*sp++ & ax;    }
        else if(op==EQ)     { ax=*sp++ == ax;   }
        else if(op==NE)     { ax=*sp++ != ax;   }
        else if(op==LT)     { ax=*sp++ < ax;    }
        else if(op==LE)     { ax=*sp++ <= ax;   }
        else if(op==GT)     { ax=*sp++ > ax;    }
        else if(op==GE)     { ax=*sp++ >= ax;   }
        else if(op==SHL)    { ax=*sp++ << ax;   }
        else if(op==SHR)    { ax=*sp++ >> ax;   }
        else if(op==ADD)    { ax=*sp++ + ax;    }
        else if(op==SUB)    { ax=*sp++ - ax;    }
        else if(op==MUL)    { ax=*sp++ * ax;    }
        else if(op==DIV)    { ax=*sp++ / ax;    }
        else if(op==MOD)    { ax=*sp++ % ax;    }

        //调用内置函数
        else if(op==EXIT)   { printf("exit(%d)", *sp); return *sp;}
        else if(op==OPEN)   { ax=open((char*)sp[1], sp[0]); }
        else if(op==CLOS)   { ax=close(*sp); }
        else if(op==READ)   { ax=read(sp[2], (char*)sp[1], *sp); }
        else if(op==PRTF)   {
            tmp=sp+pc[1];
            ax=printf((char*)tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5], tmp[-6]);
        }
        else if(MALC)       { ax=(int)malloc(*sp); }
        else if(MSET)       { ax=(int)memset((char*)sp[2], sp[1], *sp); }
        else if(MCMP)       { ax=memcmp((char*)sp[2], (char*)sp[1], *sp); }
        else {
            printf("unknown instruction: %d\n", op);
            return -1;
        }
    }
}

int main(int argc, char** argv){
    int index;      //index for readed char or keywords in enum {Char...}
    int fd;         //file description
    debug=1;
    assembly=0;

    //int *tmp;

    --argc;
    ++argc;

    //parse arguments
    //TODO: 1287 - 1300

    if((fd=open(*argv, 0))<0){
        printf("could not open(%s)\n", *argv);
        return -1;
    }

    //arbitrary size
    poolsize=256*1024;
    line=1;

    //allocate memory
    //TODO: 1310 - 1333
    if(!(text=malloc(poolsize))){
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
    //malloc symbols's memory
    //TODO: if(!(symbols...

    //init memory
    memset(text, 0, poolsize);
    memset(data, 0, poolsize);
    memset(stack, 0, poolsize);
    old_text=text;

    //init symbols's memory
    //TODO: memset(symbols, 0, poolsize);
    
    src="char else enum if int return sizeof while open read "
        "close printf malloc memset memcmp exit void main";
    //向符号表中添加关键字
    index=Char;
    while(i<=While){
        next();
        current_id[Token]=index++;
    }
    //向符号表中添加内置函数
    index=OPEN;
    while(index<=EXIT){
        next();
        current_id[Class]=Sys;
        current_id[Type]=INT;
        current_id[Value]=index++;
    }
    next(); current_id[Token]=Char; //TODO: void类型?
    next(); idmain=current_id;      //TODO: main?

    //malloc for source area
    if(!(src=old_src=malloc(poolsize))){
        printf("could not malloc(%d) for source area\n", poolsize);
        return -1;
    }

    //read the source file
    if((index=read(fd, src, poolsize-1))<=0){
        printf("read() returned %d\n", index);
        return -1;
    }

    src[index]=0;       //add EOF character
    close(fd);      //close file

    program();      //analisis

    //main() not defined
    //TODO: 1371 - 1376

    //dump_text
    //TODO: 1377 - 1380

    //setup stack
    //TODO: 1382 - 1388
    sp=(int*)((int)stack+poolsize);

    //TODO: TEST
    index=0;
    text[index++]=IMM;
    text[index++]=10;
    text[index++]=PUSH;
    text[index++]=IMM;
    text[index++]=20;
    text[index++]=ADD;
    text[index++]=PUSH;
    text[index++]=EXIT;
    pc=text;

    return eval();  //execute
}
