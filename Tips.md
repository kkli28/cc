> 实现变量在函数体内的任意位置定义

函数体内代码结构大致如下：
```
//1. 处理变量定义，统计变量数量count
while(INT | CHAR) { var_decl(); }

//2. 写入进入函数的指令
vm->addInst(I_ENT);
vm->addInstData(count);

//2处修改方式如下：
vm->addInst(I_ENT);
int* varCountAddr = vm->getNextTextPos();  //记录写入变量数量的代码段位置
vm->addInstData(0);  //占据该位置
enter_func_body();   //进入函数体，同时继续对变量定义进行解析，增加count值
*varCountAddr = count;

//3. 离开函数
vm->addInst(I_LEV);
```
