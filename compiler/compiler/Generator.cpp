#include "stdafx.h"
#include "Generator.h"

//构造函数
kkli::Generator::Generator(std::string sourceFile)
	: lexer(sourceFile, "") {
	table = SymbolTable::getInstance();
	vm = VirtualMachine::getInstance();
}

//匹配Token
void kkli::Generator::match(int type, std::string format) {
	DEBUG_GENERATOR("Generator::match(" + Token::getTokenTypeName(type) + ")", format);

	if (tokenInfo.first == type) {
		tokenInfo = lexer.next(FORMAT(format));
	}
	else {
		throw Error(lexer.getLine(), "expected token [" + Token::getTokenTypeName(type) + "]");
	}
}

void kkli::Generator::gen(std::string format) {
	DEBUG_GENERATOR("Generator::run()", format);

	tokenInfo = lexer.next(FORMAT(format));
	while (tokenInfo.first != END) {
		global_decl(FORMAT(format));
	}
}

//全局定义
void kkli::Generator::global_decl(std::string format) {
	/*
	文法：
	<global_decl> = <enum_decl> | <var_decl> | <func_decl>
	*/
	DEBUG_GENERATOR("Generator::global_decl()", format);

	int type;  //全局定义的类型
	baseType = INT_TYPE;  //基本类型默认为INT_TYPE

	//enum常量定义
	while (tokenInfo.first == ENUM) {
		enum_decl(FORMAT(format));
	}

	//全局变量或函数定义的类型
	if (tokenInfo.first == INT) {
		match(INT, FORMAT(format));
	}
	else if (tokenInfo.first == CHAR) {
		match(CHAR, FORMAT(format));
		baseType = CHAR_TYPE;
	}

	while (tokenInfo.first != SEMICON && tokenInfo.first != RBRACE) {
		type = baseType;

		//多级指针，如 int *******x;
		while (tokenInfo.first == MUL) {
			match(MUL, FORMAT(format));
			type = type + PTR_TYPE;
		}

		DEBUG_GENERATOR("type = " + Token::getDataTypeName(type), FORMAT(format));
		if (tokenInfo.first != ID) {
			throw Error(lexer.getLine(), "expected token [ID]");
		}

		if (table->getCurrentToken(FORMAT(format)).klass != ERROR) {
			throw Error(lexer.getLine(), "duplicate global declaration, [id] = " +
				table->getCurrentToken(FORMAT(format)).name);
		}

		match(ID, FORMAT(format));

		//函数定义
		if (tokenInfo.first == LPAREN) {
			Token& tk = table->getCurrentToken(FORMAT(format));
			tk.klass = FUNC;
			tk.dataType = type;
			tk.value = reinterpret_cast<int>(vm->getNextTextPos());
			func_decl(FORMAT(format));
		}

		//全局变量声明之后必须为分隔符（逗号或分号）
		else if (tokenInfo.first != COMMA && tokenInfo.first != SEMICON) {
			throw Error(lexer.getLine(), "wrong variables declaration.");
		}

		//Tips: 处理后续符号为ASSIGN的情况，将初始值写入tk.value，以实现变量声明时初始化。
		else {
			Token& tk = table->getCurrentToken(FORMAT(format));
			tk.klass = GLOBAL;
			tk.dataType = type;
			tk.value = reinterpret_cast<int>(vm->getNextDataPos(INT_TYPE, FORMAT(format)));
			vm->addDataInt(0, FORMAT(format));  //添加一个0，为这个变量占据写入值的位置，避免这个位置被其他变量使用。
		}
		
		if (tokenInfo.first == COMMA) {
			match(COMMA, FORMAT(format));
		}
	}

	DEBUG_GENERATOR(std::string("now tokenInfo.first = ") + (tokenInfo.first == SEMICON ? "SEMICON" : "RBRACE"), FORMAT(format));
	tokenInfo = lexer.next(FORMAT(format));
}

