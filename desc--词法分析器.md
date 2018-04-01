# 词法分析

为了实现对内置关键字以及内置函数的支持，在读取源文件之前，首先将
这些符号插入符号表，填入相应的信息。

支持的关键字有：char, else, enum, if, int, return, sizeof, while, void

支持的函数有：printf, malloc, exit, main（程序的入口）。

向符号表插入内置符号的代码如下：

```C++
//构建字符串，用next函数进行处理
source = "char else enum if int return sizeof while printf malloc exit void main";
source.push_back(eof);

SymbolTable* table = SymbolTable::getInstance();
int type = CHAR;
while (type <= WHILE) {

    //写入 type, name, hash 信息
	next();

    //改写 type 为对应的内置类型
	table->getCurrentToken().type = type;  //改写type类型为内置类型
	++type;
}

type = I_PRTF;
while (type <= I_EXIT) {

    //写入 type, name, hash 信息
	next();
	Token& tk = table->getCurrentToken();

    //完善 klass, dataType, value 信息
	tk.klass = SYS_FUNC;
	tk.dataType = INT;
	tk.value = type++;
}

//将void类型看做char类型
next();
table->getCurrentToken().type = CHAR;

//记录main函数的Token，便于解析后设置pc的值
next();next();
table->setMainToken(&(table->getCurrentToken()));
```

对源代码扫描，通过 `next()` 返回下一个词法单元的信息 `<type, value>`。

各种类型词法元素的分析：

> 换行符

增加行号。

> 宏 / 文件包含

不支持，直接跳过。

> 标识符

标识符在识别的过程中，会记录其每个字符的hash值，便于之后快速查找符号表
（通过比较int值而非多个字符来减少查询时间）。且名字会存入Token.name中。
若符号表中存在该标识符，则设置符号表中当前Token为此Token，然后返回。
若符号表中不存在该标识符，则需要向符号表中写入其信息 `[type, name, hash]`。
因为内置符号在解析前就已写入符号表，因此现在的标识符肯定是用户自定义的，其
type域为ID。

> 数字

支持八进制、十进制和十六进制，其值保存在tokenInfo.value域中。

> 注释

支持行注释和块注释。

> 字符

仅支持转义字符 `'\n'`，其他的如 `'\a'` 就看做字符a。字符的类型也是NUM。

> 字符串

字符串的各个字符会写入到虚拟机的 data段，其类型为STRING，值为其在 data段中的开始位置。

> 操作符

如 `+, ++, =, ==, !` 等，没啥可讲的。

> 空白符

跳过空白符，如空格，制表符等。

> 结束符

如果是文件结束符，则返回 END。
