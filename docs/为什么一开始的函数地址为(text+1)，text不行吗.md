> 为什么一开始的函数地址就是(text+1)？*text这个空间不就没有东西了吗？

对的，*text位置不放任何东西。因为函数从main函数开始执行， 只要其他函数的地址能够获得，就不用
管代码是否连续。只要每个函数自身的代码完整（有正确的退出语句），则不会导致问题。

对于global_decl函数里对函数定义的识别，并对其设置代码段位置：

```C++
{
		if (token == '(') {
			current_id[Class] = Fun;
			current_id[Value] = (int)(text + 1);
			function_declaration();
		}
		else {
			// variable declaration
			current_id[Class] = Glo; // global variable
			current_id[Value] = (int)data;
			data = data + sizeof(int);
		}
}
```

语句：`current_id[Value] = (int)(text+1)` 就是将函数地址设置为text+1的位置。因为在其他地方，向代码段写入指令的方式类似：`*++text=ADD` ，因此默认 `*text` 为上一条指令。由于只要函数的地址设置正确，并有正确的返回，则text中间一部分空着也没有任何关系，因为调用函数的时候，是以地址跳转的方式进行的，只需要知道函数的首指令地址，则除了函数本身的指令块需要连续外，各个函数之间不需要。如：

```
text:
0x00 | ADD |
0x01 |     |  //空的
0x02 |     |  //空的
0x03 | LEA |  // <-- func1 函数的地址
0x04 | ... |
0x05 | ... |
0x06 | RET |  // <-- func1 函数退出
0x07 | ... |
0x08 |     |  //空的
0x09 | ADD |  // <-- func2 函数的地址
0x0A | ... |

//调用函数func1是，是将pc的值设置为0x03来进行的，所以func1前方即使有空位，不影响函数的调用，只要保证函数内部没有空位即可。
```
