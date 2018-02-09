> 1. 设计

整体步骤：
```
1. 结构设计：整体结构。
2. 虚拟机设计：指令集的设计，以及虚拟机的实现。
```

解释器与编译器的区别仅在代码生成阶段有区别，如词法分析、语法分析。

解释器需要自己实现虚拟机和指令集，可帮助我们了解计算机的工作原理。

词法分析：将字符串转化为内部表示结构

语法分析：将词法分析得到的标记流生成一棵语法树

目标代码生成：将语法树转化成目标代码。

> 2. 虚拟机

代码段：存放代码

数据段：存放已初始化的数据。

栈：用于处理函数调用相关的数据，如调用帧或局部变量。

堆：程序动态分配的内存。

```
//各个段在内存中的布局（理论上）

+----------------+
|  stack  |      |  high address
|  ...    v      |
|                |
|                |
|                |
|  ...    ^      |
|  heap   |      |
+----------------+
|  bss segment   |
+----------------+
|  data segment  |
+----------------+
|  text segment  |  low address
+----------------+

//本编译器的实际布局
//各个段独立存在于内存中，互不相关
//heap段内存使用虚拟机分配(malloc)的内存

+----------------+
|  stack  |      |  high address
|  ...    v      |
|                |
|                |
*----------------+

+----------------+
|  text   |      |  high address
|  ...    v      |
|                |
|                |
*----------------+

+----------------+
|  data   |      |  high address
|  ...    v      |
|                |
|                |
*----------------+
```

用户需要分配内部才能时，使用MSET指令可让虚拟机调用malloc函数，从而使用虚拟机分配的内存。

整个编译器（包括虚拟机）假设内存地址为32位，`int*`和int类型都是32位。

整个系统只支持三种不同的类型：char型、int型和指针类型，其中指针类型可以是int指针或char指针，或者指针的指针。

PC程序计数器：存放的是内存地址，指向下一条要执行的指令。

SP栈指针寄存器：指向栈顶，入栈时SP减小。

BP基址指针寄存器：指向栈的某个位置，函数调用时会使用。

AX通用寄存器：存放指令执行后的结果。实际为栈顶缓存。

本虚拟机中只有一个寄存器，识别参数类型时值还是地址比较困难，因此将
MOV指令拆分为5个指令：

```
IMM <num>   ;将<num>放入寄存器ax中
LC          ;将ax中的值作为char型变量的地址，并取其值放到ax中
LI          ;将ax中的值作为int型变量的地址，并取得其值放到ax中
SC          ;将栈顶(*sp)的值作为char型变量的地址 ，将ax中的值存入其中。
SI          ;将栈顶(*sp)的值作为int型变量的地址，将ax中的值存入。
```

函数调用指令CALL不能用JMP取代。因为JMP将丢失原pc的信息，而函数调用
需要在函数返回时恢复现场。

函数调用约定：
```
函数参数顺序入栈(C语言标准为逆序入栈)
函数调用的返回结果放在ax寄存器中。
```

函数调用与返回：
```
//函数调用
                ;函数调用参数入栈
CALL <addr>     ;*--sp=(int)(pc+1); pc=(int*)*pc;
                ;pc入栈，pc指向函数入口

ENT             ;*--sp=(int)bp; bp=sp; sp=sp-*pc++;
                ;bp入栈，bp指向sp，sp预留空间用于存放局部变量
...

//函数返回     
LEV             ;sp=bp; bp=(int*)*sp++; pc=(int*)*sp++;
                ;从bp中恢复sp，bp出栈，pc出栈

ADJ  <num>      ;sp=sp+*pc++;
                ;函数调用参数出栈


...
```

从上可知函数调用和函数返回是逆过程。分别有 `CALL/ENT` 和 `LEV/ADJ` 来实现。

