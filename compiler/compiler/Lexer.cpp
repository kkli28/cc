#include "stdafx.h"
#include "Lexer.h"

//���캯��
kkli::Lexer::Lexer(std::string sourceFile, std::string format) {
	DEBUG_LEXER("Lexer::lexer(" + sourceFile + ")", format);
	DEBUG_LEXER("[add builtin] begin", FORMAT(format));

	source = "char else enum if int return sizeof while printf malloc exit void main";
	source.push_back(END);
	
	SymbolTable* table = SymbolTable::getInstance();
	int type = CHAR;
	while (type <= WHILE) {
		next(FORMAT(format));
		table->getCurrentToken(FORMAT(format)).type = type;
		++type;
	}

	type = I_PRTF;
	while (type <= I_EXIT) {
		next(FORMAT(format));
		Token& tk = table->getCurrentToken(FORMAT(format));
		tk.klass = SYS_FUNC;
		tk.dataType = INT_TYPE;
		tk.value = type++;
	}

	next(FORMAT(format));
	table->getCurrentToken(FORMAT(format)).type = CHAR;
	next(FORMAT(format));
	table->setMainToken(FORMAT(format));

	DEBUG_LEXER("[add builtin] end", FORMAT(format));
	DEBUG_LEXER_SYMBOL_TABLE(table->getSymbolTableInfo(), FORMAT(format));

	std::ifstream inFile(sourceFile);
	if (!inFile.good()) {
		throw Error("can't open file: " + sourceFile);
	}

	inFile >> std::noskipws;    //�������հ�

	char buff[1000000];
	int i = 0;
	while (!inFile.eof()) inFile >> buff[i++];

	source = std::move(std::string(buff, i));

	//���ĩβ�ַ�
	source.push_back('\0');
	source.push_back(END);

	DEBUG_LEXER_SOURCE("\n[source--begin]:\n" + source + "\n[source--end]\n", FORMAT(format));

	index = 0;
	line = 1;
}

