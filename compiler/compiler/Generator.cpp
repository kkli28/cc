#include "stdafx.h"
#include "Generator.h"

//构造函数
kkli::Generator::Generator(std::string sourceFile)
	: lexer(sourceFile) {
	table = SymbolTable::getInstance();
	vm = VirtualMachine::getInstance();
}

//匹配Token
void kkli::Generator::match(int type) {
	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::match(" + Token::getTokenTypeName(type) + ")");
	}

	if (tokenInfo.first == type) {
		tokenInfo = lexer.next();
	}
	else {
		throw Error(lexer.getLine(), "expected token [" + Token::getTokenTypeName(type) + "]");
	}
}

void kkli::Generator::run() {
	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::run()");
	}

	tokenInfo = lexer.next();
	while (tokenInfo.first != END) {
		global_decl();
	}
}

//全局定义
void kkli::Generator::global_decl() {
	/*
	文法：
	<global_decl> = <enum_decl> | <var_decl> | <func_decl>
	*/

	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::global_decl()");
	}

	int type;  //全局定义的类型
	int i;
	baseType = INT_TYPE;  //基本类型默认为INT_TYPE

	//enum常量定义
	if (tokenInfo.first == ENUM) {
		enum_decl();
	}

	//全局变量或函数定义的类型
	else if (tokenInfo.first == INT) {
		match(INT);
	}
	else if (tokenInfo.first == CHAR) {
		match(CHAR);
		baseType == CHAR_TYPE;
	}

	while (tokenInfo.first != SEMICON && tokenInfo.first != '}') {
		type = baseType;

		//多级指针，如 int *******x;
		while (tokenInfo.first == MUL) {
			match(MUL);
			type = type + PTR_TYPE;
		}

		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("Generator::global_decl(): type = " + Token::getDataTypeName(type));
		}

		if (tokenInfo.first != ID) {
			throw Error(lexer.getLine(), "expected token [ID]");
		}

		if (table->getCurrentToken().klass != ERROR) {
			throw Error(lexer.getLine(), "duplicate global declaration, [id] = " +
				table->getCurrentToken().name);
		}

		match(ID);

		//函数定义
		if (tokenInfo.first == LPAREN) {
			table->finishToken(FUNC, type, reinterpret_cast<int>(vm->getNextTextPos()));
			func_decl();
		}

		//全局变量声明之后必须为分隔符（逗号或分号）
		else if (tokenInfo.first != COMMA && tokenInfo.first != SEMICON) {
			throw Error(lexer.getLine(), "wrong variables declaration.");
		}
		else {
			table->finishToken(GLOBAL, type, reinterpret_cast<int>(vm->getNextDataPos()));
			vm->addIntData(0);  //添加一个0，为这个变量占据写入值的位置，避免这个位置被其他变量使用。
		}
		
		if (tokenInfo.first == COMMA) {
			match(COMMA);
		}
	}

	lexer.next();
}

//enum定义
void kkli::Generator::enum_decl() {
	/*
    <enum_decl> = 'enum' '{' <id> [ '=' <num> ] 
	              {',' <id> [ '=' <num> ]}+ '}' ';'
	*/

	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::enum_decl()");
	}

	int varIndex = 0;  //enum常量的位置
	int varValue = 0;  //enum常量的值
	match(ENUM);
	match(LBRACE);
	while (tokenInfo.first != RBRACE) {
		++varIndex;

		if (tokenInfo.first != ID) {
			throw Error(lexer.getLine(), "expected token [ID]");
		}
		tokenInfo = lexer.next();

		//{ a = 0 } 中的等于
		if (tokenInfo.first == ASSIGN) {
			tokenInfo = lexer.next();
			if (tokenInfo.first != NUM) {
				throw Error(lexer.getLine(), "expected token [NUM]");
			}
			varValue = tokenInfo.second;
			tokenInfo = lexer.next();
		}

		//Debug：输出当前处理的enum常量的索引及值
		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("Generator::enum_decl(): index = " + std::to_string(varIndex)
				+ "  value = " + std::to_string(varValue));
		}

		table->finishToken(NUMBER, INT_TYPE, varValue++);

		//{ a = 0, b = 1 } 中的逗号
		if (tokenInfo.first == COMMA) {
			tokenInfo = lexer.next();

			//逗号后必须跟enum常量
			if (tokenInfo.first != ID) {
				throw Error(lexer.getLine(), "expected token [ID]");
			}
		}
	}
	match(RBRACE);
	match(SEMICON);
}

