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

    /*
    标识类型，包括Num、Id、Char、While等
    */
    int token;

    /*
    hash值，用于快速查找是否有同名变量
    */
    int hash;

    /*
    标识符本身的字符串，如 int var=2; 中的var
    */
    int name;

    /*
    数据类型，分为三类：int、char、ptr，其中ptr需要与int和char叠加，形成一级或多级指针
    */
    int type;

    /*
    标识是什么类别的，如立即数、函数、系统内置、全局变量、局部变量
    */
    int class;

    /*
    变量的值，如 a=2; 则a的值为2
    */
    int Value;

    /*
    包括BToken、BHash、BName、BType、BClass、BValue，
    分别是对应的Token、Hash、Name等的缓存，当进入函数内部时，
    若定义了同名变量，则覆盖外部变量信息，将外部变量信息的XXX字段移入BXXX字段，并在
    原本的XXX字段中存入新值。则在使用时，直接使用其XXX字段的值即可。
    在函数返回时，若BXXX中有信息，则需要将其信息转移到XXX字段，以便恢复外部变量信息。
    因为只有XXX与BXXX，因此只有全局与局部两种作用域类型。
    */
    int BXXX;   //对应的缓存变量
    ...
};
```

```C++
在next()中识别到Id就填入信息，但是如果遇到如下的代码，是如何判断错误的呢？

int func() {
    a = 2;
}

在next()中，识别到a，则填入Token、Hash、Name，暂时不报错。函数体内，因为先处理
变量定义，再处理statement，则会跳过变量定义，因为没有int或char前导。
进入statement()时，判定为expression，则进入expression()。在expression()中，Token==Id的处理中，发现不是函数调用，也不是enum变量，则只是普通的变量。但是因为没有Loc或Glo信息（均为0），因此不是一个合法的变量，故报错：undefined variable。其Loc或Glo信息，需要在函数体内的变量定义模块填充。
```

> 代码生成

```
在global_declaration中，识别为函数后，其value值为int(text+1)为何要+1？
因为函数调用是将pc的值设置为函数首地址实现的，只需要函数本身的代码“完整”，就没关系，函数之间的代码有无空隙没关系。
```
