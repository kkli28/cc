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
int basetype;
int expr_type;

//function frame
//TODO: 0: arg1...

//index of bp pointer on stack
int index_of_bp;

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
        //若是字符串，则token为 "，token_val为其首地址
        //若是单个字符，则token为Num，token_val为其十进制值
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
void match(int tk){
    if(token==tk){
        next();
    }
    else{
        printf("%d: expected token: %d\n", line, tk);
        exit(-1);
    }
}

//expression
void expression(int level){
    int *id;    //current_id的指针
    int tmp;    //函数调用时参数的个数、表达式类型
    int *addr;  //跳转地址

    //一元运算符
    {
        if(!token){
            printf("%d: unexpected token EOF of expression\n", line);
            exit(-1);
        }

        //立即数
        if(token==Num){
            match(Num);

            //生成: IMM val
            *++text=IMM;
            *++text=token_val;
            expr_type=INT;      //记录表达式类型
        }

        //字符串
        else if(token=='"'){
            //生成: IMM token_val
            *++text=IMM;
            *++text=token_val;
            match('"');

            //载入后续的字符串，因为多个字符串都在data中连续存放，
            //因此不需要再 IMM token_val 了。详见info.md
            while(token=='"'){
                match('"');
            }

            //(int)data+sizeof(int)即在字符串末尾添加 '\0'
            //因data初始化为0，因此只需后移一个int位置即可
            //& (-sizeof(int)) 为字节对其到int
            data=(char*)(((int)data+sizeof(int)) & (-sizeof(int)));
            expr_type=PTR;  //字符串为指针类型
        }

        //sizeof
        //仅有: sizeof(int), sizeof(char), sizeof(PTR)
        else if(token==Sizeof){
            match(Sizeof);
            match('(');
            expr_type=INT;
            if(token==Int){
                match(Int);
            }
            else{
                match(Char);
                expr_type=CHAR;
            }
            while(token==Mul){
                match(Mul);
                expr_type=expr_type=PTR;
            }
            match(')');
            
            //生成: IMM val
            //若为sizeof(char)则val为1，否则为4
            *++text=IMM;
            *++text=(expr_type==CHAR)? sizeof(char): sizeof(int);
            expr_type=INT;  //表达式的值，为INT
        }

        //取变量的值和函数调用都是以Id标记开始
        else if(token==Id){
            match(Id);
            id=current_id;

            //函数调用
            if(token=='('){
                match('(');
                tmp=0;
                while(token!=')'){
                    expression(Assign); //参数是一个表达式
                    *++text=PUSH;  //参数顺序进栈
                    ++tmp;
                    if(token==','){
                        match(',');
                    }
                }
                match(')');

                //系统内置函数
                if(id[Class]==Sys){
                    *++text=id[Value];  //函数名就是指令
                }
                //自定义的函数
                else if(id[Class]==Fun){
                    *++text=CALL;
                    *++text=id[Value]; //CALL <addr>
                }
                //并非函数
                else{
                    printf("%d: bad function call\n", line);
                    exit(-1);
                }

                //函数调用结束，若有参数，需要将其出栈
                if(tmp>0){
                    *++text=ADJ; 
                    *++text=tmp;
                }
                expr_type=id[Type];  //表达式类型为函数返回类型
            }

            //enum变量
            else if(id[Class]==Num){
                *++text=IMM;
                *++text=id[Value];
                expr_type=INT;
            }

            //变量
            else{
                //局部变量，其符号表Value域存放的是其相对于
                //new_bp的位置
                if(id[Class]==Loc){
                    *++text=LEA;
                    *++text=index_of_bp-id[Value];
                }
                //全局变量，直接从其地址加载值
                else if(id[Class]==Glo){
                    *++text=IMM;
                    *++text=id[Value];
                }
                else{
                    printf("%d: undefined variable\n", line);
                    exit(-1);
                }

                expr_type=id[Type];
                *++text=(expr_type==CHAR)?LC: LI;
            }
        }

        //强制转换 或 括号运算
        else if(token=='('){
            match('(');
            //强制转换
            if(token==Int || token==Char){
                tmp=(token==Char)? CHAR: INT; //转换后的类型
                match(token);
                while(token==Mul){
                    match(Mul);
                    tmp=tmp+PTR;
                }
                match(')');

                //强制转换运算符和++运算符有相同的优先级
                expression(Inc);
                expr_type=tmp;
            }

            //括号运算
            else{
                expression(Assign);
                match(')');
            }
        }

        //解引用 *
        else if(token==Mul){
            match(Mul);
            //解引用运算符和++有相同的优先级
            expression(Inc);

            if(expr_type>=PTR){
                expr_type=expr_type-PTR;
            }
            else{
                printf("%d: bad dereference\n", line);
                exit(-1);
            }

            //若是多次解引用，如****a，则每次都解一次引用
            //每次都LC或LI，最终将多次解引用，从而得到正确的结果
            *++text=(expr_type==CHAR)? LC: LI;
        }

        //取地址 &
        else if(token==And){
            match(And);
            expression(Inc);
            
            //对变量int a; 取地址，即&a，当进入expression(Inc)中时，
            //会生成指令：
            //IMM <addr>
            //LI
            //取地址则只需将其LI指令去掉即可
            if(*text==LC || *text==LI){
                --text;
            }
            else {
                printf("%d: bad address of\n", line);
                exit(-1);
            }

            expr_type=expr_type+PTR;  //类型时前一次类型的指针类型
        }

        //非 !
        else if(token=='!'){
            match('!');
            expression(Inc);
            
            //!expr 即 expr == 0
            *++text=PUSH;
            *++text=IMM;
            *++text=0;
            *++text=EQ;

            expr_type=INT;
        }

        //按位取反 ~
        else if(token=='~'){
            match('~');
            expression(Inc);

            //~expr 即 expr XOR -1
            *++text=PUSH;
            *++text=IMM;
            *++text=-1;
            *++text=XOR;

            expr_type=INT;
        }

        //+expr 即 expr
        else if(token==Add){
            match(Add);
            expression(Inc);
            expr_type=INT;
        }

        //-expr 即 expr*(-1)
        else if(token==Sub){
            match(Sub);

            //-1型，则match(token)后，token_val为1
            //则直接取-token_val即可
            if(token==Num){
                *++text=Imm;
                *++text=-tokan_val;
                match(Mum);
            }

            //-a型，则需要a*(-1)
            else{
                //将-1进栈
                *++text=IMM;
                *++text=-1;
                *++text=PUSH;

                //将a计算后放入ax中
                expression(Inc);

                //ax中的值乘以-1，即-a
                *++text=MUL;
            }
            expr_type=INT;
        }

        //前置++/--
        else if(token==Inc||token==Dec){
            tmp=token;
            match(token);
            expression(Inc);

            //变量的地址将用到两次，第一次取变量值，第二次将
            //递增后的值写回
            //因此步骤为：
            //1. 将变量的地址入栈，并将其值取出放到ax中
            //2. 变量中的值+1，并将该值写回变量地址

            //1
            if(*text==LC){
                *text=PUSH;
                *++text=LC;
            }
            else if(*text==LI) {
                *text=PUSH;
                *++text=LI;
            }
            else {
                printf("%d: bad lvalue of pre-increment\n", line);
                exit(-1);
            }

            //2
            *++text=PUSH;
            *++text=IMM;

            //普通变量+1
            //TODO: ?当时char*型呢？还是+4?
            *++text=(expr_type>PTR)?sizeof(int): sizeof(char);
            *++text=(tmp==Inc)?ADD: SUB;
            *++text=(expr_type==CHAR)?SC: SI;
        }
        else{
            printf("%d: bad expression\n", line);
            exit(-1);
        }
    }

    //二元运算符
    {
        while(token>=level){
            tmp=expr_type;

            //赋值
            if(token==Assign){
                match(Assign);
                if(*text==LC || *text==LI){
                    *text=PUSH;  //保存左值的指针
                }
                else{
                    printf("%d: bad lvalue in assignment\n", line);
                    exit(-1);
                }
                expression(Assign);
                *++text=(expr_type==CHAR)? SC: SI;
            }

            //三元操作 expr?a: b，类似小型if语句
            else if(token==Cond){
                match(Cond);
                *++text=JZ;
                addr=++text;
                expression(Assign);
                if(token==':'){
                    match(':');
                }
                else{
                    printf("%d: missing colon in conditional\n", line);
                    exit(-1);
                }
                *addr=(int)(text+3);
                *++text=JMP;
                addr=++text;
                expression(Cond);  //第二个表达式优先级高于Assign
                *addr=(int)(text+1);
            }

            //逻辑或
            else if(token==Lor){
                match(Lor);
                *++text=JNZ;
                addr=++text;
                expression(Lan);    //当前表达式需要至少&&才能抢走操作数
                *addr=(int)(text+1);
                expr_type=INT;
            }
        }
    }
}

