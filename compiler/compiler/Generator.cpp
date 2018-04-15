#include "stdafx.h"
#include "Generator.h"

//���캯��
kkli::Generator::Generator(std::string sourceFile)
	: lexer(sourceFile, "") {
	table = SymbolTable::getInstance();
	vm = VirtualMachine::getInstance();
}

//ƥ��Token
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

//ȫ�ֶ���
void kkli::Generator::global_decl(std::string format) {
	/*
	�ķ���
	<global_decl> = <enum_decl> | <var_decl> | <func_decl>
	*/
	DEBUG_GENERATOR("Generator::global_decl()", format);

	int type;  //ȫ�ֶ��������
	baseType = INT_TYPE;

	//enum����
	if (tokenInfo.first == ENUM) {
		enum_decl(FORMAT(format));
		return;
	}

	//ȫ�ֱ��������Ķ���
	if (tokenInfo.first == INT) {
		match(INT, FORMAT(format));
	}
	else if (tokenInfo.first == CHAR) {
		match(CHAR, FORMAT(format));
		baseType = CHAR_TYPE;
	}
	else {
		throw Error(lexer.getLine(), "bad type [" + Token::getTokenTypeName(tokenInfo.first) + "]");
	}

	while (tokenInfo.first != SEMICON && tokenInfo.first != RBRACE) {
		type = baseType;

		//�༶ָ�룬�� int *******x;
		while (tokenInfo.first == MUL) {
			match(MUL, FORMAT(format));
			type = type + PTR_TYPE;
		}

		DEBUG_GENERATOR("type = " + Token::getDataTypeName(type), FORMAT(format));
		if (tokenInfo.first != ID) {
			std::cout << "tokenInfo.first: " << Token::getTokenTypeName(tokenInfo.first) << std::endl;
			throw Error(lexer.getLine(), "expected token [ID]");
		}

		if (table->getCurrentToken(FORMAT(format)).klass != ERROR) {
			throw Error(lexer.getLine(), "duplicate global declaration, [id] = " +
				table->getCurrentToken(FORMAT(format)).name);
		}

		match(ID, FORMAT(format));

		//��������
		if (tokenInfo.first == LPAREN) {
			Token& tk = table->getCurrentToken(FORMAT(format));
			tk.klass = FUNC;
			tk.dataType = type;
			tk.value = reinterpret_cast<int>(vm->getNextTextPos());  //������λ�ã����һ��ָ���λ��
			func_decl(FORMAT(format));
		}

		//���鶨��
		else if (tokenInfo.first == LBRACK) {
			global_arr_decl(type, FORMAT(format));

			if (tokenInfo.first == COMMA) {
				match(COMMA, FORMAT(format));
			}
		}

		//ȫ�ֱ�������
		else {
			global_var_decl(type, FORMAT(format));

			if (tokenInfo.first == COMMA) {
				match(COMMA, FORMAT(format));
			}
		}
	}

	DEBUG_GENERATOR(std::string("now tokenInfo.first = ") + (tokenInfo.first == SEMICON ? "SEMICON" : "RBRACE"), FORMAT(format));
	tokenInfo = lexer.next(FORMAT(format));  //������SEMICON��RBRACE��������match
}

//ȫ�ֱ�������
void kkli::Generator::global_var_decl(int type, std::string format) {
	//<type> {'*'}+ <id> ['=' <num>] {',' {'*'}+ <id> ['=' <num>]}+ ';'
	//                   ^         ^
	//                   |---------|

	//������������Ϊ�ָ���
	if (tokenInfo.first != COMMA && tokenInfo.first != SEMICON && tokenInfo.first != ASSIGN) {
		throw Error(lexer.getLine(), "wrong variables declaration.");
	}

	Token& tk = table->getCurrentToken(FORMAT(format));
	tk.klass = GLOBAL;
	tk.dataType = type;
	tk.value = reinterpret_cast<int>(vm->getNextDataPos(INT_TYPE, FORMAT(format)));
	vm->addDataInt(0, FORMAT(format));

	//������ʼ��
	if (tokenInfo.first == ASSIGN) {
		match(ASSIGN, FORMAT(format));

		//int a = +1;  int a = -1;
		int factor = 1;
		if (tokenInfo.first == SUB || tokenInfo.first == ADD) {
			tokenInfo = lexer.next(FORMAT(format));
			factor = (tokenInfo.first == SUB ? -1 : 1);
			if (tokenInfo.first != NUM) {
				throw Error(lexer.getLine(), "bad variable definition.");
			}
		}

		//��ֵ��
		if (tokenInfo.first == NUM) {
			if (tk.dataType >= PTR_TYPE) {
				WARNING->add(lexer.getLine(), "assign a number to a pointer.");
			}
			*reinterpret_cast<int*>(tk.value) = factor*tokenInfo.second;  //д���ʼֵ
		}

		//�ַ�����
		else if (tokenInfo.first == STRING) {
			if (tk.dataType != PTR_TYPE) {
				WARNING->add(lexer.getLine(), Token::getDataTypeName(tk.dataType) + " type variable " + tk.name + " get STRING type value.");
			}
			*reinterpret_cast<int*>(tk.value) = factor*tokenInfo.second;
		}
		else {
			throw Error(lexer.getLine(), "bad variable definition.");
		}
		tokenInfo = lexer.next(FORMAT(format));
	}
}