//enum定义
void kkli::Generator::enum_decl(std::string format) {
	/*
    <enum_decl> = 'enum' '{' <id> [ '=' <num> ] 
	              {',' <id> [ '=' <num> ]}+ '}' ';'
	*/
	DEBUG_GENERATOR("Generator::enum_decl()", format);

	int varIndex = 0;  //enum常量的位置
	int varValue = 0;  //enum常量的值
	match(ENUM, FORMAT(format));
	match(LBRACE, FORMAT(format));
	while (tokenInfo.first != RBRACE) {
		++varIndex;

		match(ID, FORMAT(format));

		//enum { a = 0, ... } 中的等于
		if (tokenInfo.first == ASSIGN) {
			match(ASSIGN, FORMAT(format));

			//允许正负数 +1, -1 等
			bool negtive = false;
			if (tokenInfo.first == SUB) {
				match(SUB, FORMAT(format));
				negtive = true;
			}
			else if (tokenInfo.first == ADD) {
				match(ADD, FORMAT(format));
			}
			if (tokenInfo.first != NUM) {
				throw Error(lexer.getLine(), "expected token [NUM]");
			}
			varValue = negtive ? -tokenInfo.second : tokenInfo.second;
			match(NUM, FORMAT(format));
		}
		DEBUG_GENERATOR("[index] = " + std::to_string(varIndex)
			+ "  value = " + std::to_string(varValue), FORMAT(format));

		Token& tk = table->getCurrentToken(FORMAT(format));
		tk.klass = NUMBER;
		tk.dataType = INT_TYPE;
		tk.value = varValue++;
		
		//{ a = 0, b = 1 } 中的逗号
		if (tokenInfo.first == COMMA) {
			match(COMMA, FORMAT(format));

			//逗号后必须跟enum常量
			if (tokenInfo.first != ID) {
				throw Error(lexer.getLine(), "expected token [ID]");
			}
		}
	}
	match(RBRACE, FORMAT(format));
	match(SEMICON, FORMAT(format));
}

//函数定义
void kkli::Generator::func_decl(std::string format) {
	DEBUG_GENERATOR("Generator::func_del()", format);

	match(LPAREN, FORMAT(format));
	func_param(FORMAT(format));
	match(RPAREN, FORMAT(format));
	match(LBRACE, FORMAT(format));
	func_body(FORMAT(format));
	
	DEBUG_GENERATOR_SYMBOL("\n[======== before restore ========]" + table->getSymbolTableInfo(), FORMAT(format));

	//恢复全局变量
	std::vector<Token>& tb = table->getTable();
	for (auto& tk : tb) {
		if (tk.klass == LOCAL) {
			tk.restoreInfo(FORMAT(format));
		}
	}

	DEBUG_GENERATOR_SYMBOL("\n[======== after restore ========] " + table->getSymbolTableInfo(), FORMAT(format));
}

//函数参数定义
void kkli::Generator::func_param(std::string format) {
	DEBUG_GENERATOR("Generator::func_param()", format);

	int dataType;
	int params = 0;
	while (tokenInfo.first != RPAREN) {
		dataType = INT_TYPE;
		if (tokenInfo.first == INT) {
			match(INT, FORMAT(format));
		}
		else if (tokenInfo.first == CHAR) {
			match(CHAR, FORMAT(format));
			dataType = CHAR_TYPE;
		}

		//指针类型
		while (tokenInfo.first == MUL) {
			match(MUL, FORMAT(format));
			dataType = dataType + PTR_TYPE;
		}
		DEBUG_GENERATOR("[param type] = " + Token::getDataTypeName(dataType), FORMAT(format));

		if (tokenInfo.first != ID) {
			throw Error(lexer.getLine(), "bad parameter declaration.");
		}
		if (table->getCurrentToken(FORMAT(format)).klass == LOCAL) {
			throw Error(lexer.getLine(), "duplicate parameter declaration.");
		}

		match(ID, FORMAT(format));

		if (tokenInfo.first != COMMA && tokenInfo.first != RPAREN) {
			throw Error(lexer.getLine(), "wrong param declaration!");
		}

		DEBUG_GENERATOR_SYMBOL("\n[======== before backup ========] " + table->getSymbolTableInfo(), "");

		//备份全局变量信息，并填入局部变量信息
		Token& tk = table->getCurrentToken(FORMAT(format));
		tk.saveInfo(FORMAT(format));
		tk.klass = LOCAL;
		tk.dataType = dataType;
		tk.value = params++;
		
		DEBUG_GENERATOR_SYMBOL("\n[======== after backup ========] " + table->getSymbolTableInfo(), "");
		
		if (tokenInfo.first == COMMA) {
			match(COMMA, FORMAT(format));
		}
	}

	indexOfBP = params + 1;

	DEBUG_GENERATOR("[index of bp]: " + std::to_string(indexOfBP), FORMAT(format));
}