//next
std::pair<int, int> kkli::Lexer::next(std::string format) {
	DEBUG_LEXER("Lexer::next()", format);
	
	int value = 0;
	SymbolTable* table = SymbolTable::getInstance();
	VirtualMachine* vm = VirtualMachine::getInstance();

	char curr = source[index];
	while (curr != END) {

		//���з�
		if (curr == '\n') {
			curr = get();
			DEBUG_LEXER_NEXT("[\\n]", FORMAT(format));
			++line;
		}

		//�� �� �ļ����ã�ֱ����������֧�֣�
		else if (curr == '#') {
			DEBUG_LEXER_NEXT("[#...]", FORMAT(format));
			curr = get();
			while (curr != END && curr != '\n')  curr = get();
		}

		//��ʶ��
		else if (isAlpha(curr) || curr == '_') {
			DEBUG_LEXER_NEXT("[ID]", FORMAT(format));
			int begIndex = index;      //�ñ�ʶ�����Ƶ���ʼ����
			int hash = curr;           //�ñ�ʶ����hashֵ
			curr = get();
			while (isAlpha(curr) || isNum(curr) || curr == '_') {
				hash = hash * 147 + curr;
				curr = get();
			}

			std::string name(source, begIndex, index - begIndex);

			//���ű����иñ�ʶ��
			if (table->has(hash, name, FORMAT(format))) {
				return { table->getCurrentToken(FORMAT(format)).type, 0 };
			}

			//���ű���û�иñ�ʶ�����������������Ϣ
			else {
				Token& tk = table->getCurrentToken(FORMAT(format));
				tk.type = ID;
				tk.name = name;
				tk.hash = hash;

				DEBUG_LEXER_NEXT("add [type] ID, [name] " + name + ", [hash] " + std::to_string(hash), FORMAT(format));
				return { ID, 0 };
			}
		}

		//����
		else if (isNum(curr)) {
			value = curr - '0';
			curr = get();

			//ʮ���������˽�������ʮ����������0��0x��ʼ��
			if (value != 0) {
				while (isNum(curr)) {
					value = value * 10 + curr - '0';
					curr = get();
				}
				DEBUG_LEXER_NEXT("[dec] " + std::to_string(value), FORMAT(format));
			}

			else {
				//ʮ��������
				if (curr == 'x' || curr == 'X') {
					curr = get();
					bool hasNum = false;
					while (isNum(curr)
						|| (curr >= 'a' && curr <= 'z')
						|| (curr >= 'A' && curr <= 'Z')) {
						hasNum = true;
						value = value * 16 + (curr & 15) + (curr >= 'A' ? 9 : 0);
						curr = get();
					}

					if (!hasNum) {
						throw Error("Line "+std::to_string(line)+". Invalid hex type, need 0~9 or a~z/A~Z after 0x.");
					}
					DEBUG_LEXER_NEXT("[hex] " + std::to_string(value), FORMAT(format));
				}

				//�˽�����
				else {
					bool hasNum = false;
					while (curr >= '0' && curr <= '7') {
						hasNum = true;
						value = value * 8 + curr - '0';
						curr = get();
					}

					DEBUG_LEXER_NEXT("[oct] " + std::to_string(value), FORMAT(format));
				}
			}

			return { NUM, value };
		}

		//ע�� �� ����
		else if (curr == '/') {
			curr = get();

			//����ע��
			if (curr == '/') {
				while (curr != '\n' && curr != END) curr = get();
				DEBUG_LEXER_NEXT("[//]", FORMAT(format));
			}

			//����ע��
			else if (curr == '*') {
				curr = get();
				while (true) {
					if (curr == '*') {
						curr = get();
						if (curr == '/') {
							curr = get();
							break;
						}
					}
					else if (curr == END) break;
					else curr = get();
				}
				DEBUG_LEXER_NEXT("[/**/]", FORMAT(format));
			}

			//����
			else {
				DEBUG_LEXER_NEXT("[/]", FORMAT(format));
				return { DIV, 0 };
			}
		}

		//�ַ�
		else if (curr == '\'') {
			curr = get();
			if (curr == END) {
				throw Error("Line "+std::to_string(line)+". Invalid char type, need \'.");
			}

			//ת���ַ�
			if (curr == '\\') {
				curr = get();
				if (curr == END) {
					throw Error("Line " + std::to_string(line) + ". Invalid char type, need escape charactor after \\.");
				}

				if (curr == 'n') value = '\n';
				else value = curr;
			}

			curr = get();
			if (curr != '\'') {
				throw Error("Line "+std::to_string(line)+". Invalid char type, need \' to finish charactor");
			}
			curr = get();

			DEBUG_LEXER_NEXT("[char]", FORMAT(format));
			return { NUM, value };
		}

		//�ַ���
		else if (curr == '"') {
			curr = get();

			//��¼�ַ�����ʼλ��
			value = reinterpret_cast<int>(vm->getNextDataPos(CHAR_TYPE, FORMAT(format)));

			while (curr != '"' && curr != END) {

				//ת���ַ�
				if (curr == '\\') {
					curr = get();
					if (curr == 'n') vm->addDataChar('\n', FORMAT(format));
					else if (curr == END) {
						throw Error("Line " + std::to_string(line) + ". Invalid string type, need escape charactor after \\.");
					}
					else vm->addDataChar(curr, FORMAT(format));
					curr = get();
				}
				else {
					vm->addDataChar(curr, FORMAT(format));
					curr = get();
				}
			}

			//ĩβ����ַ� '\0'
			vm->addDataChar(0, FORMAT(format));

			if (curr == END) {
				throw Error("Line " +std::to_string(line)+". Invalid string type, need \" to finish string.");
			}
			curr = get();

			DEBUG_LEXER_NEXT("[string]", FORMAT(format));
			return { STRING, value };
		}

		// = �� ==
		else if (curr == '=') {
			curr = get();

			// ==
			if (curr == '=') {
				curr = get();

				DEBUG_LEXER_NEXT("[EQ]", FORMAT(format));
				return { EQ, 0 };
			}

			// =
			else {
				DEBUG_LEXER_NEXT("[ASSIGN]", FORMAT(format));
				return { ASSIGN, 0 };
			}
		}

		// + �� ++
		else if (curr == '+') {
			curr = get();

			// ++
			if (curr == '+') {
				curr = get();
				
				DEBUG_LEXER_NEXT("[INC]", FORMAT(format));
				return { INC, 0 };
			}

			// +
			else {
				DEBUG_LEXER_NEXT("[ADD]", FORMAT(format));
				return { ADD, 0 };
			}
		}

		// - �� --
		else if (curr == '-') {
			curr = get();

			// --
			if (curr == '-') {
				curr = get();
				DEBUG_LEXER_NEXT("[DEC]", FORMAT(format));
				return { DEC, 0 };
			}

			// -
			else {
				DEBUG_LEXER_NEXT("[SUB]", FORMAT(format));
				return { SUB, 0 };
			}
		}

		// ! �� !=
		else if (curr == '!') {
			curr = get();

			// !=
			if (curr == '=') {
				curr = get();
				DEBUG_LEXER_NEXT("[NE]", FORMAT(format));
				return { NE, 0 };
			}

			// !
			else {
				DEBUG_LEXER_NEXT("[NOT]", FORMAT(format));
				return { NOT, 0 };
			}
		}

		// < �� <= �� <<
		else if (curr == '<') {
			curr = get();

			// <=
			if (curr == '=') {
				curr = get();
				DEBUG_LEXER_NEXT("[LE]", FORMAT(format));
				return { LE, 0 };
			}

			// <<
			else if (curr == '<') {
				curr = get();
				DEBUG_LEXER_NEXT("[SHL]", FORMAT(format));
				return { SHL, 0 };
			}

			// <
			else {
				DEBUG_LEXER_NEXT("[LT]", FORMAT(format));
				return { LT, 0 };
			}
		}

		// > �� >= �� >>
		else if (curr == '>') {
			curr = get();

			// >=
			if (curr == '=') {
				curr = get();
				DEBUG_LEXER_NEXT("[GE]", FORMAT(format));
				return { GE, 0 };
			}

			// >>
			else if (curr == '>') {
				curr = get();
				DEBUG_LEXER_NEXT("[SHR]", FORMAT(format));
				return { SHR, 0 };
			}

			// >
			else {
				DEBUG_LEXER_NEXT("[GT]", FORMAT(format));
				return { GT, 0 };
			}
		}

		// | �� ||
		else if (curr == '|') {
			curr = get();

			// ||
			if (curr == '|') {
				curr = get();
				DEBUG_LEXER_NEXT("[LOR]", FORMAT(format));
				return { LOR, 0 };
			}

			// |
			else {
				DEBUG_LEXER_NEXT("[OR]", FORMAT(format));
				return { OR, 0 };
			}
		}

		// & �� &&
		else if (curr == '&') {
			curr = get();

			// &&
			if (curr == '&') {
				curr = get();
				DEBUG_LEXER_NEXT("[LAN]", FORMAT(format));
				return { LAN, 0 };
			}

			// &
			else {
				DEBUG_LEXER_NEXT("[AND]", FORMAT(format));
				return { AND, 0 };
			}
		}

		// ^
		else if (curr == '^') {
			curr = get();
			DEBUG_LEXER_NEXT("[XOR]", FORMAT(format));
			return { XOR, 0 };
		}

		// %
		else if (curr == '%') {
			curr = get();
			DEBUG_LEXER_NEXT("[MOD]", FORMAT(format));
			return { MOD, 0 };
		}

		// *
		else if (curr == '*') {
			curr = get();
			DEBUG_LEXER_NEXT("[MUL]", FORMAT(format));
			return { MUL, 0 };
		}

		// ?
		else if (curr == '?') {
			curr = get();
			DEBUG_LEXER_NEXT("[COND]", FORMAT(format));
			return { COND, 0 };
		}

		// ,
		else if (curr == ',') {
			curr = get();
			DEBUG_LEXER_NEXT("[COMMA]", FORMAT(format));
			return { COMMA, 0 };
		}

		// :
		else if (curr == ':') {
			curr = get();
			DEBUG_LEXER_NEXT("[COLON]", FORMAT(format));
			return { COLON, 0 };
		}

		// ;
		else if (curr == ';') {
			curr = get();
			DEBUG_LEXER_NEXT("[SEMICON]", FORMAT(format));
			return { SEMICON, 0 };
		}

		// (
		else if (curr == '(') {
			curr = get();
			DEBUG_LEXER_NEXT("[LPAREN]", FORMAT(format));
			return { LPAREN, 0 };
		}

		// )
		else if (curr == ')') {
			curr = get();
			DEBUG_LEXER_NEXT("[RPAREN]", FORMAT(format));
			return { RPAREN, 0 };
		}

		// [
		else if (curr == '[') {
			curr = get();
			DEBUG_LEXER_NEXT("[LBRACK]", FORMAT(format));
			return { LBRACK, 0 };
		}

		// ]
		else if (curr == ']') {
			curr = get();
			DEBUG_LEXER_NEXT("[RBRACK]", FORMAT(format));
			return { RBRACK, 0 };
		}

		// {
		else if (curr == '{'){
			curr = get();
			DEBUG_LEXER_NEXT("[LBRACE]", FORMAT(format));
			return { LBRACE, 0 };
		}

		// }
		else if (curr == '}'){
			curr = get();
			DEBUG_LEXER_NEXT("[RBRACE]", FORMAT(format));
			return { RBRACE, 0 };
		}

		// �հ׷�
		else {
			curr = get();
			DEBUG_LEXER_NEXT("[WS]", FORMAT(format));
		}
	}

	//ɨ�����
	//TODO: �ĳ�EOF?
	return { END, 0 };
}
