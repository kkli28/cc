# Token

为了简单且便于管理，本编译器只支持两种作用域，即全局作用域和局部作用域。

Token的定义如下：

```
class Token {
public:
    int type;        //类型
    int klass;       //种类
    string name;     //名字
    int dataType;    //数据类型
    int value;       //值
    int hash;        //hash值
    std::vector<int> args;  //函数的参数类型

    int Bklass;      //种类的备份
    int BdataType;   //数据类型的备份
    int Bvalue;      //值的部分
}
```

类型分为很多种，如标识符(ID)，关键字(CHAR、IF)、运算符(GT、AND)、符号(LPAREN、COMMA)等。

种类分为五种，即数字(NUMBER)、用户定义函数(FUNC)、系统函数(SYS_FUNC)、全局变量(GLOBAL)、局部变量(LOCAL)。

只支持2种基本类型，整形(INT_TYPE)和字符型(CHAR_TYPE)，以及它们的一级和多级指针，其中指针的表示由
整形或字符串型加上多个指针型(PTR_TYPE)来实现。如CHAR_TYPE为0，INT_TYPE为1，PTR_TYPE为2，
则数字7表示 `PTR_TYPE + PTR_TYPE + PTR_TYPE + INT_TYPE`，即 `int***`。

全局的名字，其信息存储在 `[type, klass, name, dataType, value, hash]` 中，如果在函数内又
定义了相同的名字，则将全局符号的 `[klass, dataType, value]` 移动到 `[Bklass, BdataType, Bvalue]` 中，实现对全局名字的隐藏。退出函数时，也需要将全局的信息恢复。