//函数体
void kkli::Generator::func_body(std::string format) {
	DEBUG_GENERATOR("Generator::func_body()", format);

	int variableIndex = indexOfBP;  //局部变量在栈上相对于bp的位置

	DEBUG_GENERATOR("[start variable decl]", FORMAT(format));

	//局部变量定义
	while (tokenInfo.first == INT || tokenInfo.first == CHAR) {
		baseType = (tokenInfo.first == INT) ? INT_TYPE : CHAR_TYPE;
		match(tokenInfo.first, FORMAT(format));

		while (tokenInfo.first != SEMICON) {
			int dataType = baseType;

			//多级指针
			while (tokenInfo.first == MUL) {
				match(MUL, FORMAT(format));
				dataType += PTR_TYPE;
			}

			if (tokenInfo.first != ID) {
				throw Error(lexer.getLine(), "bad local declaration.");
			}

			if (table->getCurrentToken(FORMAT(format)).klass == LOCAL) {
				throw Error(lexer.getLine(), "duplicate local declaration.");
			}

			match(ID, FORMAT(format));

			if (tokenInfo.first != COMMA && tokenInfo.first != SEMICON) {
				throw Error(lexer.getLine(), "bad local declaration.");
			}

			DEBUG_GENERATOR_SYMBOL("\n[======== before backup ========] " + table->getSymbolTableInfo(), "");

			//存储局部变量
			Token& currToken = table->getCurrentToken(FORMAT(format));
			currToken.saveInfo(FORMAT(format));
			currToken.klass = LOCAL;
			currToken.dataType = dataType;
			currToken.value = ++variableIndex;

			DEBUG_GENERATOR_SYMBOL("\n[======== before backup ========] " + table->getSymbolTableInfo(), "");

			if (tokenInfo.first == COMMA) {
				match(COMMA, FORMAT(format));
			}
		}

		match(SEMICON, FORMAT(format));
	}
	DEBUG_GENERATOR("[end variable decl]", format);

	//在栈上留下保存变量所需的空间，并进入函数
	vm->addInst(I_ENT, FORMAT(format));
	vm->addInstData(variableIndex - indexOfBP, FORMAT(format));  //Tips: 这里可以放入0，然后一直记录变量count，最后回填，实现变量在函数体中间定义。

	DEBUG_GENERATOR("[start statement decl]", FORMAT(format));

	//语句定义
	while (tokenInfo.first != RBRACE) {
		statement(FORMAT(format));
	}

	DEBUG_GENERATOR("[end statement decl]", FORMAT(format));

	vm->addInst(I_LEV, FORMAT(format));
}

//语句
void kkli::Generator::statement(std::string format) {
	//1. if(expr) statement [else statement]
	//2. while(expr) statement
	//3. { statement }
	//4. return expr;
	//5. expr;  //expr可空

	DEBUG_GENERATOR("Generator::statement()", format);
	
	//if语句
	if (tokenInfo.first == IF) {
		DEBUG_GENERATOR("[if]", FORMAT(format));

		match(IF, FORMAT(format));
		match(LPAREN, FORMAT(format));
		expression(ASSIGN, FORMAT(format));
		match(RPAREN, FORMAT(format));

		int *branch;  //用于填写语句的分支跳转地址

		vm->addInst(I_JZ, FORMAT(format));
		branch = vm->getNextTextPos();
		vm->addInstData(0, FORMAT(format));  //占据一个位置，用以写入 I_JZ 的跳转位置
		statement(FORMAT(format));
		if (tokenInfo.first == ELSE) {
			DEBUG_GENERATOR("[else]", FORMAT(format));

			match(ELSE, FORMAT(format));
			*branch = int(vm->getNextTextPos() + 2);
			vm->addInst(I_JMP, FORMAT(format));
			branch = vm->getNextTextPos();
			vm->addInstData(0, FORMAT(format));  //占据一个位置
			statement(FORMAT(format));
		}
		*branch = int(vm->getNextTextPos());
	}

	//while语句
	else if (tokenInfo.first == WHILE) {
		DEBUG_GENERATOR("[while]", FORMAT(format));

		int *branchA, *branchB;  //用于填写语句的分支跳转地址
		match(WHILE, FORMAT(format));
		branchA = vm->getNextTextPos();
		match(LPAREN, FORMAT(format));
		expression(ASSIGN, FORMAT(format));
		match(RPAREN, FORMAT(format));

		vm->addInst(I_JZ, FORMAT(format));
		branchB = vm->getNextTextPos();
		vm->addInstData(0, FORMAT(format));

		statement(FORMAT(format));
		vm->addInst(I_JMP, FORMAT(format));
		vm->addInstData(int(branchA), FORMAT(format));
		*branchB = int(vm->getNextTextPos());
	}

	//{ statement }
	else if (tokenInfo.first == LBRACE) {
		DEBUG_GENERATOR("[{statement}]", FORMAT(format));

		match(LBRACE, FORMAT(format));
		while (tokenInfo.first != RBRACE) {
			statement(FORMAT(format));
		}
		match(RBRACE, FORMAT(format));
	}

	//return语句
	else if (tokenInfo.first == RETURN) {
		match(RETURN, FORMAT(format));
		if (tokenInfo.first != SEMICON) {
			DEBUG_GENERATOR("[return expr]", FORMAT(format));
			expression(ASSIGN, FORMAT(format));
		}
		else {
			DEBUG_GENERATOR("[return]", FORMAT(format));
		}
		match(SEMICON, FORMAT(format));
		vm->addInst(I_LEV, FORMAT(format));
	}

	//expr
	else {
		DEBUG_GENERATOR("[expr]", FORMAT(format));

		expression(ASSIGN, FORMAT(format));
		match(SEMICON, FORMAT(format));
	}
}