//函数定义
void kkli::Generator::func_decl() {
	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::func_del()");
	}

	match(LPAREN);
	func_param();
	match(RPAREN);
	match(LBRACE);
	func_body();
	
	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("before recover: \n" + table->getSymbolTableInfo());
	}

	//恢复全局变量
	std::vector<Token> tb = table->getTable();
	for (auto& tk : tb) {
		if (tk.klass == LOCAL) {
			tk.restoreInfo();
		}
	}

	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("After restore: \n" + table->getSymbolTableInfo());
	}
}

//函数参数定义
void kkli::Generator::func_param() {
	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::func_param()");
	}

	int dataType;
	int params = 0;
	while (tokenInfo.first != RPAREN) {
		dataType = INT_TYPE;
		if (tokenInfo.first == INT) {
			match(INT);
		}
		else if (tokenInfo.first == CHAR) {
			match(CHAR);
			dataType = CHAR_TYPE;
		}

		//指针类型
		while (tokenInfo.first == MUL) {
			match(MUL);
			dataType = dataType + PTR_TYPE;
		}

		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("Generator::func_param(): param type = " + Token::getDataTypeName(dataType));
		}

		if (tokenInfo.first != ID) {
			throw Error(lexer.getLine(), "bad parameter declaration.");
		}
		if (table->getCurrentToken().klass == LOCAL) {
			throw Error(lexer.getLine(), "duplicate parameter declaration.");
		}

		match(ID);

		if (tokenInfo.first != COMMA && tokenInfo.first != RPAREN) {
			throw Error(lexer.getLine(), "wrong param declaration!");
		}

		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("before backup: " + table->getSymbolTableInfo());
		}

		//备份全局变量信息，并填入局部变量信息
		Token& tk = table->getCurrentToken();
		tk.saveInfo();
		tk.klass = LOCAL;
		tk.dataType = dataType;
		tk.value = params++;
		
		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("after backup: " + table->getSymbolTableInfo());
		}

		if (tokenInfo.first == COMMA) {
			match(COMMA);
		}
	}

	indexOfBP = params + 1;

	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("indexOfBP: " + std::to_string(indexOfBP));
	}
}

//函数体
void kkli::Generator::func_body() {
	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::func_body()");
	}

	int variableIndex = indexOfBP;  //局部变量在栈上相对于bp的位置

	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::func_body(), start variable decl.");
	}

	//局部变量定义
	while (tokenInfo.first == INT || tokenInfo.first == CHAR) {
		baseType = (tokenInfo.first == INT) ? INT_TYPE : CHAR_TYPE;
		match(tokenInfo.first);

		while (tokenInfo.first != SEMICON) {
			int dataType = baseType;

			//多级指针
			while (tokenInfo.first == MUL) {
				match(MUL);
				dataType += PTR_TYPE;
			}

			if (tokenInfo.first != ID) {
				throw Error(lexer.getLine(), "bad local declaration.");
			}

			if (table->getCurrentToken().klass == LOCAL) {
				throw Error(lexer.getLine(), "duplicate local declaration.");
			}

			match(ID);

			if (tokenInfo.first != COMMA || tokenInfo.first != SEMICON) {
				throw Error(lexer.getLine(), "bad local declaration.");
			}

			//存储局部变量
			Token& currToken = table->getCurrentToken();
			currToken.saveInfo();
			currToken.klass = LOCAL;
			currToken.dataType = dataType;
			currToken.value = ++variableIndex;

			if (tokenInfo.first == COMMA) {
				match(COMMA);
			}
		}

		match(SEMICON);
	}

	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::func_body(), end variable decl.");
	}

	//在栈上留下保存变量所需的空间，并进入函数
	vm->addInst(I_ENT);
	vm->addInstData(variableIndex - indexOfBP);

	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::func_body(), start statement decl.");
	}

	//语句定义
	while (tokenInfo.first != RBRACE) {
		statement();
	}

	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::func_body(), end statement decl.");
	}

	vm->addInst(I_LEV);
}

