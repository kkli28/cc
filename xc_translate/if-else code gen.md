> 对 if-else 语句代码生成的分析

```C
//对应xc.c代码 876行 - 910行

//==================== 语句与生成的汇编示意 ====================
// statement                assembly
// -------------------------------------
//a1. if (...)              <cond>
//a2.                       JZ a
//a3.      <statement>      <statement>
//a4. else                  JMP b
//a5.                       a:
//a6.      <statement>      <statement>
//a7.                       b:

//==================== 代码 ====================
//      int *a, *b;    //用于分支控制
b1.     *++text = JZ;
b2.     b = ++text;
b3.     statement();
b4.     if (token == Else) {
b5          match(Else);
b6.         // emit code for JMP B
b7.         *b = (int)(text + 3);
b8.         *++text = JMP;
b9.         b = ++text;
b10.            statement();
b11     }
b12.    *b = (int)(text + 1);

//==================== 分析 ====================
b1: 当前 text 指向最后一次生成的代码，++text 指向新位置，并生成 JZ 指令
b2: b 指向再下一个位置，用于写入 JZ 指令将要跳转的地址，图示：

//      |addr|   asm  |
        |1   |   xxx  | <-- text
        |2   |   JZ   | <-- ++text
  b --> |3   |        |

b3: 解析 statement 语句，并在 b 所指位置下方生成对应汇编代码
b7: 获取当前 text 所指下方第 3 个位置的值，填入 b 所指地址
//TODO: 
```