//function: statement
void statement(){
    int *a, *b;  //跳转点a和b

    //if语句
    if(token==If){
        //code                  assembly
        //if (...)              <cond>
        //                      JZ a
        //  <true-statement>    <true-statement>
        //                      JMP b
        //else
        //                      a:
        //  <false-statement>   <false-statement>
        //b:                    b:
        match(If);
        match('(');
        
        //表达式level为Assign
        //生成: <cond>
        expression(Assign);
        match(')');

        //生成: JZ  //JZ a
        *++text=JZ;
        b=++text;   //用于存放 JZ a 中操作数a的位置

        //生成: <true-statement>
        statement();
        if(token==Else){
            match(Else);

            //填入a的位置
            //text为<true-statement>最后一条指令
            //text+1和text+2为给JMP b预留空间
            //text+1为JMP，text+2为JMP b中的b
            *b=(int)(text+3);

            //生成: JMP  //JMP b
            *++text=JMP;
            b=++text;  //用于存放JMP b中操作数b的位置

            //生成: <false-statement>
            statement();
        }
        *b=(int)(text+1);  //放入b的位置
    }

    //while语句
    else if(token==While){
        //code              assembly
        //a:                a:
        //while (<cond>)    <cond>
        //                  JZ b
        //  <statement>     <statement>
        //                  JMP a
        //b:                b:

        match(While);
        a=text+1;           //记录a的位置
        match('(');
        expression(Assign);
        match(')');

        //生成: JZ  //JZ b
        *++text=JZ;
        b=++text;           //用于存放JZ b中操作数b的位置

        //生成: <statement>
        statement();

        //生成: JMP  //JMP a
        *++text=JMP;
        *++text=(int)a;     //放入a的位置
        *b=(int)(text+1);   //放入b的位置
    }

    //块语句 {statement}
    else if(token=='{'){
        match('{');

        //{} 中可能包含多个statement
        while(token!='}'){
            statement();
        }
        match('}');
    }

    //return语句
    else if(token==Return){
        match(Return);
        if(token!=';'){
            expression(Assign); //表达式level为Assign
        }
        match(';');

        //return意味着将要退出函数，用LEV来表示
        //生成: LEV
        *++text=LEV;
    }

    //空语句
    else if(token==';'){
        match(';');
    }

    //expression; ，如a=b;或函数调用func();
    else{
        expression(Assign);
        match(';');
    }
}

