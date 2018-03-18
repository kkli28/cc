#include "stdafx.h"
#include "Lexer.h"

//���캯��
kkli::Lexer::Lexer(std::string sourceFile) {
	if (OUTPUT_LEXER_ACTIONS) {
		Debug::output("Lexer::Lexer(" + sourceFile + ")");
	}

	std::ifstream inFile(sourceFile);
	inFile >> std::noskipws;    //�������հ�

	char buff[1000000];
	int i = 0;
	while (!inFile.eof()) inFile >> buff[i++];

	source = std::move(std::string(buff, i));
	source[source.size() - 1] = eof;  //��ӽ�β�ַ�

	index = 0;
	line = 1;

	if (OUTPUT_LEXER_ACTIONS) {
		Debug::output("    words: " + std::to_string(i));
		Debug::output("    end word: " + std::to_string(source[source.size() - 1]));
	}
}

//next
std::pair<kkli::TokenType, int> kkli::Lexer::next() {
	int value = 0;
	SymbolTable* table = SymbolTable::getInstance();
	VirtualMachine* vm = VirtualMachine::getInstance();

	char curr = source[index];
	while (curr != eof) {

		//���з�
		if (curr == '\n') {
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [\\n]");
			}

			++line;
		}

		//�� �� �ļ����ã�ֱ����������֧�֣�
		else if (curr == '#') {
			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [#...]");
			}

			curr = get();
			while (curr != eof && curr != '\n')  curr = get();
		}

		//��ʶ��
		else if (isAlpha(curr) || curr == '_') {
			
			int begIndex = index;      //�ñ�ʶ�����Ƶ���ʼ����
			int hash = curr;           //�ñ�ʶ����hashֵ
			curr = get();
			while (isAlpha(curr) || isNum(curr) || curr == '_') {
				hash = hash * 147 + curr;
				curr = get();
			}

			std::string name(source, begIndex, index - begIndex);

			//����ű�
			auto info = table->has(hash, name);

			//���ű����иñ�ʶ��
			if (info.first) {
				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [id] " + name + " [has]");
				}

				return { info.second, 0 };
			}

			//���ű���û�иñ�ʶ�����������������Ϣ
			else {
				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [id] " + name + " [add]");
				}

				table->addToken(TokenType::ID, name, hash);
				return { TokenType::ID, 0 };
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

				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [dec] " + std::to_string(value));
				}
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
					if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
						Debug::output("Lexer::next(): [hex] " + std::to_string(value));
					}
				}

				//�˽�����
				else {
					bool hasNum = false;
					while (curr >= '0' && curr <= '7') {
						hasNum = true;
						value = value * 8 + curr - '0';
						curr = get();
					}

					if (!hasNum) {
						throw Error("Line " + std::to_string(line) + ". Invalid oct type, need 0~7 after 0.");
					}
					if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
						Debug::output("Lexer::next(): [oct] " + std::to_string(value));
					}
				}
			}

			return { TokenType::NUM, value };
		}

		//ע�� �� ����
		else if (curr == '/') {
			curr = get();

			//����ע��
			if (curr == '/') {
				while (curr != '\n' && curr != eof) curr = get();

				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [//]");
				}
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
					else if (curr == eof) break;
					else curr = get();
				}

				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [/**/]");
				}
			}

			//����
			else {
				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [/]");
				}

				return { TokenType::DIV, 0 };
			}
		}

		//�ַ�
		else if (curr == '\'') {
			curr = get();
			if (curr == eof) {
				throw Error("Line "+std::to_string(line)+". Invalid char type, need \'.");
			}

			//ת���ַ�
			if (curr == '\\') {
				curr = get();
				if (curr == eof) {
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

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [char]");
			}

			return { TokenType::NUM, value };
		}

		//�ַ���
		else if (curr == '"') {
			curr = get();

			//��¼�ַ�����ʼλ��
			value = reinterpret_cast<int>(vm->getNextDataPos());

			while (curr != '"' && curr != eof) {

				//ת���ַ�
				if (curr == '\\') {
					curr = get();
					if (curr == 'n') vm->addData('\n');
					else if (curr == eof) {
						throw Error("Line " + std::to_string(line) + ". Invalid string type, need escape charactor after \\.");
					}
					else vm->addData(curr);
					curr = get();
				}
				else {
					vm->addData(curr);
					curr = get();
				}
			}
			if (curr == eof) {
				throw Error("Line " +std::to_string(line)+". Invalid string type, need \" to finish string.");
			}
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [string]");
			}

			return { TokenType::STRING, value };
		}

		// = �� ==
		else if (curr == '=') {
			curr = get();

			// ==
			if (curr == '=') {
				curr = get();
				
				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [EQ]");
				}
				
				return { TokenType::EQ, 0 };
			}

			// =
			else {
				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [ASSIGN]");
				}

				return { TokenType::ASSIGN, 0 };
			}
		}

		// + �� ++
		else if (curr == '+') {
			curr = get();

			// ++
			if (curr == '+') {
				curr = get();

				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [INC]");
				}

				return { TokenType::INC, 0 };
			}

			// +
			else {
				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [ADD]");
				}

				return { TokenType::ADD, 0 };
			}
		}

		// - �� --
		else if (curr == '-') {
			curr = get();

			// --
			if (curr == '-') {
				curr = get();

				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [DEC]");
				}

				return { TokenType::DEC, 0 };
			}

			// -
			else {
				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [SUB]");
				}

				return { TokenType::SUB, 0 };
			}
		}

		// ! �� !=
		else if (curr == '!') {
			curr = get();

			// !=
			if (curr == '=') {
				curr = get();

				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [NE]");
				}

				return { TokenType::NE, 0 };
			}

			// !
			else {
				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [NOT]");
				}

				return { TokenType::NOT, 0 };
			}
		}

		// < �� <= �� <<
		else if (curr == '<') {
			curr = get();

			// <=
			if (curr == '=') {
				curr = get();

				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [LE]");
				}

				return { TokenType::LE, 0 };
			}

			// <<
			else if (curr == '<') {
				curr = get();

				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [SHL]");
				}

				return { TokenType::SHL, 0 };
			}

			// <
			else {
				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [LT]");
				}

				return { TokenType::LT, 0 };
			}
		}

		// > �� >= �� >>
		else if (curr == '>') {
			curr = get();

			// >=
			if (curr == '=') {
				curr = get();

				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [GE]");
				}

				return { TokenType::GE, 0 };
			}

			// >>
			else if (curr == '>') {
				curr = get();

				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [SHR]");
				}

				return { TokenType::SHR, 0 };
			}

			// >
			else {
				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [GT]");
				}

				return { TokenType::GT, 0 };
			}
		}

		// | �� ||
		else if (curr == '|') {
			curr = get();

			// ||
			if (curr == '|') {
				curr = get();

				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [LOR]");
				}

				return { TokenType::LOR, 0 };
			}

			// |
			else {
				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [OR]");
				}

				return { TokenType::OR, 0 };
			}
		}

		// & �� &&
		else if (curr == '&') {
			curr = get();

			// &&
			if (curr == '&') {
				curr = get();

				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [LAN]");
				}

				return { TokenType::LAN, 0 };
			}

			// &
			else {
				if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
					Debug::output("Lexer::next(): [AND]");
				}

				return { TokenType::AND, 0 };
			}
		}

		// ^
		else if (curr == '^') {
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [XOR]");
			}

			return { TokenType::XOR, 0 };
		}

		// %
		else if (curr == '%') {
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [MOD]");
			}

			return { TokenType::MOD, 0 };
		}

		// *
		else if (curr == '*') {
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [MUL]");
			}

			return { TokenType::MUL, 0 };
		}

		// ?
		else if (curr == '?') {
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [COND]");
			}

			return { TokenType::COND, 0 };
		}

		// ,
		else if (curr == ',') {
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [COMMA]");
			}

			return { TokenType::COMMA, 0 };
		}

		// :
		else if (curr == ':') {
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [COLON]");
			}

			return { TokenType::COLON, 0 };
		}

		// ;
		else if (curr == ';') {
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [SEMICON]");
			}

			return { TokenType::SEMICON, 0 };
		}

		// (
		else if (curr == '(') {
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [LPAREN]");
			}

			return { TokenType::LPAREN, 0 };
		}

		// )
		else if (curr == ')') {
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [RPAREN]");
			}

			return { TokenType::RPAREN, 0 };
		}

		// [
		else if (curr == '[') {
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [LBRACK]");
			}

			return { TokenType::LBRACK, 0 };
		}

		// ]
		else if (curr == ']') {
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [RBRACK]");
			}

			return { TokenType::RBRACK, 0 };
		}

		// {
		else if (curr == '{'){
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [LBRACE]");
			}

			return { TokenType::LBRACE, 0 };
		}

		// }
		else if (curr == '}'){
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [RBRACE]");
			}

			return { TokenType::RBRACE, 0 };
		}

		// �հ׷�
		else {
			curr = get();

			if (OUTPUT_LEXER_FUNC_NEXT_DETAIL) {
				Debug::output("Lexer::next(): [WS]");
			}
		}
	}

	//ɨ�����
	return { TokenType::END, 0 };
}