//ȫ�����鶨��
void kkli::Generator::global_arr_decl(int type, std::string format) {
	//<type> { '*' }+ <id> '[' <num> ']' [ '=' '{' {<num>, [',' <num>]+'}' ] ';'
	//                      ^                                        ^
	//                      |----------------------------------------|
	Token& tk = table->getCurrentToken(FORMAT(format));
	tk.klass = GLOBAL;
	tk.dataType = type + PTR_TYPE;

	//ע�⣬int arr[10]; ��arr������Ҫһ��4�ֽڿռ����ڴ洢��ֵ
	//arr��ֵ��������10��Ԫ��֮ǰ���� (low addr) | arr | elem1 | elem2 | ... | elem10 | (high addr)
	tk.value = reinterpret_cast<int>(vm->getNextDataPos(type, FORMAT(format)));
	vm->addDataInt(0, FORMAT(format));
	*reinterpret_cast<int*>(tk.value) = reinterpret_cast<int>(vm->getNextDataPos(type, FORMAT(format)));

	match(LBRACK, FORMAT(format));
	if (tokenInfo.first != NUM) {
		throw Error(lexer.getLine(), "Generator::global_decl(): wrong array definition.");
	}

	int arraySize = tokenInfo.second;
	match(NUM, FORMAT(format));
	match(RBRACK, FORMAT(format));

	std::vector<int> values;

	//�����ʼ��
	if (tokenInfo.first == ASSIGN) {
		match(ASSIGN, FORMAT(format));
		match(LBRACE, FORMAT(format));

		while (tokenInfo.first != RBRACE) {
			if (tokenInfo.first != NUM && tokenInfo.first != STRING) {
				throw Error(lexer.getLine(), "VirtualMachine::global_arr_decl(): need number in array definition.");
			}

			//TODO: ���ͼ��
			values.push_back(tokenInfo.second);
			tokenInfo = lexer.next(FORMAT(format));
			if (tokenInfo.first == COMMA) {
				match(COMMA, FORMAT(format));

				//����֮�������ֵ�����ܳ��� int a[4] = {1, 2, 3, }; ���������3����ȱ��ֵ������
				if (tokenInfo.first != NUM && tokenInfo.first != STRING) {
					throw Error(lexer.getLine(), "Generator::global_arr_decl(): wrong array definition.");
				}
			}
			else if (tokenInfo.first != RBRACE) {
				throw Error(lexer.getLine(), "Generator::local_arr_decl(): wrong array definition.");
			}
		}
		if (values.size() > arraySize) {
			throw Error(lexer.getLine(), "Generator::global_arr_decl(): too many values for array definition.");
		}

		//char�ͣ�1�ֽڣ�����
		if (type == CHAR_TYPE) {
			std::vector<char> vals;
			for (int i = 0; i < values.size(); ++i) {
				vals.push_back(values[i]);
			}
			vm->addDataCharArray(arraySize, vals, FORMAT(format));
		}

		//int�ͣ�4�ֽڣ�����
		else {
			vm->addDataIntArray(arraySize, values, FORMAT(format));
		}

		match(RBRACE, FORMAT(format));
	}

	//��������
	else {
		if (type == CHAR_TYPE) {
			vm->addDataCharArray(arraySize, {}, FORMAT(format));
		}
		else {
			vm->addDataIntArray(arraySize, {}, FORMAT(format));
		}
	}
}

