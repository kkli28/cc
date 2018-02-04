
> 对 i++/i-- 语句代码生成的分析

```C
//对应xc.c代码 806行 - 833行

//==================== 语句与生成的汇编示意 ====================
//假设 int i; ax中存放i的地址addr<i>，当前*text为LI
// statement            assembly
// -----------------------------
// i++                  PUSH    ;将addr<i>入栈 | stack: addr<i>
//                      LI      ;获取i的值
//                      PUSH    ;将i的值入栈   | stack: addr<i> i
//                      IMM     ;立即数1，既i的自增量
//                      1       ;若i为int*型，则为4 | ax: 1
//                      ADD     ;i的值+1      | stack: addr<i>  | ax: i+1
//                      SI      ;将ax中的值存入addr<i>所指内存中，既设置i=i+1
//                      PUSH    ;将i+1入栈 | stack: i+1
//                      IMM
//                      1       ;ax: 1
//                      SUB     ;将i+1减去1，ax存i原本的值 | stack: <null>
//                              ;ax: i (i原本的值)

//==================== 代码 ====================
//      int *a, *b;    //用于分支控制
//else if (token == Inc || token == Dec) {
// postfix inc(++) and dec(--)
// we will increase the value to the variable and decrease it
// on `ax` to get its original value.
c1.  if (*text == LI) {
c2.      *text = PUSH;
c3.      *++text = LI;
c4     }
c5.  else if (*text == LC) {
c6.      *text = PUSH;
c7.      *++text = LC;
c8.  }
c9.  else {
c10.     printf("%d: bad value in increment\n", line);
c11.     exit(-1);
c12. }

c13. *++text = PUSH;
c14. *++text = IMM;
c15. *++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);
c16. *++text = (token == Inc) ? ADD : SUB;
c17. *++text = (expr_type == CHAR) ? SC : SI;
c18. *++text = PUSH;
c19. *++text = IMM;
c20. *++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);
c21. *++text = (token == Inc) ? SUB : ADD;
c22. match(token);
//}

//==================== 分析 ====================
c1-c4/c5-c8: 根据指令类型，将ax中的值(变量的地址)入栈，并取变量的值放入ax中。图示：

//假设变量为 int i; 表达式类型为 i++;
//      | instruction |
        | PUSH        | ;addr<i> 入栈
        | LI          | ;取i的值放入ax

//      | stack   | ax       |
        | addr<i> | i        |

c9-c12: 在错误的类型上执行后置++/--。
c13-c15: 将i值入栈，并取立即数1。图示：

//假设变量为 int i; 表达式类型为 i++;
//      | instruction |
        | PUSH        | ;addr<i> 入栈
        | LI          | ;取i的值放入ax
        | PUSH        | ;将i的值入栈
        | IMM         | ;将立即数1放入ax
        | 1           | 

//      | stack   | ax       |
        | addr<i> | 1        |
        | i       |

c16: 将i的值加上1。图示：

//假设变量为 int i; 表达式类型为 i++;
//      | instruction |
        | PUSH        | ;addr<i> 入栈
        | LI          | ;取i的值放入ax
        | PUSH        | ;将i的值入栈
        | IMM         | ;将立即数1放入ax
        | 1           |
        | ADD         | ;i的值加1

//      | stack   | ax       |
        | addr<i> | i+1      |

c17-c18: 将i+1存入变量i中，并将ax中的值入栈。图示：

//假设变量为 int i; 表达式类型为 i++;
//      | instruction |
        | PUSH        | ;addr<i> 入栈
        | LI          | ;取i的值放入ax
        | PUSH        | ;将i的值入栈
        | IMM         | ;将立即数1放入ax
        | 1           |
        | ADD         | ;i的值加1
        | SI          | ;将ax的值i+1写入addr<i>所指内存中(更新变量i的值为i+1)
        | PUSH        | ;将ax的值i+1入栈

//      | stack   | ax       |
        | i+1     | i+1      | ;现在addr<i>的值为i+1

c19-c20: 取立即数1。图示：

//假设变量为 int i; 表达式类型为 i++;
//      | instruction |
        | PUSH        | ;addr<i> 入栈
        | LI          | ;取i的值放入ax
        | PUSH        | ;将i的值入栈
        | IMM         | ;将立即数1放入ax
        | 1           |
        | ADD         | ;i的值加1
        | SI          | ;将ax的值i+1写入addr<i>所指内存中(更新变量i的值为i+1)
        | PUSH        | ;将ax的值i+1入栈
        | IMM         | ;将立即数1放入ax
        | 1           |

//      | stack   | ax       |
        | i+1     | 1        |

c21: 将i的新值(stack中)减1，存放在ax中，供使用。图示：

//假设变量为 int i; 表达式类型为 i++;
//      | instruction |
        | PUSH        | ;addr<i> 入栈
        | LI          | ;取i的值放入ax
        | PUSH        | ;将i的值入栈
        | IMM         | ;将立即数1放入ax
        | 1           |
        | ADD         | ;i的值加1
        | SI          | ;将ax的值i+1写入addr<i>所指内存中(更新变量i的值为i+1)
        | PUSH        | ;将ax的值i+1入栈
        | IMM         | ;将立即数1放入ax
        | 1           |
        | SUB         | ;i+1减1，既将i的原值放入ax中，供使用

//      | stack   | ax       |
                  | i        |
```
