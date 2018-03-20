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
	if (tokenInfo.first == type) {
		tokenInfo = lexer.next();
	}
	else {
		throw Error(lexer.getLine(), "expected token [" + Token::getTokenTypeName(type) + "]");
	}
}

void kkli::Generator::run() {
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
			vm->addData(0);  //���һ��0��Ϊ�������ռ��д��ֵ��λ�ã��������λ�ñ���������ʹ�á�
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
			tk.klass = tk.Bklass;
			tk.type = tk.Btype;
			tk.value = tk.Bvalue;
		}
	}

	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("After recover: \n" + table->getSymbolTableInfo());
	}
}

//������������
void kkli::Generator::func_param() {
	if (OUTPUT_GENERATOR_ACTIONS) {
		Debug::output("Generator::func_param()");
	}

	int type;
	int params = 0;
	while (tokenInfo.first != RPAREN) {
		type = INT_TYPE;
		if (tokenInfo.first == INT) {
			match(INT);
		}
		else if (tokenInfo.first == CHAR) {
			match(CHAR);
			type = CHAR_TYPE;
		}

		//ָ������
		while (tokenInfo.first == MUL) {
			match(MUL);
			type = type + PTR_TYPE;
		}

		if (OUTPUT_GENERATOR_ACTIONS) {
			Debug::output("param type: " + Token::getDataTypeName(type));
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
		tk.Bklass = tk.klass;
		tk.Btype = tk.type;
		tk.Bvalue = tk.value;
		tk.klass = LOCAL;
		tk.type = type;
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

//TODO: ������