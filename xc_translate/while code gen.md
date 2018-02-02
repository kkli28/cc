> 对 while 语句代码生成的分析

```C
//对应xc.c代码 行 - 行

//==================== 语句与生成的汇编示意 ====================
// statement            assembly
// -----------------------------
// a:                   a:
// while (<cond>)       <cond>
//                      JZ b
//     <statement>      <statement>
//                      JMP a
// b:                   b:

//==================== 代码 ====================
//      int *a, *b;    //用于分支控制
c1.     match(While);
c2      a=text+1;
        
c3.     match('(');
c4.     expression(Assign);
c5.     match(')');

c6.     *++text=JZ;
c7.     b=++text;
        
c8.     statement();
c9.     *++text=JMP;
c10.    *++text=(int)a;
c11.    *b=(int)(text+1);

//==================== 分析 ====================
c2: a 记录 while 语句将要生成指令的开始位置
c4: 填入 expression 的指令
c6: 当前 text 指向表达式生成的最后一条指令，++text 以填入 JZ 指令
c7: b 指向 JZ 指令后一个位置，用以填入 JZ 指令的操作数(跳转地址)，图示：

//      |addr|  asm  |
  a --> |1   |  xxx  | // 1-7: expression 的指令
        |... |  xxx  |
        |7   |  xxx  | <-- text
        |8   |  JZ   | <-- ++text
  b --> |9   |       | // 待填入跳转地址

c8: 在 b 之后填入 statement 生成的指令
c9: 当前 text 指向 statement 生成的最后一条指令，++text 以填入 JMP 指令
c10: ++text 以填入 JMP 指令的操作数
c11: text+1 既 JZ 指令的跳转地址。图示：

//      |addr|  asm  |
  a --> |1   |  xxx  | // 1-7: expression 的指令
        |... |  xxx  |
        |7   |  xxx  |
        |8   |  JZ   |
  b --> |9   |  19   |
        |10  |  xxx  | //10-16: statement 的指令
        |... |  xxx  |
        |16  |  xxx  | <-- text
        |17  |  JMP  | <-- ++text
        |18  |  1    | <-- ++text // 跳转到 a
        |19  |       | // JZ 指令的跳转地址
```