//语句
void kkli::Generator::statement() {
	//1. if(expr) statement [else statement]
	//2. while(expr) statement
	//3. { statement }
	//4. return expr;
	//5. expr;  //expr可空
	
	//if语句
	if (tokenInfo.first == IF) {
		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("Generator::statement(): [if]");
		}

		match(IF);
		match(LPAREN);
		expression(ASSIGN);
		match(RPAREN);

		int *branch;  //用于填写语句的分支跳转地址

		vm->addInst(I_JZ);
		branch = vm->getNextTextPos();
		vm->addInstData(0);  //占据一个位置，用以写入 I_JZ 的跳转位置
		statement();
		if (tokenInfo.first == ELSE) {
			if (OUTPUT_GENERATOR_ACTIONS) {
				Debug::output("    has [else]");
			}

			match(ELSE);
			*branch = int(vm->getNextTextPos() + 2);
			vm->addInst(I_JMP);
			branch = vm->getNextTextPos();
			vm->addInstData(0);  //占据一个位置
			statement();
		}
		*branch = int(vm->getNextTextPos());
	}

	//while语句
	else if (tokenInfo.first == WHILE) {
		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("Generator::statement(): [while]");
		}

		int *branchA, *branchB;  //用于填写语句的分支跳转地址
		match(WHILE);
		branchA = vm->getNextTextPos();
		match(LPAREN);
		expression(ASSIGN);
		match(RPAREN);

		vm->addInst(I_JZ);
		branchB = vm->getNextTextPos();
		vm->addInstData(0);

		statement();
		vm->addInst(I_JMP);
		vm->addInstData(int(branchA));
		*branchB = int(vm->getNextTextPos());
	}

	//{ statement }
	else if (tokenInfo.first == LBRACE) {
		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("Generator::statement(): [{statement}]");
		}

		match(LBRACE);
		while (tokenInfo.first != RBRACE) {
			statement();
		}
		match(RBRACE);
	}

	//return语句
	else if (tokenInfo.first == RETURN) {
		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("Generator::statement(): [return]");
		}

		match(RETURN);
		if (tokenInfo.first != SEMICON) {
			if (OUTPUT_GENERATOR_ACTIONS) {
				Debug::output("    has [expr]");
			}
			expression(ASSIGN);
		}
		match(SEMICON);
		vm->addInst(I_LEV);
	}

	//expr
	else {
		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("Generator::statement(): [expr]");
		}

		expression(ASSIGN);
		match(SEMICON);
	}
}

//表达式
void kkli::Generator::expression(int priority) {
	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::expression(" + std::to_string(priority) + ")");
	}

	//单元表达式
	{
		if (tokenInfo.first == ERROR) {
			throw Error(lexer.getLine(), "bad identifier ERROR.");
		}

		if (tokenInfo.first == END) {
			throw Error(lexer.getLine(), "unexpected token EOF of expression.");
		}

		if (tokenInfo.first == NUM) {
			match(NUM);
			vm->addInst(I_IMM);
			vm->addInstData(tokenInfo.second);
			exprType = INT_TYPE;
		}

		if (tokenInfo.first == STRING) {
			vm->addInst(I_IMM);
			vm->addInstData(tokenInfo.second);
			match(STRING);

			vm->addCharData('\0');
			exprType = PTR_TYPE;
		}

		//TODO: 
	}
}