//function: enum_declaration
void enum_declaration(){
    //解析enum中的列表: enum [id] { a=1, b=2, ...... }
    //位置:                        | <-- here --> |
    int i;  //enum内变量的值，从0开始
    i=0;
    while(token!='}'){
        if(token!=Id){
            printf("%d: bad enum identifier %d\n", line, token);
            exit(-1);
        }
        next();

        //a=2型，即enum内变量有赋值行为
        if(token==Assign){
            next();
            if(token!=Num){
                printf("%d: bad enum initializer\n", line);
                exit(-1);
            }
            i=token_val;    //获取当前enum变量的值
            next();
        }

        current_id[Class]=Num;
        current_id[Type]=INT;
        current_id[Value]=i++; //若有值，则i被改变。若无值，则i从0开始

        //后面还有变量定义
        if(token==','){
            next();
        }
    }
}

//function: function_parameter
void function_parameter(){
    //type func_name ( type arg1, type arg2, ... ) {...}
    //位置:            | <-- here ------------> |

    int type;       //TODO: 啥?
    int params;     //TODO: 啥?
    params=0;

    while(token!=')'){
        type=INT;
        if(token==Int){
            match(Int);
        }
        else if(token==Char){
            match(Char);
            type=CHAR;
        }

        //指针类型
        while(token==Mul){
            match(Mul);
            type=type+PTR;
        }

        //参数名
        if(token!=Id){
            printf("%d: bad parameter declaration\n", line);
            exit(-1);
        }

        //如果为局部变量，则表示已存在同名局部变量
        if(current_id[Class]==Loc){
            printf("%d: duplicate parameter declaration\n", line);
            exit(-1);
        }

        match(Id);
        //存放局部变量
        //Value中存放的是参数的位置
        current_id[BClass]=current_id[Class]; current_id[Class]=Loc;
        current_id[BType]=current_id[Type]; current_id[Type]=type;
        current_id[BValue]=current_id[Value]; current_id[Value]=params++;

        //还有参数待解析
        if(token==','){
            match(',');
        }
    }
    index_of_bp=params+1;  //info.md中new_bp的位置
}