//enum����
void kkli::Generator::enum_decl(std::string format) {
	/*
    <enum_decl> = 'enum' '{' <id> [ '=' <num> ] 
	              {',' <id> [ '=' <num> ]}+ '}' ';'
	*/
	DEBUG_GENERATOR("Generator::enum_decl()", format);

	int varIndex = 0;  //enum������λ��
	int varValue = 0;  //enum������ֵ
	match(ENUM, FORMAT(format));
	match(LBRACE, FORMAT(format));
	while (tokenInfo.first != RBRACE) {
		++varIndex;

		match(ID, FORMAT(format));

		//enum { a = 0, ... } �еĵ���
		if (tokenInfo.first == ASSIGN) {
			match(ASSIGN, FORMAT(format));

			//���������� +1, -1 ��
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
		
		//{ a = 0, b = 1 } �еĶ���
		if (tokenInfo.first == COMMA) {
			match(COMMA, FORMAT(format));

			//���ź�����enum����
			if (tokenInfo.first != ID) {
				throw Error(lexer.getLine(), "expected token [ID]");
			}
		}
	}
	match(RBRACE, FORMAT(format));
	match(SEMICON, FORMAT(format));
}

//��������
void kkli::Generator::func_decl(std::string format) {
	DEBUG_GENERATOR("Generator::func_del()", format);

	currFuncIndex = table->getCurrent(FORMAT(format));
	func_param(FORMAT(format));
	func_body(FORMAT(format));
	
	DEBUG_GENERATOR_SYMBOL("\n[======== before restore ========]" + table->getSymbolTableInfo(), FORMAT(format));

	//�ָ�ȫ�ֱ���
	std::vector<Token>& tb = table->getTable();
	for (auto& tk : tb) {
		if (tk.klass == LOCAL) {
			tk.restoreInfo(FORMAT(format));
		}
	}

	DEBUG_GENERATOR_SYMBOL("\n[======== after restore ========] " + table->getSymbolTableInfo(), FORMAT(format));
}

//������������
void kkli::Generator::func_param(std::string format) {
	DEBUG_GENERATOR("Generator::func_param()", format);

	match(LPAREN, FORMAT(format));
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

		//ָ������
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

		table->getToken(currFuncIndex).addArgument(dataType, FORMAT(format));  //��¼��������������
		DEBUG_GENERATOR("add " + Token::getDataTypeName(dataType) + " argument", FORMAT(format));

		DEBUG_GENERATOR_SYMBOL("\n[======== before backup ========] " + table->getSymbolTableInfo(), "");

		//����ȫ�ֱ�����Ϣ��������ֲ�������Ϣ
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
	match(RPAREN, FORMAT(format));
	indexOfBP = params + 1;  //pc�Ĵ�����ֵ��bp+1��

	DEBUG_GENERATOR("[index of bp]: " + std::to_string(indexOfBP), FORMAT(format));
}

//������
void kkli::Generator::func_body(std::string format) {
	DEBUG_GENERATOR("Generator::func_body()", format);

	match(LBRACE, FORMAT(format));
	
	vm->addInst(I_ENT, FORMAT(format));
	int* entCount = vm->getNextTextPos();  //��¼�洢����������λ��
	vm->addInstData(0, FORMAT(format));

	int variableIndex = indexOfBP;
	while (tokenInfo.first != RBRACE) {

		//�ֲ�����
		if (tokenInfo.first == INT || tokenInfo.first == CHAR) {
			baseType = (tokenInfo.first == INT) ? INT_TYPE : CHAR_TYPE;
			match(tokenInfo.first, FORMAT(format));

			do {
				int dataType = baseType;

				//�༶ָ��
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

				if (tokenInfo.first != COMMA && tokenInfo.first != SEMICON && tokenInfo.first != ASSIGN) {
					throw Error(lexer.getLine(), "bad local declaration.");
				}

				DEBUG_GENERATOR_SYMBOL("\n[======== before backup ========] " + table->getSymbolTableInfo(), "");

				//�洢�ֲ�����
				Token& currToken = table->getCurrentToken(FORMAT(format));
				currToken.saveInfo(FORMAT(format));
				currToken.klass = LOCAL;
				currToken.dataType = dataType;
				currToken.value = ++variableIndex;

				DEBUG_GENERATOR_SYMBOL("\n[======== before backup ========] " + table->getSymbolTableInfo(), "");

				if (tokenInfo.first == COMMA) {
					match(COMMA, FORMAT(format));
				}
			} while (tokenInfo.first != SEMICON);

			match(SEMICON, FORMAT(format));
		}

		//���
		else {
			statement(FORMAT(format));
		}
	}
	
	*entCount = variableIndex - indexOfBP;  //�����������
	vm->addInst(I_LEV, FORMAT(format));
}

//���
void kkli::Generator::statement(std::string format) {
	//1. if(expr) statement [else statement]
	//2. while(expr) statement
	//3. { statement }
	//4. return expr;
	//5. expr;  //expr�ɿ�

	DEBUG_GENERATOR("Generator::statement()", format);
	
	//if���
	if (tokenInfo.first == IF) {
		DEBUG_GENERATOR("[if]", FORMAT(format));

		match(IF, FORMAT(format));
		match(LPAREN, FORMAT(format));
		expression(ASSIGN, FORMAT(format));
		match(RPAREN, FORMAT(format));

		int *branch;  //������д���ķ�֧��ת��ַ

		vm->addInst(I_JZ, FORMAT(format));
		branch = vm->getNextTextPos();
		vm->addInstData(0, FORMAT(format));  //ռ��һ��λ�ã�����д�� I_JZ ����תλ��
		statement(FORMAT(format));
		if (tokenInfo.first == ELSE) {
			DEBUG_GENERATOR("[else]", FORMAT(format));

			match(ELSE, FORMAT(format));
			*branch = int(vm->getNextTextPos() + 2);
			vm->addInst(I_JMP, FORMAT(format));
			branch = vm->getNextTextPos();
			vm->addInstData(0, FORMAT(format));  //ռ��һ��λ��
			statement(FORMAT(format));
		}
		*branch = int(vm->getNextTextPos());
	}

	//while���
	else if (tokenInfo.first == WHILE) {
		DEBUG_GENERATOR("[while]", FORMAT(format));

		int *branchA, *branchB;  //������д���ķ�֧��ת��ַ
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

	//return���
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

//���ʽ
void kkli::Generator::expression(int priority, std::string format) {
	DEBUG_GENERATOR("Generator::expression(" + std::to_string(priority) + ")", format);

	//һԪ���ʽ
	DEBUG_GENERATOR("[unary expression]", FORMAT(format));
	do {
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
			//���ֿ��ܣ��������á�enum������ȫ��/�ֲ�����
			DEBUG_GENERATOR("[ID]", FORMAT(format));

			Token& current = table->getCurrentToken(FORMAT(format));
			match(ID, FORMAT(format));

			//��������
			if (tokenInfo.first == LPAREN) {
				match(LPAREN, FORMAT(format));
				std::vector<int> dataTypes;  //��¼���������еĲ�������

				while (tokenInfo.first != RPAREN) {
					expression(ASSIGN, FORMAT(format));
					dataTypes.push_back(exprType);
					vm->addInst(I_PUSH, FORMAT(format));
					if (tokenInfo.first == COMMA) {
						match(COMMA, FORMAT(format));
					}
					else if (tokenInfo.first != RPAREN) {
						throw Error(lexer.getLine(), "bad function call!");
					}
				}
				match(RPAREN, FORMAT(format));

				//ϵͳ����
				if (current.klass == SYS_FUNC) {
					DEBUG_GENERATOR("[SYS_FUNC]", FORMAT(format));
					vm->addInst(current.value, FORMAT(format));
				}

				//�û��Զ��庯��
				else if (current.klass == FUNC) {
					DEBUG_GENERATOR("[CUSTOM_FUNC]", FORMAT(format));
					vm->addInst(I_CALL, FORMAT(format));
					vm->addInstData(current.value, FORMAT(format));
				}
				else {
					throw Error(lexer.getLine(), "function " + current.name + " not defined.");
				}

				//��֤�������õĺϷ���
				//������match(RPAREN)������Ϊ��������û�ж��壬����Ҫ�Ϸ���Error������ bad function call �Ĵ���
				validFunctionCall(current, dataTypes, FORMAT(format));

				//���ջ�ϲ���
				if (dataTypes.size() > 0) {
					vm->addInst(I_ADJ, FORMAT(format));
					vm->addInstData(dataTypes.size(), FORMAT(format));
				}
				exprType = current.dataType;
			}

			//enum����
			else if (current.klass == NUMBER) {
				DEBUG_GENERATOR("[enum variable]", FORMAT(format));
				vm->addInst(I_IMM, FORMAT(format));
				vm->addInstData(current.value, FORMAT(format));
				exprType = INT_TYPE;
			}

			//��ͨ����
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

				//Ĭ�ϲ����Ǽ���ֵ����ֵ���������Ǹ�ֵ����Ĩ��I_LC/I_LI��ֻ�����ַ
				exprType = current.dataType;
				vm->addInst(exprType == CHAR_TYPE ? I_LC : I_LI, FORMAT(format));
			}
		}

		else if (tokenInfo.first == LPAREN) {
			match(LPAREN, FORMAT(format));

			//ǿ������ת��
			if (tokenInfo.first == INT || tokenInfo.first == CHAR) {
				int castType = (tokenInfo.first == CHAR) ? CHAR_TYPE : INT_TYPE;
				match(tokenInfo.first, FORMAT(format));
				while (tokenInfo.first == MUL) {
					match(MUL, FORMAT(format));
					castType += PTR_TYPE;
				}
				match(RPAREN, FORMAT(format));
				DEBUG_GENERATOR("[cast] " + Token::getDataTypeName(castType), FORMAT(format));
				expression(INC, FORMAT(format));  //��Ҫ���ȼ�INC����һ����
				exprType = castType;
			}

			//���ű��ʽ
			else {
				DEBUG_GENERATOR("[(expr)]", FORMAT(format));
				expression(ASSIGN, FORMAT(format));
				match(RPAREN, FORMAT(format));
			}
		}

		else if (tokenInfo.first == MUL) {
			DEBUG_GENERATOR("[MUL]", FORMAT(format));
			//*a����ȡĳ����ַ��ֵ
			match(MUL, FORMAT(format));
			expression(INC, FORMAT(format));
			if (exprType >= PTR_TYPE) {
				exprType -= PTR_TYPE;
			}
			else {
				throw Error(lexer.getLine(), "bad dereference.");
			}

			//����ȡһ��char�⣬ȡint��ȡ��ַ����ȡint��Ĭ��int��ָ���ֽ�����ͬ��
			vm->addInst(exprType == CHAR_TYPE ? I_LC : I_LI, FORMAT(format));
		}

		else if (tokenInfo.first == AND) {
			//ȡ��ַ
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

			//!expr �ȼ��� expr == 0
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

			//~a �ȼ��� a^(-1)
			vm->addInst(I_PUSH, FORMAT(format));
			vm->addInst(I_IMM, FORMAT(format));
			vm->addInstData(-1, FORMAT(format));
			vm->addInst(I_XOR, FORMAT(format));

			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == ADD) {
			DEBUG_GENERATOR("[ADD]", FORMAT(format));
			//+a��ɶҲ����
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
				//-a �ȼ��� (-1)*a
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

			//++
			int tk = tokenInfo.first;
			match(tk, FORMAT(format));
			expression(INC, FORMAT(format));
			if (vm->getTopInst(FORMAT(format)) == I_LC) {
				vm->deleteTopInst(FORMAT(format));
				vm->addInst(I_PUSH, FORMAT(format));
				vm->addInst(I_LC, FORMAT(format));
			}
			else if (vm->getTopInst(FORMAT(format)) == I_LI) {
				vm->deleteTopInst(FORMAT(format));
				vm->addInst(I_PUSH, FORMAT(format));
				vm->addInst(I_LI, FORMAT(format));
			}
			else {
				throw Error(lexer.getLine(), "bad lvalue of pre-increment.");
			}
			vm->addInst(I_PUSH, FORMAT(format));

			//++pointer����Ӧ����pointer = int(pointer)+4������Ϊ++var����var = var+1
			vm->addInst(I_IMM, FORMAT(format));
			vm->addInstData(exprType > PTR_TYPE ? 4 : 1, FORMAT(format));
			vm->addInst(tk == INC ? I_ADD : I_SUB, FORMAT(format));
			vm->addInst(exprType == CHAR_TYPE ? I_SC : I_SI, FORMAT(format));
		}

		else {
			throw Error(lexer.getLine(), "bad expression.");
		}
	} while (false);

	//��Ԫ���ʽ
	DEBUG_GENERATOR("[binary expression]", FORMAT(format));
	do {
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
				vm->addInstData(0, FORMAT(format));      //ռ��һ��λ������д����ת��ַ
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
				vm->addInstData(0, FORMAT(format));     //ռ��һ��λ������д����ת��ַ
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
				exprType = tempType;  //����������������Ϊ׼
				//���⣺�� 'a' + 1000 ʱ���������Ϊchar�����255


				//��char*��ָ�룬���������4�ټӵ�ָ����
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

				//ָ����� p1 - p2
				if (tempType > PTR_TYPE && tempType == exprType) {
					DEBUG_GENERATOR("[SUB ptr - ptr]", FORMAT(format));

					vm->addInst(I_SUB, FORMAT(format));
					vm->addInst(I_PUSH, FORMAT(format));
					vm->addInst(I_IMM, FORMAT(format));
					vm->addInstData(4, FORMAT(format));
					vm->addInst(I_DIV, FORMAT(format));
					exprType = INT_TYPE;
				}

				//ָ���ƶ� p1 - 2
				else if (tempType > PTR_TYPE) {
					DEBUG_GENERATOR("[SUB ptr - var]", FORMAT(format));

					vm->addInst(I_PUSH, FORMAT(format));
					vm->addInst(I_IMM, FORMAT(format));
					vm->addInstData(4, FORMAT(format));
					vm->addInst(I_MUL, FORMAT(format));
					vm->addInst(MUL, FORMAT(format));
					exprType = tempType;
				}

				//��ͨ������� a - b
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

				//��++a���洢������a-1�������
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

				//��char*��ָ��
				if (tempType > PTR_TYPE) {
					vm->addInst(I_PUSH, FORMAT(format));
					vm->addInst(I_IMM, FORMAT(format));
					vm->addInstData(4, FORMAT(format));
					vm->addInst(I_MUL, FORMAT(format));
				}
				//�Ա���ִ��[]����������
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
	} while (false);
}

//��֤�����ĺϷ���
void kkli::Generator::validFunctionCall(const Token& funcToken, const std::vector<int>& dataTypes, std::string format) const {
	DEBUG_GENERATOR("kkli::Generator::ValidateFunctionCall()", format);

	//���ú�������Ҫ��������
	if (funcToken.klass == SYS_FUNC) {
		if (funcToken.name == "printf") {
			if (dataTypes.size() < 1) {
				throw Error(lexer.getLine(), "function '" + funcToken.name + "' need at least 1 argument.");
			}
			else if(dataTypes.size() > 6) {
				throw Error(lexer.getLine(), "function '" + funcToken.name + "' support at most 6 arguments.");
			}
			else if (dataTypes[0] < PTR_TYPE) {
				throw Error(lexer.getLine(), "function '" + funcToken.name + "' need ptr type argument at first argument.");
			}
		}
		else if (funcToken.name == "exit" || funcToken.name == "malloc") {
			if (dataTypes.size() != 1) {
				throw Error(lexer.getLine(), "function '" + funcToken.name + "' expect 1 argument.");
			}
			else if (dataTypes[0] != INT_TYPE) {
				Warning::getInstance()->add(lexer.getLine(), "function exit expect int type argument.");
			}
		}
		else {
			throw Error("no system function names '" + funcToken.name + "'.");
		}
		return;
	}

	//�����ú���
	int size1 = funcToken.argsDataType.size();
	int size2 = dataTypes.size();
	if (size1 != size2) {
		throw Error(lexer.getLine(), "function '" + funcToken.name + "' expect " + std::to_string(size1) + " arguments.");
	}
	for (int i = 0; i < size1; ++i) {
		if (funcToken.argsDataType[i] != dataTypes[i]) {
			WARNING->add(lexer.getLine(), "function '" + funcToken.name + "' need " +
				Token::getDataTypeName(funcToken.argsDataType[i]) + " type argument at argument index " + std::to_string(i) + ", but get "+
			Token::getDataTypeName(dataTypes[i])+ " type.");
		}
	}
}