函数调用中参数在栈中的排布：
```
//函数定义
function(int arg1, int arg2, int arg3){
    int v1, v2;
    ... //statements
}

//函数调用
function(1, 2, 3);

|   ...    |  high address
+----------+
|  arg1    |  new_bp + 4
+----------+
|  arg2    |  new_bp + 3
+----------+
|  arg3    |  new_bp + 2
+----------+
| ret addr |  new_bp + 1
+----------+
|  old_bp  |  <- new bp
+----------+
| local v1 |  new_bp - 1
+----------+
| local v2 |  new_bp - 2
+----------+
|   ...    |  low address
```

如果需要取得第一个参数，需要得到new_bp+4，但是ADD指令无法操作ax，
因此提供了 `LEA <offset>` 指令，用于获取相对于bp的栈上的位置。

对于所有的运算符，都提供对应的指令，且所有指令都是二元的。第一个参数
放在栈顶，第二个参数放在ax中。计算后会将栈顶的参数出栈，结果放在ax寄存器中。因此计算后两个参数都无法再（立即）取得。

代码中，ADJ及以上的指令是带参数的，其余的不带参数。例如：
```
//带参数指令
//text和text+1是一个指令(IMM 10)的两个部分
IMM         <-- text
10          <-- text+1

//不带参数的指令
//text和text+1是不同的两个指令
ADD         <-- text
PUSH        <-- text+1
```

> 3. 词法分析

不一次性将所有源代码转换成标记流是因为：
```
1. 字符串转换成标记流有时是有状态的，即与代码上下文是有关系的。
2. 保存所有标记流没有意义，浪费空间。
```
所以提供一个函数用于返回下一个标记。即每次要用标记时再通过词法分析技术获取下一个标记。

支持的标记：
```
Num:数字  Fun:函数  Sys:系统内置，如read函数
Glo:全局变量  Loc:局部变量  Id:标识符  

//关键字
Char Else  Enum  If  Int Return  Sizeof  While

//操作符
Assign  Cond(可判定真假的表达式)  Lor  Lan  Or  Xor  And
Eq  Ne  Lt  Gt  Le  Ge  Shl  Shr  Add  Sub  Mul  Div  Mod
Inc  Dec  Brak(左方括号[)
```

`next()` 函数用于获取下一个标记。其中的while循环用于跳过空白符以及
不认识的字符。

不支持的内容：
```
1. 直接跳过以 '#' 开始的行（跳过预处理）
2. 不支持块注释(/*xxx*/)，只支持行注释(//)
3. 不支持除 '\n' 以外的转义字符，只将其看做普通字符，
   如'\x'看做字符x。
```

对于内置关键字，在程序开始时先将他们添加到符号表。在解析时遇到时，
因为符号表已有相关项目，则直接返回其Token域即可。

> 4. 递归下降(没啥可讲的)

> 5. 变量定义

EBNF定义：
```
program = {global_declaration} +

global_declaration = 
    enum_declaration
    | variable_declaration
    | function_definition

enum_declaration =
    'enum' [id] '{' id ['=' num] {',' id ['=' num]} + '}' ';'

variable_declaration =
    type {'*'}+ id {',' {'*'}+ id }+ ';'

function_definition =
    type {'*'}+ id '(' parameter_declaration ')'
    '{' body_definition '}'

parameter_declaration = {type {'*'}+ id}+

body_definition = {variable_declaration}+ {statement}+

statement =
    if_statement | while_statement
    | '{' statement '}' | 'return' expression ';'
    | expression ';' | ';'

if_statement =
    'if' '(' expression ')' statement
    ['else' statement]

while_statement =
    'while' '(' expression ')' statement
```

> 6. 函数定义

当函数被调用时栈的状态:
```
//函数定义
//int demo(int arg1, int arg2){
//    int local1;
//    int local2;
//    ...    
//}

//栈的状态
|   ......   |  high address
+------------+
|  arg1      |  new_bp + 3
+------------+
|  arg2      |  new_bp + 2
+------------+
|  ret addr  |  new_bp + 1
+------------+
|  old_bp    |  new_bp
+------------+
|  local1    |  new_bp - 1
+------------+
|  local2    |  new_bp - 2
+------------+
|   ......   |  low address
```