//function: function_body
void function_body(){
    //type func_name ( type arg1, type arg2, ... ) { ... }
    //位置:                                         |<-->|

    int pos_local;  //局部变量在栈上的位置
    int type;
    pos_local=index_of_bp;

    //局部变量定义
    while(token==Int || token==Char){
        basetype=(token==Int)?INT:CHAR;
        match(token);

        while(token!=';'){
            type=basetype;
            //指针类型
            while(token==Mul){
                match(Mul);
                type=type+PTR;
            }
            //没有找到变量名
            if(token!=Id){
                printf("%d: bad local declaration\n", line);
                exit(-1);
            }
            //变量重复定义
            if(current_id[Class]==Loc){
                printf("%d: duplicate local declaration\n", line);
                exit(-1);
            }
            match(Id);
            
            //存放局部变量
            //Value存放变量相对于new_bp的位置
            current_id[BClass]=current_id[Class];
            current_id[Class]=Loc;
            current_id[BType]=current_id[Type];
            current_id[Type]=type;
            current_id[BValue]=current_id[Value];
            current_id[Value]=++pos_local;

            //还有变量定义，如int a, b;
            if(token==','){
                match(',');
            }
        }
        match(';');
    }

    //为局部变量预留空间
    //ENT的作用: bp入栈，bp指向sp，sp预留空间用于存放局部变量
    //ENT的实现: *--sp=(int)bp; bp=sp; sp=sp-*pc++;
    //sp下移的位置由*pc指出，即下方的*++text=pos_local-index_of_bp;
    *++text=ENT;
    *++text=pos_local-index_of_bp;  //存放局部变量个数

    //statements
    while(token!='}'){
        statement();
    }

    //函数退出
    //LEV功能: 从bp中恢复sp，bp出栈，pc出栈
    //LEV实现: sp=bp; bp=(int*)*sp++; pc=(int*)*sp++;
    *++text=LEV;
}

//function: function_declaration
void function_declaration(){
    //type func_name ( type arg1, type arg2, ... ) {...}
    //位置:          | <-- here ---------------------> |
    
    match('(');
    function_parameter();
    match(')');
    match('{');
    function_body();
    //需要在 global_declaration中匹配，用于退出当前函数定义
    //match('}')
    
    //将被局部变量隐藏的全局变量复原
    current_id=symbols;
    while(current_id[Token]){
        //若当前变量是局部变量，则其对应的全局变量信息存放在Bxxx中
        if(current_id[Class]==Loc){
            current_id[Type]=current_id[BType];
            current_id[Class]=current_id[BClass];
            current_id[Value]=current_id[BValue];
        }

        //步进 IdSize，current_id指向下一个标识符信息首部
        current_id=current_id+IdSize;
    }
}

//function: global_declaration
void global_declaration(){
    int type;   //变量的实际类型
    int i;      //TODO: 未知

    basetype=INT;

    //解析enum
    if(token==Enum){
        //enum [id] {a=10, b=20, ... }
        match(Enum);
        if(token!='{'){
            match(Id);
        }
        if(token=='{'){
            match('{');
            enum_declaration();
            match('}');
        }
        match(';');
        return;
    }

    //解析类型信息
    if(token==Int){
        match(Int);
    }
    else if(token==Char){
        match(Char);
        basetype=Char;
    }
    //两者都不是，默认为int型

    //遇到 ';' 或 '}' 则解析完一次全局定义
    //int a; int* b; 是两次全局定义，分别在两次global_declaration中解析
    while(token!=';' && token!='}'){
        type=basetype;

        //多级指针
        while(token==Mul){
            match(Mul);
            type=type+PTR;
        }

        //没有匹配到Id，如int 3;
        if(token!=Id){
            printf("%d: bad glocal declaration\n", line);
            exit(-1);
        }

        //变量重复定义
        if(current_id[Class]){
            printf("%d: duplicate global declaration\n", line);
            exit(-1);
        }

        match(Id);

        //next()中current_id指向当前标识符信息首部
        //next()中只填了Name/Hash/Token，现在Type回填
        current_id[Type]=type;
        
        //函数定义
        if(token=='('){
            current_id[Class]=Fun;
            current_id[Value]=(int)(text+1);
            function_declaration(); //解析函数的参数列表和函数体
        }

        //普通变量定义
        else{
            current_id[Class]=Glo;
            //data预留4个空间给当前变量存放值
            //因为是变量声明，所以该地址还没有填入任何值
            current_id[Value]=(int)data;
            data=data+sizeof(int);
        }

        //int a, b, c;中的逗号，后续还是变量定义
        if(token==','){
            match(',');
        }
    }

    //使用完token后，还需要调用next，给其他地方使用
    next();
}

//program
void program(){
    next();

    //多次全局定义
    while(token>0){
        global_declaration();
    }
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
