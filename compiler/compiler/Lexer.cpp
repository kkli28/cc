#include "stdafx.h"
#include "Lexer.h"

//构造函数
kkli::Lexer::Lexer(std::string sourceFile, std::string format) {
	if (OUTPUT_LEXER_ACTIONS) {
		Debug::output("Lexer::lexer(" + sourceFile + ")", format);
		Debug::output("[add builtin] begin", FORMAT(format));
	}
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

	if (OUTPUT_LEXER_ACTIONS) {
		Debug::output("[add builtin] end", FORMAT(format));
	}

	if (OUTPUT_LEXER_ACTIONS) {
		Debug::output(table->getSymbolTableInfo(), FORMAT(format));
	}

	std::ifstream inFile(sourceFile);
	inFile >> std::noskipws;    //不跳过空白

	char buff[1000000];
	int i = 0;
	while (!inFile.eof()) inFile >> buff[i++];

	source = std::move(std::string(buff, i));

	//添加末尾字符
	source.push_back('\0');
	source.push_back(END);

	index = 0;
	line = 1;
}

//next
std::pair<int, int> kkli::Lexer::next(std::string format) {
	if (OUTPUT_LEXER_ACTIONS) {
		Debug::output("Lexer::next()", format);
	}

	int value = 0;
	SymbolTable* table = SymbolTable::getInstance();
	VirtualMachine* vm = VirtualMachine::getInstance();

	char curr = source[index];
	while (curr != END) {

		//换行符
		if (curr == '\n') {
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[\\n]", FORMAT(format));
			}

			++line;
		}

		//宏 或 文件引用（直接跳过，不支持）
		else if (curr == '#') {
			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[#...]", FORMAT(format));
			}

			curr = get();
			while (curr != END && curr != '\n')  curr = get();
		}

		//标识符
		else if (isAlpha(curr) || curr == '_') {
			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[ID]", FORMAT(format));
			}
			int begIndex = index;      //该标识符名称的起始索引
			int hash = curr;           //该标识符的hash值
			curr = get();
			while (isAlpha(curr) || isNum(curr) || curr == '_') {
				hash = hash * 147 + curr;
				curr = get();
			}

			std::string name(source, begIndex, index - begIndex);

			//符号表中有该标识符
			if (table->has(hash, name, FORMAT(format))) {
				return { table->getCurrentToken(FORMAT(format)).type, 0 };
			}

			//符号表中没有该标识符，则向其中添加信息
			else {
				Token& tk = table->getCurrentToken(FORMAT(format));
				tk.type = ID;
				tk.name = name;
				tk.hash = hash;

				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("add [type] ID, [name] " + name + ", [hash] " + std::to_string(hash), FORMAT(format));
				}
				return { ID, 0 };
			}
		}

		//数字
		else if (isNum(curr)) {
			value = curr - '0';
			curr = get();

			//十进制数（八进制数与十六进制数以0及0x开始）
			if (value != 0) {
				while (isNum(curr)) {
					value = value * 10 + curr - '0';
					curr = get();
				}

				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[dec] " + std::to_string(value), FORMAT(format));
				}
			}

			else {
				//十六进制数
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
					if (OUTPUT_LEXER_ACTIONS) {
						Debug::output("[hex] " + std::to_string(value), FORMAT(format));
					}
				}

				//八进制数
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
					if (OUTPUT_LEXER_ACTIONS) {
						Debug::output("[oct] " + std::to_string(value), FORMAT(format));
					}
				}
			}

			return { NUM, value };
		}

		//注释 或 除号
		else if (curr == '/') {
			curr = get();

			//单行注释
			if (curr == '/') {
				while (curr != '\n' && curr != END) curr = get();

				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[//]", FORMAT(format));
				}
			}

			//多行注释
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

				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[/**/]", FORMAT(format));
				}
			}

			//除号
			else {
				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[/]", FORMAT(format));
				}

				return { DIV, 0 };
			}
		}

		//字符
		else if (curr == '\'') {
			curr = get();
			if (curr == END) {
				throw Error("Line "+std::to_string(line)+". Invalid char type, need \'.");
			}

			//转义字符
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

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[char]", FORMAT(format));
			}

			return { NUM, value };
		}

		//字符串
		else if (curr == '"') {
			curr = get();

			//记录字符串起始位置
			value = reinterpret_cast<int>(vm->getNextDataPos());

			while (curr != '"' && curr != END) {

				//转义字符
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

			//TODO: 末尾添加0？

			if (curr == END) {
				throw Error("Line " +std::to_string(line)+". Invalid string type, need \" to finish string.");
			}
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[string]", FORMAT(format));
			}

			return { STRING, value };
		}

		// = 或 ==
		else if (curr == '=') {
			curr = get();

			// ==
			if (curr == '=') {
				curr = get();
				
				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[EQ]", FORMAT(format));
				}
				
				return { EQ, 0 };
			}

			// =
			else {
				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[ASSIGN]", FORMAT(format));
				}

				return { ASSIGN, 0 };
			}
		}

		// + 或 ++
		else if (curr == '+') {
			curr = get();

			// ++
			if (curr == '+') {
				curr = get();

				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[INC]", FORMAT(format));
				}

				return { INC, 0 };
			}

			// +
			else {
				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[ADD]", FORMAT(format));
				}

				return { ADD, 0 };
			}
		}

		// - 或 --
		else if (curr == '-') {
			curr = get();

			// --
			if (curr == '-') {
				curr = get();

				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[DEC]", FORMAT(format));
				}

				return { DEC, 0 };
			}

			// -
			else {
				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[SUB]", FORMAT(format));
				}

				return { SUB, 0 };
			}
		}

		// ! 或 !=
		else if (curr == '!') {
			curr = get();

			// !=
			if (curr == '=') {
				curr = get();

				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[NE]", FORMAT(format));
				}

				return { NE, 0 };
			}

			// !
			else {
				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[NOT]", FORMAT(format));
				}

				return { NOT, 0 };
			}
		}

		// < 或 <= 或 <<
		else if (curr == '<') {
			curr = get();

			// <=
			if (curr == '=') {
				curr = get();

				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[LE]", FORMAT(format));
				}

				return { LE, 0 };
			}

			// <<
			else if (curr == '<') {
				curr = get();

				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[SHL]", FORMAT(format));
				}

				return { SHL, 0 };
			}

			// <
			else {
				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[LT]", FORMAT(format));
				}

				return { LT, 0 };
			}
		}

		// > 或 >= 或 >>
		else if (curr == '>') {
			curr = get();

			// >=
			if (curr == '=') {
				curr = get();

				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[GE]", FORMAT(format));
				}

				return { GE, 0 };
			}

			// >>
			else if (curr == '>') {
				curr = get();

				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[SHR]", FORMAT(format));
				}

				return { SHR, 0 };
			}

			// >
			else {
				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[GT]", FORMAT(format));
				}

				return { GT, 0 };
			}
		}

		// | 或 ||
		else if (curr == '|') {
			curr = get();

			// ||
			if (curr == '|') {
				curr = get();

				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[LOR]", FORMAT(format));
				}

				return { LOR, 0 };
			}

			// |
			else {
				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[OR]", FORMAT(format));
				}

				return { OR, 0 };
			}
		}

		// & 或 &&
		else if (curr == '&') {
			curr = get();

			// &&
			if (curr == '&') {
				curr = get();

				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[LAN]", FORMAT(format));
				}

				return { LAN, 0 };
			}

			// &
			else {
				if (OUTPUT_LEXER_ACTIONS) {
					Debug::output("[AND]", FORMAT(format));
				}

				return { AND, 0 };
			}
		}

		// ^
		else if (curr == '^') {
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[XOR]", FORMAT(format));
			}

			return { XOR, 0 };
		}

		// %
		else if (curr == '%') {
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[MOD]", FORMAT(format));
			}

			return { MOD, 0 };
		}

		// *
		else if (curr == '*') {
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[MUL]", FORMAT(format));
			}

			return { MUL, 0 };
		}

		// ?
		else if (curr == '?') {
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[COND]", FORMAT(format));
			}

			return { COND, 0 };
		}

		// ,
		else if (curr == ',') {
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[COMMA]", FORMAT(format));
			}

			return { COMMA, 0 };
		}

		// :
		else if (curr == ':') {
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[COLON]", FORMAT(format));
			}

			return { COLON, 0 };
		}

		// ;
		else if (curr == ';') {
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[SEMICON]", FORMAT(format));
			}

			return { SEMICON, 0 };
		}

		// (
		else if (curr == '(') {
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[LPAREN]", FORMAT(format));
			}

			return { LPAREN, 0 };
		}

		// )
		else if (curr == ')') {
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[RPAREN]", FORMAT(format));
			}
			return { RPAREN, 0 };
		}

		// [
		else if (curr == '[') {
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[LBRACK]", FORMAT(format));
			}

			return { LBRACK, 0 };
		}

		// ]
		else if (curr == ']') {
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[RBRACK]", FORMAT(format));
			}

			return { RBRACK, 0 };
		}

		// {
		else if (curr == '{'){
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[LBRACE]", FORMAT(format));
			}

			return { LBRACE, 0 };
		}

		// }
		else if (curr == '}'){
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[RBRACE]", FORMAT(format));
			}

			return { RBRACE, 0 };
		}

		// 空白符
		else {
			curr = get();

			if (OUTPUT_LEXER_ACTIONS) {
				Debug::output("[WS]", FORMAT(format));
			}
		}
	}

	//扫描结束
	//TODO: 改成EOF?
	return { END, 0 };
}
