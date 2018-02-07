> 对 if-else 语句代码生成的分析

```C
//对应xc.c代码 876行 - 910行

//==================== 语句与生成的汇编示意 ====================
// statement                assembly
// -------------------------------------
// if (...)                  <cond>
//                           JZ a
//      <if-statement>       <if-statement>
// else                      JMP b
//                           a:
//      <else-statement>     <else-statement>
//                           b:

//==================== 代码 ====================
//     int *a, *b;    //用于分支控制
c1.    *++text = JZ;
c2.    b = ++text;
c3.    statement();    //if-statement
c4.    if (token == Else) {
c5         match(Else);
c6.        // emit code for JMP B
c7.        *b = (int)(text + 3);
c8.        *++text = JMP;
c9.        b = ++text;
c10.       statement(); //else-statement
c11    }
c12.   *b = (int)(text + 1);

//==================== 分析 ====================
c1: 当前 text 指向最后一次生成的代码，++text 指向新位置，并生成 JZ 指令
c2: b 指向再下一个位置，用于写入 JZ 指令的操作数(要跳转的地址)，图示：

//      |addr|  asm  |
        |1   |  xxx  | <-- text
        |2   |  JZ   | <-- ++text
  b --> |3   |       | //等待填入跳转地址

c3: 解析 <if-statement> 语句，并在 b 所指位置下方生成对应指令
c7: b 获取当前 text 所指下方第 3 个位置的值，填入 b 所指地址，图示：

//      |addr|  asm  |
        |1   |  xxx  |
        |2   |  JZ   | // 2-3: 不满足if条件，JZ a
  b --> |3   |  14   | // 填入跳转地址 text+3 : 14
        |4   |  xxx  | // 4-11: <if-statement> 生成的指令
        |5   |  xxx  |
        |... |  xxx  |
        |11  |  xxx  | <-- text
        |12  |       | // 12-13: 无条件跳转，JMP b
        |13  |       |
        |14  |       | <-- text+3

c8: 当前 text 指向 <if-statement> 生成指令的最后一个指令/数据，++text 以填入 JMP 指令
9: b 指向 JMP 指令后一个位置，用以填入 JMP 指令的操作数(跳转地址)，图示：

//      |addr|  asm  |
        |1   |  xxx  |
        |2   |  JZ   | // 2-3: 不满足if条件，JZ a
        |3   |  14   |
        |4   |  xxx  | // 4-11: <if-statement> 生成的指令
        |5   |  xxx  |
        |... |  xxx  |
        |11  |  xxx  | <-- text
        |12  |  JMP  | <-- ++text
 b -->  |13  |       | // 等待填入跳转地址
        |14  |       |

c10: 解析 <else-statement> 语句，并在 b 所指位置下方生成对应指令
c12: 当前 text 指向 <else-statement> 生成的最后一个指令/数据，text+1 为JMP 将要跳转的位置。图示：

//      |addr|  asm  |
        |1   |  xxx  |
        |2   |  JZ   | // 2-3: 不满足if条件，JZ a
        |3   |  14   |
        |4   |  xxx  | // 4-11: <if-statement> 生成的指令
        |5   |  xxx  |
        |... |  xxx  |
        |11  |  xxx  |
        |12  |  JMP  |
 b -->  |13  |  23   | //填入跳转地址 text+1 : 23
        |14  |  xxx  | //14-22: <else-statement> 生成的指令
        |... |  xxx  |
        |22  |  xxx  | <-- text
        |23  |       | <-- text+1
```