//表达式
void kkli::Generator::expression(int priority, std::string format) {
	DEBUG_GENERATOR("Generator::expression(" + std::to_string(priority) + ")", format);

	//一元表达式
	DEBUG_GENERATOR("[unary expression]", FORMAT(format));
	if (tokenInfo.first == ERROR) {
		throw Error(lexer.getLine(), "bad identifier ERROR.");
	}

	else if (tokenInfo.first == END) {
		throw Error(lexer.getLine(), "unexpected token EOF of expression.");
	}

	else if (tokenInfo.first == NUM) {
		vm->addInst(I_IMM, FORMAT(format));
		vm->addInstData(tokenInfo.second, FORMAT(format));
		DEBUG_GENERATOR("[NUM] " + std::to_string(tokenInfo.second), FORMAT(format));

		exprType = INT_TYPE;
		match(NUM, FORMAT(format));
	}

	else if (tokenInfo.first == STRING) {
		vm->addInst(I_IMM, FORMAT(format));
		vm->addInstData(tokenInfo.second, FORMAT(format));

		exprType = PTR_TYPE;
		DEBUG_GENERATOR(std::string("[STRING] ") + reinterpret_cast<char*>(tokenInfo.second), FORMAT(format));

		match(STRING, FORMAT(format));
	}

	else if (tokenInfo.first == SIZEOF) {
		match(SIZEOF, FORMAT(format));
		match(LPAREN, FORMAT(format));
		exprType = INT_TYPE;
		if (tokenInfo.first == INT) {
			match(INT, FORMAT(format));
		}
		else if (tokenInfo.first == CHAR) {
			match(CHAR, FORMAT(format));
			exprType = CHAR_TYPE;
		}
		while (tokenInfo.first == MUL) {
			match(MUL, FORMAT(format));
			exprType = exprType + PTR_TYPE;
		}

		DEBUG_GENERATOR("[SIZEOF] " + Token::getDataTypeName(exprType), FORMAT(format));
		match(RPAREN, FORMAT(format));
		vm->addInst(I_IMM, FORMAT(format));
		vm->addInstData(exprType == CHAR_TYPE ? 1 : 4, FORMAT(format));
		exprType = INT;
	}

	else if (tokenInfo.first == ID) {
		//三种可能：函数调用、enum变量、全局/局部变量
		DEBUG_GENERATOR("[ID]", FORMAT(format));

		match(ID, FORMAT(format));
		Token& current = table->getCurrentToken(FORMAT(format));

		//函数调用
		if (tokenInfo.first == LPAREN) {
			match(LPAREN, FORMAT(format));
			int args = 0;
			while (tokenInfo.first != RPAREN) {
				expression(ASSIGN, FORMAT(format));
				vm->addInst(I_PUSH, FORMAT(format));
				++args;
				if (tokenInfo.first == COMMA) {
					match(COMMA, FORMAT(format));
				}
			}
			match(RPAREN, FORMAT(format));

			//系统函数
			if (current.klass == SYS_FUNC) {
				DEBUG_GENERATOR("[SYS_FUNC]", FORMAT(format));
				vm->addInst(current.value, FORMAT(format));
			}

			//用户自定义函数
			else if (current.klass == FUNC) {
				DEBUG_GENERATOR("[CUSTOM_FUNC]", FORMAT(format));
				vm->addInst(I_CALL, FORMAT(format));
				vm->addInstData(current.value, FORMAT(format));
			}
			else {
				throw Error(lexer.getLine(), "bad function call.");
			}

			//清除栈上参数
			if (args > 0) {
				DEBUG_GENERATOR("[elim " + std::to_string(args) + " params on stack]", FORMAT(format));
				vm->addInst(I_ADJ, FORMAT(format));
				vm->addInstData(args, FORMAT(format));
			}
			exprType = current.dataType;
		}

		//enum变量
		else if (current.klass == NUMBER) {
			DEBUG_GENERATOR("[enum variable]", FORMAT(format));
			vm->addInst(I_IMM, FORMAT(format));
			vm->addInstData(current.value, FORMAT(format));
			exprType = INT_TYPE;
		}

		//普通变量
		else {
			if (current.klass == LOCAL) {
				DEBUG_GENERATOR("[local variable]", FORMAT(format));
				vm->addInst(I_LEA, FORMAT(format));
				vm->addInstData(indexOfBP - current.value, FORMAT(format));
			}
			else if (current.klass == GLOBAL) {
				DEBUG_GENERATOR("[global variable]", FORMAT(format));
				vm->addInst(I_IMM, FORMAT(format));
				vm->addInstData(current.value, FORMAT(format));
			}
			else {
				throw Error(lexer.getLine(), "undefined variable.");
			}

			//默认操作是加载值（右值），如果后续是赋值，则抹掉I_LC/I_LI，只用其地址
			exprType = current.dataType;
			vm->addInst(exprType == CHAR_TYPE ? I_LC : I_LI, FORMAT(format));
		}
	}

	else if (tokenInfo.first == LPAREN) {
		match(LPAREN, FORMAT(format));

		//强制类型转换
		if (tokenInfo.first == INT || tokenInfo.first == CHAR) {
			int castType = (tokenInfo.first == CHAR) ? CHAR_TYPE : INT_TYPE;
			match(tokenInfo.first, FORMAT(format));
			while (tokenInfo.first == MUL) {
				match(MUL, FORMAT(format));
				castType += PTR_TYPE;
			}
			match(RPAREN, FORMAT(format));
			DEBUG_GENERATOR("[cast] " + Token::getDataTypeName(castType), FORMAT(format));
			expression(INC, FORMAT(format));  //需要优先级INC（高一级）
			exprType = castType;
		}

		//括号表达式
		else {
			DEBUG_GENERATOR("[(expr)]", FORMAT(format));
			expression(ASSIGN, FORMAT(format));
			match(RPAREN, FORMAT(format));
		}
	}

	else if (tokenInfo.first == MUL) {
		DEBUG_GENERATOR("[MUL]", FORMAT(format));
		//*a，即取某个地址的值
		match(MUL, FORMAT(format));
		expression(INC, FORMAT(format));
		if (exprType >= PTR_TYPE) {
			exprType -= PTR_TYPE;
		}
		else {
			throw Error(lexer.getLine(), "bad dereference.");
		}

		//除了取一个char外，取int和取地址都是取int（默认int和指针字节数相同）
		vm->addInst(exprType == CHAR_TYPE ? I_LC : I_LI, FORMAT(format));
	}

	else if (tokenInfo.first == AND) {
		//取地址
		DEBUG_GENERATOR("[AND]", FORMAT(format));
		match(AND, FORMAT(format));
		expression(INC, FORMAT(format));
		int inst = vm->getTopInst(FORMAT(format));
		if (inst == I_LC || inst == I_LI) {
			vm->deleteTopInst(FORMAT(format));
		}
		else {
			throw Error(lexer.getLine(), "bad address of.");
		}

		exprType = exprType + PTR_TYPE;
		DEBUG_GENERATOR("exprType = " + Token::getDataTypeName(exprType), FORMAT(format));
	}

	else if (tokenInfo.first == NOT) {
		DEBUG_GENERATOR("[NOT]", FORMAT(format));
		match(NOT, FORMAT(format));
		expression(INC, FORMAT(format));

		//!expr 等价于 expr == 0
		vm->addInst(I_PUSH, FORMAT(format));
		vm->addInst(I_IMM, FORMAT(format));
		vm->addInstData(0, FORMAT(format));
		vm->addInst(I_EQ, FORMAT(format));

		exprType = INT_TYPE;
	}

	else if (tokenInfo.first == TILDE) {
		DEBUG_GENERATOR("[TILDE]", FORMAT(format));
		match(TILDE, FORMAT(format));
		expression(INC, FORMAT(format));

		//~a 等价于 a^(-1)
		vm->addInst(I_PUSH, FORMAT(format));
		vm->addInst(I_IMM, FORMAT(format));
		vm->addInstData(-1, FORMAT(format));
		vm->addInst(I_XOR, FORMAT(format));

		exprType = INT_TYPE;
	}

	else if (tokenInfo.first == ADD) {
		DEBUG_GENERATOR("[ADD]", FORMAT(format));
		//+a，啥也不做
		match(ADD, FORMAT(format));
		expression(INC, FORMAT(format));
		exprType = INT_TYPE;
	}

	else if (tokenInfo.first == SUB) {
		DEBUG_GENERATOR("[SUB]", FORMAT(format));
		//-a
		match(SUB, FORMAT(format));
		if (tokenInfo.first == NUM) {
			vm->addInst(I_IMM, FORMAT(format));
			vm->addInstData(-tokenInfo.second, FORMAT(format));
			match(NUM, FORMAT(format));
		}
		else {
			//-a 等价于 (-1)*a
			vm->addInst(I_IMM, FORMAT(format));
			vm->addInstData(-1, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(INC, FORMAT(format));
			vm->addInst(I_MUL, FORMAT(format));
		}

		exprType = INT_TYPE;
	}

	else if (tokenInfo.first == INC || tokenInfo.first == DEC) {
		DEBUG_GENERATOR("[INC/DEC]", FORMAT(format));

		int tk = tokenInfo.first;
		match(tk, FORMAT(format));
		expression(INC, FORMAT(format));
		if (vm->getTopInst(FORMAT(format)) == I_LC) {
			vm->addInst(I_PUSH, FORMAT(format));
			vm->addInst(I_LC, FORMAT(format));
		}
		else if (vm->getTopInst(FORMAT(format)) == I_LI) {
			vm->addInst(I_PUSH, FORMAT(format));
			vm->addInst(I_LI, FORMAT(format));
		}
		else {
			throw Error(lexer.getLine(), "bad lvalue of pre-increment.");
		}
		vm->addInst(I_PUSH, FORMAT(format));
		vm->addInst(I_IMM, FORMAT(format));
		vm->addInstData(exprType > PTR_TYPE ? 4 : 1, FORMAT(format));
		vm->addInst(tk == INC ? I_ADD : I_SUB, FORMAT(format));
		vm->addInst(exprType == CHAR_TYPE ? I_SC : I_SI, FORMAT(format));
	}

	else {
		throw Error(lexer.getLine(), "bad expression.");
	}

	//二元表达式
	DEBUG_GENERATOR("[binary expression]", FORMAT(format));

	while (tokenInfo.first >= priority) {
		if (tokenInfo.first == ASSIGN) {
			DEBUG_GENERATOR("[ASSIGN]", FORMAT(format));

			int tempType = exprType;
			match(ASSIGN, FORMAT(format));
			if (vm->getTopInst(FORMAT(format)) == I_LI || vm->getTopInst(FORMAT(format)) == I_LC) {
				vm->deleteTopInst(FORMAT(format));
				vm->addInst(I_PUSH, FORMAT(format));
			}
			else {
				throw Error(lexer.getLine(), "bad lvalue in assignment.");
			}
			expression(ASSIGN, FORMAT(format));
			exprType = tempType;
			vm->addInst(exprType == CHAR_TYPE ? I_SC : I_SI, FORMAT(format));
		}

		else if (tokenInfo.first == COND) {
			//expr ? a : b
			DEBUG_GENERATOR("[COND]", FORMAT(format));

			match(COND, FORMAT(format));
			vm->addInst(I_JZ, FORMAT(format));
			int* addr = vm->getNextTextPos();
			vm->addInstData(0, FORMAT(format));      //占据一个位置用于写入跳转地址
			expression(ASSIGN, FORMAT(format));
			if (tokenInfo.first == COLON) {
				match(COLON, FORMAT(format));
			}
			else {
				throw Error(lexer.getLine(), "missing colon in conditional.");
			}
			*addr = reinterpret_cast<int>(vm->getNextTextPos() + 2);
			vm->addInst(I_JMP, FORMAT(format));
			addr = vm->getNextTextPos();
			vm->addInstData(0, FORMAT(format));     //占据一个位置用于写入跳转地址
			expression(COND, FORMAT(format));
			*addr = reinterpret_cast<int>(vm->getNextTextPos());
		}

		else if (tokenInfo.first == LOR) {
			// ||
			DEBUG_GENERATOR("[LOR]", FORMAT(format));

			match(LOR, FORMAT(format));
			vm->addInst(I_JNZ, FORMAT(format));
			int* addr = vm->getNextTextPos();
			vm->addInstData(0, FORMAT(format));
			expression(LAN, FORMAT(format));
			*addr = reinterpret_cast<int>(vm->getNextTextPos());
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == LAN) {
			// &&
			DEBUG_GENERATOR("[LAN]", FORMAT(format));

			match(LAN, FORMAT(format));
			vm->addInst(I_JZ, FORMAT(format));
			int* addr = vm->getNextTextPos();
			vm->addInstData(0, FORMAT(format));
			expression(OR, FORMAT(format));
			*addr = reinterpret_cast<int>(vm->getNextTextPos());
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == OR) {
			// |
			DEBUG_GENERATOR("[OR]", FORMAT(format));

			match(OR, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(XOR, FORMAT(format));
			vm->addInst(I_OR, FORMAT(format));
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == XOR) {
			// ^
			DEBUG_GENERATOR("[XOR]", FORMAT(format));

			match(XOR, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(AND, FORMAT(format));
			vm->addInst(I_XOR, FORMAT(format));
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == AND) {
			// &
			DEBUG_GENERATOR("[AND]", FORMAT(format));

			match(AND, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(EQ, FORMAT(format));
			vm->addInst(I_AND, FORMAT(format));
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == EQ) {
			// ==
			DEBUG_GENERATOR("[EQ]", FORMAT(format));

			match(EQ, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(NE, FORMAT(format));
			vm->addInst(I_EQ, FORMAT(format));
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == NE) {
			//!=
			DEBUG_GENERATOR("[NE]", FORMAT(format));

			match(NE, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(LT, FORMAT(format));
			vm->addInst(I_NE, FORMAT(format));
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == LT) {
			// <
			DEBUG_GENERATOR("[LT]", FORMAT(format));

			match(LT, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(SHL, FORMAT(format));
			vm->addInst(I_LT, FORMAT(format));
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == GT) {
			// >
			DEBUG_GENERATOR("[GT]", FORMAT(format));

			match(GT, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(SHL, FORMAT(format));
			vm->addInst(I_GT, FORMAT(format));
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == LE) {
			// <=
			DEBUG_GENERATOR("[LE]", FORMAT(format));

			match(LE, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(SHL, FORMAT(format));
			vm->addInst(I_LE, FORMAT(format));
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == GE) {
			// >=
			DEBUG_GENERATOR("[GE]", FORMAT(format));

			vm->addInst(I_PUSH, FORMAT(format));
			expression(SHL, FORMAT(format));
			vm->addInst(I_GE, FORMAT(format));
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == SHL) {
			// <<
			DEBUG_GENERATOR("[SHL]", FORMAT(format));

			match(SHL, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(ADD, FORMAT(format));
			vm->addInst(I_SHL, FORMAT(format));
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == SHR) {
			// >>
			DEBUG_GENERATOR("[SHR]", FORMAT(format));

			match(SHR, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(ADD, FORMAT(format));
			vm->addInst(I_SHR, FORMAT(format));
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == ADD) {
			// a + b
			DEBUG_GENERATOR("[ADD]", FORMAT(format));

			int tempType = exprType;
			match(ADD, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(MUL, FORMAT(format));
			exprType = tempType;  //结果类型以左操作数为准
			//问题：当 'a' + 1000 时会溢出吗？因为char型最大255


			//非char*型指针，则加数乘以4再加到指针上
			if (exprType > PTR_TYPE) {
				vm->addInst(I_PUSH, FORMAT(format));
				vm->addInst(I_IMM, FORMAT(format));
				vm->addInstData(4, FORMAT(format));
				vm->addInst(MUL, FORMAT(format));
			}
			vm->addInst(I_ADD, FORMAT(format));
		}

		else if (tokenInfo.first == SUB) {
			// a - b
			DEBUG_GENERATOR("[SUB]", FORMAT(format));

			match(SUB, FORMAT(format));
			int tempType = exprType;
			vm->addInst(I_PUSH, FORMAT(format));
			expression(MUL, FORMAT(format));

			//指针相减 p1 - p2
			if (tempType > PTR_TYPE && tempType == exprType) {
				DEBUG_GENERATOR("[SUB ptr - ptr]", FORMAT(format));

				vm->addInst(I_SUB, FORMAT(format));
				vm->addInst(I_PUSH, FORMAT(format));
				vm->addInst(I_IMM, FORMAT(format));
				vm->addInstData(4, FORMAT(format));
				vm->addInst(I_DIV, FORMAT(format));
				exprType = INT_TYPE;
			}

			//指针移动 p1 - 2
			else if (tempType > PTR_TYPE) {
				DEBUG_GENERATOR("[SUB ptr - var]", FORMAT(format));

				vm->addInst(I_PUSH, FORMAT(format));
				vm->addInst(I_IMM, FORMAT(format));
				vm->addInstData(4, FORMAT(format));
				vm->addInst(I_MUL, FORMAT(format));
				vm->addInst(MUL, FORMAT(format));
				exprType = tempType;
			}

			//普通变量相减 a - b
			else {
				DEBUG_GENERATOR("[SUB var - var]", FORMAT(format));

				vm->addInst(I_SUB, FORMAT(format));
				exprType = tempType;
			}
		}

		else if (tokenInfo.first == MUL) {
			// a * b
			DEBUG_GENERATOR("[MUL]", FORMAT(format));

			int tempType = exprType;
			match(MUL, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(INC, FORMAT(format));
			vm->addInst(I_MUL, FORMAT(format));
			exprType = tempType;
		}

		else if (tokenInfo.first == DIV) {
			// a / b
			DEBUG_GENERATOR("[DIV]", FORMAT(format));

			int tempType = exprType;
			match(DIV, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(INC, FORMAT(format));
			vm->addInst(I_DIV, FORMAT(format));
			exprType = tempType;
		}

		else if (tokenInfo.first == MOD) {
			// a % b
			DEBUG_GENERATOR("[MOD]", FORMAT(format));

			int tempType = exprType;
			match(MOD, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(INC, FORMAT(format));
			vm->addInst(I_MOD, FORMAT(format));
			exprType = tempType;
		}

		else if (tokenInfo.first == INC || tokenInfo.first == DEC) {
			// a++
			DEBUG_GENERATOR("[INC]", FORMAT(format));

			if (vm->getTopInst(FORMAT(format)) == I_LI) {
				vm->deleteTopInst(FORMAT(format));
				vm->addInst(I_PUSH, FORMAT(format));
				vm->addInst(I_LI, FORMAT(format));
			}
			else if (vm->getTopInst(FORMAT(format)) == I_LC) {
				vm->deleteTopInst(FORMAT(format));
				vm->addInst(I_PUSH, FORMAT(format));
				vm->addInst(I_LC, FORMAT(format));
			}
			else {
				throw Error(lexer.getLine(), "bad value in increment.");
			}

			//先++a，存储，再用a-1参与计算
			vm->addInst(I_PUSH, FORMAT(format));
			vm->addInst(I_IMM, FORMAT(format));
			vm->addInstData(exprType > PTR_TYPE ? 4 : 1, FORMAT(format));
			vm->addInst(tokenInfo.first == INC ? I_ADD : I_SUB, FORMAT(format));
			vm->addInst(exprType == CHAR_TYPE ? I_SC : I_SI, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			vm->addInst(I_IMM, FORMAT(format));
			vm->addInst(exprType > PTR_TYPE ? 4 : 1, FORMAT(format));
			vm->addInst(tokenInfo.first == INC ? I_SUB : I_ADD, FORMAT(format));
			match(INC, FORMAT(format));
		}

		else if (tokenInfo.first == LBRACK) {
			// a[1]
			DEBUG_GENERATOR("[LBRACK]", FORMAT(format));

			int tempType = exprType;
			match(LBRACK, FORMAT(format));
			vm->addInst(I_PUSH, FORMAT(format));
			expression(ASSIGN, FORMAT(format));
			match(RBRACK, FORMAT(format));

			//非char*型指针
			if (tempType > PTR_TYPE) {
				vm->addInst(I_PUSH, FORMAT(format));
				vm->addInst(I_IMM, FORMAT(format));
				vm->addInstData(4, FORMAT(format));
				vm->addInst(I_MUL, FORMAT(format));
			}
			//对变量执行[]操作，错误
			else if (tempType < PTR_TYPE) {
				throw Error(lexer.getLine(), "pointer type expected.");
			}

			exprType = tempType - PTR_TYPE;
			vm->addInst(I_ADD, FORMAT(format));
			vm->addInst(exprType == CHAR ? I_LC : I_LI, FORMAT(format));
		}

		else {
			throw Error(lexer.getLine(), "compiler error, token = "
				+ Token::getTokenTypeName(tokenInfo.first));
		}
	}
}
