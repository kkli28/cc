#ifndef __GENERATOR_H__
#define __GENERATOR_H__

/*
�ļ��������﷨�Ƶ��ķ��룬���õݹ��½����﷨������������ʵ�ִ������ɡ�
*/

#include "stdafx.h"
#include "Lexer.h"

namespace kkli {

	/*
	EBNF�ķ�������

	����
	<program> = {<global_decl>}+
	
	ȫ�ֶ��壺
	<global_decl> = <enum_decl> | <var_decl> | <func_decl>
	
	enum���壺
	<enum_decl> = 'enum' '{' <id> [ '=' <num> ]
	              {',' <id> [ '=' <num> ]}+ '}' ';'

	�������壺
	<var_decl> = <type> {'*'}+ <id> {',' {'*'}+ <id>}+ ';'

	�������壺
	<func_decl> = <type> {'*'}+ <id> '(' <param_decl> ')' '{' <body_decl> '}'

	�����������壺
	<param_decl> = <null> | <type> {'*'}+ <id> {',' <type> {'*'}+ <id>}+

	//TODO: �Ƿ�֧�ֱ����洦���壬���Ǳ����ں������ײ�����
	�����嶨��1��
	<body_decl> = {<var_decl>}+ {<statement>}+
	�����嶨��2��
	<body_decl> = {<var_decl> | <statement>}+

	��䶨�壺
	<statement> = <if_stat> | <while_stat> | '{' <statement> '}'
	             | <expr> ';' | 'return' <expr> ';' | ';'

	if��䣺
	<if_stat> = 'if' '(' <expr> ')' <statement> ['else' <statement>]

	while��䣺
	<while_stat> = 'while' '(' <expr> ')' <statement>

	��ʶ����
	<id> = <lower_alpha> <id_tail> | <upper_alpha> <id_tail> | '_' <id_tail>

	��ʶ��β����
	<id_tail> = {<lower_alpha> | <upper_alpha> | <num> | '_'}+

	Сд��ĸ��
	<lower_alpha> = 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' 
	              | 'h' | 'i' | 'j' | 'k' | 'l' | 'm' | 'n'
			      | 'o' | 'p' | 'q' | 'r' | 's' | 't'
				  | 'u' | 'v' | 'w' | 'x' | 'y' | 'z'
	
	��д��ĸ��
	<upper_alpha> = 'A' | 'B' | 'C' | 'D' | 'E' | 'F' | 'G'
	              | 'H' | 'I' | 'J' | 'K' | 'L' | 'M' | 'N'
				  | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T'
				  | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z'

	���֣�
	<num> = '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'

	���ͣ�
	<type> = 'int' | 'char'

	�գ�
	<null> = 

	//����������ȼ����ӵ͵��ߣ���
	1. =
	2. ||
	3. &&
	4. < <= > >= != ==
	5. + =
	6. * / %
	7. ! - & * ++ --
	8. ++ -- (����)
	9. () (����)
	10. [] () (��������)

	���ʽ��
	<expr> = <assign_expr> | <null>

	���ȼ���С�� = �ı��ʽ��
	<assign_expr> = <or_expr> <assign_tail>
	<asign_tail> = '=' <orexpr> <assign_tail> | <null>

	���ȼ���С�� || �ı��ʽ��
	<or_expr> = <and_expr> <or_tail>
	<or_tail = '||' <and_expr> <or_tail> | <null>

	���ȼ���С�� && �ı��ʽ
	<and_expr> = <cmp_expr> <and_tail>
	<and_tail> = '&&' <cmp_expr> <and_tail> | <null>

	���ȼ���С�� �Ƚ�(< <= > >= != ==) �ı��ʽ��
	<cmp_expr> = <add_expr> <cmp_tail>
	<cmp_tail> = <cmps> <add_expr> <cmp_tail> | <null>
	<cmps> = '<' | '<=' | '>' | '>=' | '!=' | '=='

	���ȼ���С�� �Ӽ�(+ -) �ı��ʽ��
	<add_expr> = <mul_expr> <add_tail>
	<add_tail> = <adds> <mul_expr> <add_tail> | <null>
	<adds> = '+' | '-'

	���ȼ���С�� �˳�ģ(* / %) �ı��ʽ��
	<mul_expr> = <factor> <mul_tail>
	<mul_tail> = <muls> <factor> <mul_tail> | <null>
	<muls> = '*' | '/' | '%'

	���ȼ���С�� ����(! - & * ++ --) �ı��ʽ
	<factor> = <lops> <factor> | <value>
	<lops> = '!' | '-' | '~' | '&' | '*' | '++' | '--'

	���ȼ���С�� ֵ���ʽ �ı��ʽ��
	<value> = <elem> <rops>
	<rops> = '++' | '--'

	Ԫ�ر��ʽ��
	<elem> = <id> ['[' <expr> ']'] | '(' <expr> ')' | <constant>

	�������ʽ��
	<constant> = NUM | CHAR | STRING
	*/

	//========================================
	// Generator: ����������
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