在函数内使用参数和局部变量是通过 `new_bp` 指针与对应位移进行的。

> 7. 语句

语句就是表达式加上末尾的分号，即: `statement = expression ';'`

本编译器支持六种语句：
```
if (...) <statement> [else <statement>]
while (...) <statement>
{ statement }
return expression;
<empty statement>
expression;  //表达时后面带分号
```

if语句伪代码：
```
//statement             assembly
  if(<cond>)            <cond>
                        JZ a
    <true-statement>    <true-statement>
                        JMP b
  else
  a:
    <flase-statement>   <false-statement>
  b:                    b:
    
```

> 8. 表达式

表达式即可以求值的语言元素的组合。

表达式分为一元表达式和二元表达式。一元表达式其操作符属于该元素，
二元表达式中，前后的操作符将会争抢元素。一元运算符优先级高于二元运算符。
```
一元表达式: +a
+操作符属于a，+获得唯一的操作数，直接计算+a

二元表达式: a+b*c
+操作符和*操作符将相互争抢b。
+争抢b形成a+b，*争抢b形成b*c
因为*优先级高于+，因此*抢到b，最终形成a+(b*c)
*获得两个操作数，开始计算(b*c)=e1，此时为a+e1。
+获得两个操作数，开始计算a+e1=e2，则表达式结果为e2。
```

字符串代码生成的详细介绍：
```
//"ab" 型字符串
1. 开始解析字符串，token为 "，token_val为abc在内存中的首地址
   生成代码 IMM token_val
|   |
| " |  <- token
| a |  <- token_val
| b |
| " | 
|   |

2. match(token)，并将字符串相关代码生成后，token为末尾的"之后的位置，即下一个词法单元
|   |
| " |
| a |
| b |
| " |
|   |  <- token

//"ab" "cd" 型字符串
1. 解析"ab"同上面方，生成了 IMM token_val 代码。此时token为第二个字符串的第一个"
|   |
| " |
| a |
| b |
| " |
|   |  <- 空白被跳过
| " |  <- token
| c |  <- token_val
| d |
| " |
|   |

2. 因为"ab"和"cd"在data中连续存放，因此不需要再 IMM token_val，只需match(token)即可，此时token为"cd"后的词法单元
```

字符串代码生成阶段data赋值语句分析:
```
data=(char*) (((int)data + sizeof(int)) & (-sizeof(int)))

>> 1. (int)data+sizeof(int)
将data后移1个int位置，因为data初始化为0，因此后移一个位置
便可以在字符串末尾添加结束字符 '\0'

>> 2. (-sizeof(int))
在32位机器中，sizeof(int)为4，后八位二进制为 0000 0100
-4的二进制为 1111 1100

>> 3. (...) & (...)
因为data在next中是每次存储1个字符，因此可能出现末尾不是
刚好填满4的整数倍空间，而CPU读取内存是有对齐要求的，
char地址1字节对其，int地址需要4字节对齐。
因此为了避免data中下一个位置存储int时没有根据字节对其存储，则
会导致读取数据错误。因此 data & 1111 1100 将data对齐到4字节。

图示如下：
|   |  <- 1111 1010  data+sizeof(int)的位置
|   |  <- 1111 1001
|   |  <- 1111 1000  [int对齐位置] (...) & (...) 后对齐的位置
| 0 |  <- 1111 0111  此处没有放置任何字符，为0，即字符串结束符
| g |  <- 1111 0110  data原本位置
| f |  <- 1111 0101
| e |  <- 1111 0100  [int对齐位置]
| d |  <- 1111 0011
| c |  <- 1111 0010
| b |  <- 1111 0001
| a |  <- 1111 0000  [int对齐位置]
```
