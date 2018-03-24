#include "stdafx.h"
#include "Generator.h"

//���캯��
kkli::Generator::Generator(std::string sourceFile)
	: lexer(sourceFile) {
	table = SymbolTable::getInstance();
	vm = VirtualMachine::getInstance();
}

//ƥ��Token
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

//ȫ�ֶ���
void kkli::Generator::global_decl() {
	/*
	�ķ���
	<global_decl> = <enum_decl> | <var_decl> | <func_decl>
	*/

	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::global_decl()");
	}

	int type;  //ȫ�ֶ��������
	int i;
	baseType = INT_TYPE;  //��������Ĭ��ΪINT_TYPE

	//enum��������
	if (tokenInfo.first == ENUM) {
		enum_decl();
	}

	//ȫ�ֱ����������������
	else if (tokenInfo.first == INT) {
		match(INT);
	}
	else if (tokenInfo.first == CHAR) {
		match(CHAR);
		baseType == CHAR_TYPE;
	}

	while (tokenInfo.first != SEMICON && tokenInfo.first != '}') {
		type = baseType;

		//�༶ָ�룬�� int *******x;
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

		//��������
		if (tokenInfo.first == LPAREN) {
			table->finishToken(FUNC, type, reinterpret_cast<int>(vm->getNextTextPos()));
			func_decl();
		}

		//ȫ�ֱ�������֮�����Ϊ�ָ��������Ż�ֺţ�
		else if (tokenInfo.first != COMMA && tokenInfo.first != SEMICON) {
			throw Error(lexer.getLine(), "wrong variables declaration.");
		}
		else {
			table->finishToken(GLOBAL, type, reinterpret_cast<int>(vm->getNextDataPos()));
			vm->addIntData(0);  //���һ��0��Ϊ�������ռ��д��ֵ��λ�ã��������λ�ñ���������ʹ�á�
		}
		
		if (tokenInfo.first == COMMA) {
			match(COMMA);
		}
	}

	lexer.next();
}

//enum����
void kkli::Generator::enum_decl() {
	/*
    <enum_decl> = 'enum' '{' <id> [ '=' <num> ] 
	              {',' <id> [ '=' <num> ]}+ '}' ';'
	*/

	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::enum_decl()");
	}

	int varIndex = 0;  //enum������λ��
	int varValue = 0;  //enum������ֵ
	match(ENUM);
	match(LBRACE);
	while (tokenInfo.first != RBRACE) {
		++varIndex;

		if (tokenInfo.first != ID) {
			throw Error(lexer.getLine(), "expected token [ID]");
		}
		tokenInfo = lexer.next();

		//{ a = 0 } �еĵ���
		if (tokenInfo.first == ASSIGN) {
			tokenInfo = lexer.next();
			if (tokenInfo.first != NUM) {
				throw Error(lexer.getLine(), "expected token [NUM]");
			}
			varValue = tokenInfo.second;
			tokenInfo = lexer.next();
		}

		//Debug�������ǰ�����enum������������ֵ
		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("Generator::enum_decl(): index = " + std::to_string(varIndex)
				+ "  value = " + std::to_string(varValue));
		}

		table->finishToken(NUMBER, INT_TYPE, varValue++);

		//{ a = 0, b = 1 } �еĶ���
		if (tokenInfo.first == COMMA) {
			tokenInfo = lexer.next();

			//���ź�����enum����
			if (tokenInfo.first != ID) {
				throw Error(lexer.getLine(), "expected token [ID]");
			}
		}
	}
	match(RBRACE);
	match(SEMICON);
}

//��������
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

	//�ָ�ȫ�ֱ���
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

//������������
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

		//ָ������
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

		//����ȫ�ֱ�����Ϣ��������ֲ�������Ϣ
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

//������
void kkli::Generator::func_body() {
	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::func_body()");
	}

	int variableIndex = indexOfBP;  //�ֲ�������ջ�������bp��λ��

	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::func_body(), start variable decl.");
	}

	//�ֲ���������
	while (tokenInfo.first == INT || tokenInfo.first == CHAR) {
		baseType = (tokenInfo.first == INT) ? INT_TYPE : CHAR_TYPE;
		match(tokenInfo.first);

		while (tokenInfo.first != SEMICON) {
			int dataType = baseType;

			//�༶ָ��
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

			//�洢�ֲ�����
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

	//��ջ�����±����������Ŀռ䣬�����뺯��
	vm->addInst(I_ENT);
	vm->addInstData(variableIndex - indexOfBP);

	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::func_body(), start statement decl.");
	}

	//��䶨��
	while (tokenInfo.first != RBRACE) {
		statement();
	}

	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::func_body(), end statement decl.");
	}

	vm->addInst(I_LEV);
}

