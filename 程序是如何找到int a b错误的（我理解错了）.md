> 程序是如何找到 int a b; 的错误的？

程序分析变量定义的代码如下：

```C++
    if (token == Int) {
        match(Int);
    }
    else if (token == Char) {
        match(Char);
        basetype = CHAR;
    }
    while (token != ';' && token != '}') {
        type = basetype;
        while (token == Mul) {
            match(Mul);
            type = type + PTR;
        }

        if (token != Id) {
            printf("%d: bad global declaration\n", line);
            exit(-1);
        }
        if (current_id[Class]) {
            printf("%d: duplicate global declaration\n", line);
            exit(-1);
        }
        match(Id);
        current_id[Type] = type;

        if (token == '(') {
            current_id[Class] = Fun;
            current_id[Value] = (int)(text + 1);
            function_declaration();
        } else {
            current_id[Class] = Glo;
            current_id[Value] = (int)data;
            data = data + sizeof(int);
        }
        if (token == ',') {
            match(',');
        }
    }
```

对于定义 `int a b;`，首先 `match(int)`，在match中有 `next()` 调用，则 `current_id` 指向待写入变量a信息的位置。然后 `match(ID)`，将a匹配后，调用 `next()`，因为后续为b，则 `current_id` 指向了待写入变量b信息的位置，然后在该写入变量b信息的位置写入了a的信息，其Class域为Glo。因为后续不是逗号，则不 `match(',')`。因为b为ID，因此再次进入循环，`match(ID)` 后，因为后续为分号，不会改变 `current_id` 的位置，因此 `current_id[Class]` 位置的值为Glo，导致变量重定义。

作者本意是，在 `match(int)` 之后，`current_id` 会指向写入变量a信息的地方，此时判断Class域，若非零则标识该变量重复定义。而我举的这个例子，刚好导致需要很麻烦地绕才能讲清楚为什么错误类型是重定义。
