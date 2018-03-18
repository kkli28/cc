#ifndef __GENERATOR_H__
#define __GENERATOR_H__

/*
文件描述：语法制导的翻译，采用递归下降的语法分析技术，并实现代码生成。
*/

#include "stdafx.h"
#include "Lexer.h"

namespace kkli {

	/*
	EBNF文法描述：

	程序：
	<program> = {<global_decl>}+
	
	全局定义：
	<global_decl> = <enum_decl> | <var_decl> | <func_decl>
	
	enum定义：
	<enum_decl> = 'enum' '{' <id> [ '=' <num> ]
	              {',' <id> [ '=' <num> ]}+ '}' ';'

	变量定义：
	<var_decl> = <type> {'*'}+ <id> {',' {'*'}+ <id>}+ ';'

	函数定义：
	<func_decl> = <type> {'*'}+ <id> '(' <param_decl> ')' '{' <body_decl> '}'

	函数参数定义：
	<param_decl> = <null> | <type> {'*'}+ <id> {',' <type> {'*'}+ <id>}+

	//TODO: 是否支持变量随处定义，而非必须在函数体首部定义
	函数体定义1：
	<body_decl> = {<var_decl>}+ {<statement>}+
	函数体定义2：
	<body_decl> = {<var_decl> | <statement>}+

	语句定义：
	<statement> = <if_stat> | <while_stat> | '{' <statement> '}'
	             | <expr> ';' | 'return' <expr> ';' | ';'

	if语句：
	<if_stat> = 'if' '(' <expr> ')' <statement> ['else' <statement>]

	while语句：
	<while_stat> = 'while' '(' <expr> ')' <statement>

	标识符：
	<id> = <lower_alpha> <id_tail> | <upper_alpha> <id_tail> | '_' <id_tail>

	标识符尾部：
	<id_tail> = {<lower_alpha> | <upper_alpha> | <num> | '_'}+

	小写字母：
	<lower_alpha> = 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' 
	              | 'h' | 'i' | 'j' | 'k' | 'l' | 'm' | 'n'
			      | 'o' | 'p' | 'q' | 'r' | 's' | 't'
				  | 'u' | 'v' | 'w' | 'x' | 'y' | 'z'
	
	大写字母：
	<upper_alpha> = 'A' | 'B' | 'C' | 'D' | 'E' | 'F' | 'G'
	              | 'H' | 'I' | 'J' | 'K' | 'L' | 'M' | 'N'
				  | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T'
				  | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z'

	数字：
	<num> = '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'

	类型：
	<type> = 'int' | 'char'

	空：
	<null> = 

	//运算符的优先级（从低到高）：
	1. =
	2. ||
	3. &&
	4. < <= > >= != ==
	5. + =
	6. * / %
	7. ! - & * ++ --
	8. ++ -- (后置)
	9. () (括号)
	10. [] () (函数调用)

	表达式：
	<expr> = <assign_expr> | <null>

	优先级不小于 = 的表达式：
	<assign_expr> = <or_expr> <assign_tail>
	<asign_tail> = '=' <orexpr> <assign_tail> | <null>

	优先级不小于 || 的表达式：
	<or_expr> = <and_expr> <or_tail>
	<or_tail = '||' <and_expr> <or_tail> | <null>

	优先级不小于 && 的表达式
	<and_expr> = <cmp_expr> <and_tail>
	<and_tail> = '&&' <cmp_expr> <and_tail> | <null>

	优先级不小于 比较(< <= > >= != ==) 的表达式：
	<cmp_expr> = <add_expr> <cmp_tail>
	<cmp_tail> = <cmps> <add_expr> <cmp_tail> | <null>
	<cmps> = '<' | '<=' | '>' | '>=' | '!=' | '=='

	优先级不小于 加减(+ -) 的表达式：
	<add_expr> = <mul_expr> <add_tail>
	<add_tail> = <adds> <mul_expr> <add_tail> | <null>
	<adds> = '+' | '-'

	优先级不小于 乘除模(* / %) 的表达式：
	<mul_expr> = <factor> <mul_tail>
	<mul_tail> = <muls> <factor> <mul_tail> | <null>
	<muls> = '*' | '/' | '%'

	优先级不小于 因子(! - & * ++ --) 的表达式
	<factor> = <lops> <factor> | <value>
	<lops> = '!' | '-' | '~' | '&' | '*' | '++' | '--'

	优先级不小于 值表达式 的表达式：
	<value> = <elem> <rops>
	<rops> = '++' | '--'

	元素表达式：
	<elem> = <id> ['[' <expr> ']'] | '(' <expr> ')' | <constant>

	常量表达式：
	<constant> = NUM | CHAR | STRING
	*/

	//========================================
	// Generator: 代码生成器
	//========================================
	class Generator {
	private:
		Lexer lexer;
		SymbolTable* table;
		VirtualMachine* vm;

	public:
		Generator(std::string sourceFile);
		void run();
	};
}

#endif