//���
void kkli::Generator::statement() {
	//1. if(expr) statement [else statement]
	//2. while(expr) statement
	//3. { statement }
	//4. return expr;
	//5. expr;  //expr�ɿ�
	
	//if���
	if (tokenInfo.first == IF) {
		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("Generator::statement(): [if]");
		}

		match(IF);
		match(LPAREN);
		expression(ASSIGN);
		match(RPAREN);

		int *branch;  //������д���ķ�֧��ת��ַ

		vm->addInst(I_JZ);
		branch = vm->getNextTextPos();
		vm->addInstData(0);  //ռ��һ��λ�ã�����д�� I_JZ ����תλ��
		statement();
		if (tokenInfo.first == ELSE) {
			if (OUTPUT_GENERATOR_ACTIONS) {
				Debug::output("    has [else]");
			}

			match(ELSE);
			*branch = int(vm->getNextTextPos() + 2);
			vm->addInst(I_JMP);
			branch = vm->getNextTextPos();
			vm->addInstData(0);  //ռ��һ��λ��
			statement();
		}
		*branch = int(vm->getNextTextPos());
	}

	//while���
	else if (tokenInfo.first == WHILE) {
		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("Generator::statement(): [while]");
		}

		int *branchA, *branchB;  //������д���ķ�֧��ת��ַ
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

	//return���
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

