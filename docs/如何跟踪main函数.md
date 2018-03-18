> 代码中是最终如何定位到main函数并执行之的呢？

有代码如下：

```C++
{
	src = "char else enum if int return sizeof while "
		"open read close printf malloc memset memcmp exit void main";

	// add keywords to symbol table
	i = Char;
	while (i <= While) {
		next();
		current_id[Token] = i++;
	}

	// add library to symbol table
	i = OPEN;
	while (i <= EXIT) {
		next();
		current_id[Class] = Sys;
		current_id[Type] = INT;
		current_id[Value] = i++;
	}

	next(); current_id[Token] = Char; // handle void type
	next(); idmain = current_id; // keep track of main
}
```

一开始，将内置函数、关键字写入符号表内。
将void类型看做为char，并将main也添加到符号表，则main在符号表中的信息如下：

```
Token: ID
Klass: SYS
DataType: INT
Value: 0
Hash: 0
Name: "main"
```

然后用idmain来记住main在符号表中的起始位置。接着执行program()对程序进行分析，在program()中会检测到main函数的定义，并设置其代码位置，即Value域的值会被正确地填写。此时再将pc赋值为idmain[Value]，则将pc设置为指向main函数首条指令。运行虚拟机，则程序就从main函数开始执行了。

```C++
{
	program();

	if (!(pc = (int *)idmain[Value])) {
		printf("main() not defined\n");
		return -1;
	}
}
```
