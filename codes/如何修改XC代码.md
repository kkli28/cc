# 对XC代码进行修改

> 符号表

```C++
使用class实现，重新给每个字段命名。示例：

//标记类型
enum {
    Num = 128, Fun, Sys, Glo, Loc,
    Id, Char //等等
};

class SymbolTable {
public:
    int token;  //标记
    int hash;   //hash值
    int name;   //标识符本身的字符串
    int type;   //类型
    int class;  //类别
    int Value;  //值
    int Bxxx;   //对应的缓存变量
    ...
};
```

```C++
在next()中识别到Id就填入信息，但是如果遇到如下的代码，是如何判断错误的呢？

int func() {
    a = 2;
}

在next()中，识别到a，则填入Token、Hash、Name，暂时不报错。函数体内，先处理
变量定义，再处理statement，则会跳过变量定义，因为没有int或char前导。
进入statement()时，判定为expression，则进入expression()。在expression()中，Token==Id的处理中，发现不是函数调用，也不是enum变量，则只是普通的变量。但是因为没有Loc或Glo信息（均为0），因此不是一个合法的变量，故报错：undefined variable。其Loc或Glo信息，需要在函数体内的变量定义模块填充。
```