//���ʽ
void kkli::Generator::expression(int priority) {
	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::expression(" + std::to_string(priority) + ")");
	}

	//��Ԫ���ʽ
	{
		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("Generator::expression(): ======== [unary expression] ========");
		}
		if (tokenInfo.first == ERROR) {
			throw Error(lexer.getLine(), "bad identifier ERROR.");
		}

		else if (tokenInfo.first == END) {
			throw Error(lexer.getLine(), "unexpected token EOF of expression.");
		}

		else if (tokenInfo.first == NUM) {
			match(NUM);
			vm->addInst(I_IMM);
			vm->addInstData(tokenInfo.second); 
			if (OUTPUT_GENERATOR_ACTIONS) {
				Debug::output("Generator::expression(): [NUM] "+std::to_string(tokenInfo.second));
			}
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == STRING) {
			vm->addInst(I_IMM);
			vm->addInstData(tokenInfo.second);
			match(STRING);

			vm->addCharData('\0');
			exprType = PTR_TYPE;
			if (OUTPUT_GENERATOR_ACTIONS) {
				Debug::output(std::string("Generator::expression(): [STRING] ")
					+ reinterpret_cast<char*>(tokenInfo.second));
			}
		}

		else if (tokenInfo.first == SIZEOF) {
			match(SIZEOF);
			match(LPAREN);
			exprType = INT_TYPE;
			if (tokenInfo.first == INT) {
				match(INT);
			}
			else if (tokenInfo.first == CHAR) {
				match(CHAR);
				exprType = CHAR_TYPE;
			}
			while (tokenInfo.first == MUL) {
				match(MUL);
				exprType = exprType + PTR_TYPE;
			}
			if (OUTPUT_GENERATOR_ACTIONS) {
				Debug::output("Generator::expression(): [SIZEOF] " + Token::getDataTypeName(exprType));
			}
			match(RPAREN);
			vm->addInst(I_IMM);
			vm->addInstData(exprType == CHAR_TYPE ? 1 : 4);
			exprType = INT;
		}

		else if (tokenInfo.first == ID) {
			//���ֿ��ܣ��������á�enum������ȫ��/�ֲ�����
			if (OUTPUT_GENERATOR_ACTIONS) {
				Debug::output("Generator::expression(): [ID]");
			}
			match(ID);
			
			Token& current = table->getCurrentToken();

			//��������
			if (tokenInfo.first == LPAREN) {
				match(LPAREN);
				int args = 0;
				while (tokenInfo.first != RPAREN) {
					expression(ASSIGN);
					vm->addInst(I_PUSH);
					++args;
					if (tokenInfo.first == COMMA) {
						match(COMMA);
					}
					if (tokenInfo.first == RPAREN) {
						throw Error(lexer.getLine(), "bad function call.");
					}
				}
				match(RPAREN);

				//ϵͳ����
				if (current.klass == SYS_FUNC) {
					if (OUTPUT_GENERATOR_ACTIONS) {
						Debug::output("Generator::expression(): [SYS_FUNC]");
					}
					vm->addInst(current.value);
				}

				//�û��Զ��庯��
				else if (current.klass == FUNC) {
					if (OUTPUT_GENERATOR_ACTIONS) {
						Debug::output("Generator::expression(): [CUSTOM_FUNC]");
					}
					vm->addInst(I_CALL);
					vm->addInstData(current.value);
				}
				else {
					throw Error(lexer.getLine(), "bad function call.");
				}

				//���ջ�ϲ���
				if (args > 0) {
					if (OUTPUT_GENERATOR_ACTIONS) {
						Debug::output("Generator::expression(): elim params("
							+ std::to_string(args) + ") on stack");
					}
					vm->addInst(I_ADJ);
					vm->addInstData(args);
				}
				exprType = current.dataType;
			}

			//enum����
			else if (current.klass == NUMBER) {
				if (OUTPUT_GENERATOR_ACTIONS) {
					Debug::output("Generator::expression(): [enum variable]");
				}
				vm->addInst(I_IMM);
				vm->addInstData(current.value);
				exprType = INT_TYPE;
			}

			//��ͨ����
			else {
				if (current.klass == LOCAL) {
					if (OUTPUT_GENERATOR_ACTIONS) {
						Debug::output("Generator::expression(): [local variable]");
					}
					vm->addInst(I_LEA);
					vm->addInstData(indexOfBP - current.value);
				}
				else if (current.klass == GLOBAL) {
					if (OUTPUT_GENERATOR_ACTIONS) {
						Debug::output("Generator::expression(): [global variable]");
					}
					vm->addInst(I_IMM);
					vm->addInstData(current.value);
				}
				else {
					throw Error(lexer.getLine(), "undefined variable.");
				}

				//Ĭ�ϲ����Ǽ���ֵ����ֵ������������Ǹ�ֵ����Ĩ��I_LC/I_LI��ֻ�����ַ
				exprType = current.dataType;
				vm->addInst = (exprType == CHAR_TYPE) ? I_LC : I_LI;
			}
		}

		else if (tokenInfo.first == LPAREN) {
			match(LPAREN);

			//ǿ������ת��
			if (tokenInfo.first == INT || tokenInfo.first == CHAR) {
				int castType = (tokenInfo.first == CHAR) ? CHAR_TYPE : INT_TYPE;
				match(tokenInfo.first);
				while (tokenInfo.first == MUL) {
					match(MUL);
					castType += PTR_TYPE;
				}
				match(RPAREN);
				if (OUTPUT_GENERATOR_ACTIONS) {
					Debug::output("Generator::expression(): [cast] "
						+ Token::getDataTypeName(castType));
				}
				expression(INC);  //���ȼ�ͬ++
				exprType = castType;
			}

			//���ű��ʽ
			else {
				if (OUTPUT_GENERATOR_ACTIONS) {
					Debug::output("Generator::expression(): [ parenthiesis ]");
				}
				expression(ASSIGN);
				match(RPAREN);
			}
		}

		else if (tokenInfo.first == MUL) {
			if (OUTPUT_GENERATOR_ACTIONS) {
				Debug::output("Generator::expression(): [MUL]");
			}
			//*a����ȡĳ����ַ��ֵ
			match(MUL);
			expression(INC);
			if (exprType >= PTR_TYPE) {
				exprType -= PTR_TYPE;
			}
			else {
				throw Error(lexer.getLine(), "bad dereference.");
			}

			//����ȡһ��char�⣬ȡint��ȡ��ַ����ȡint��Ĭ��int��ָ���ֽ�����ͬ��
			vm->addInst(exprType == CHAR_TYPE ? I_LC : I_LI);
		}

		else if (tokenInfo.first == AND) {
			//ȡ��ַ
			if (OUTPUT_GENERATOR_ACTIONS) {
				Debug::output("Generator::expression(): [AND]");
			}
			match(AND);
			expression(INC);
			int inst = vm->getTopInst();
			if (inst == I_LC || inst == I_LI) {
				vm->deleteTopInst();
			}
			else {
				throw Error(lexer.getLine(), "bad address of.");
			}

			exprType = exprType + PTR_TYPE;
			if (OUTPUT_GENERATOR_ACTIONS) {
				Debug::output("Generator::expression(): exprType = "
					+ Token::getDataTypeName(exprType));
			}
		}

		else if (tokenInfo.first == NOT) {
			if (OUTPUT_GENERATOR_ACTIONS) {
				Debug::output("Generator::expression(): [NOT]");
			}
			match(NOT);
			expression(INC);

			//!expr �ȼ��� expr == 0
			vm->addInst(I_PUSH);
			vm->addInst(I_IMM);
			vm->addInstData(0);
			vm->addInst(I_EQ);

			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == TILDE) {
			if (OUTPUT_GENERATOR_ACTIONS) {
				Debug::output("Generator::expression(): [TILDE]");
			}
			match(TILDE);
			expression(INC);
			
			//~a �ȼ��� a^(-1)
			vm->addInst(I_PUSH);
			vm->addInst(I_IMM);
			vm->addInstData(-1);
			vm->addInst(I_XOR);

			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == ADD) {
			if (OUTPUT_GENERATOR_ACTIONS) {
				Debug::output("Generator::expression(): [ADD]");
			}
			//+a��ɶҲ����
			match(ADD);
			expression(INC);
			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == SUB) {
			if (OUTPUT_GENERATOR_ACTIONS) {
				Debug::output("Generator::expression(): [SUB]");
			}
			//-a
			match(SUB);
			if (tokenInfo.first == NUM) {
				vm->addInst(I_IMM);
				vm->addInstData(-tokenInfo.second);
			}
			else {
				//-a �ȼ��� (-1)*a
				vm->addInst(I_IMM);
				vm->addInstData(-1);
				expression(INC);
				vm->addInst(I_MUL);
			}

			exprType = INT_TYPE;
		}

		else if (tokenInfo.first == INC || tokenInfo.first == DEC) {
			//TODO: 
		}
	}
}
