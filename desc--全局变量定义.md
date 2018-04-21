全局变量定义只支持常量或已定义的变量，不支持复杂表达式。如：

```
//支持
int a = 1;
int b = -1;
int c = a;
int d = -a;

char c1 = 'A';
char c2 = -c1;
char c3 = -'A';  //Warning

//不支持
int a = 1 + 1；
int b = a + 1;
char c1 = 'a' + 'b';
```
