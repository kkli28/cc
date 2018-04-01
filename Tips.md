
> 1. 实现变量在函数体内的任意位置定义

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

> 2. 程序的Debug信息输出格式

所有函数带有一个 string 参数，代表该函数的递归调用层次。最终的输出格式大致如下：

```
func1(): ...
    func1_1(): ....
    func1_2(): ....
func2(): ...
func3(): ...
    func3_1(): ....
        func3_1_1(): ...
        func3_1_2(): ...
...
```

通过输出信息的结构，可以看到函数调用栈的结构。

> 3. 每个逻辑类都下一个对应的测试用例

为保证每个逻辑类本身的正确性，需要完善的测试用例来保证其正确性。
//TODO: 测试用例写些什么？如何测？
