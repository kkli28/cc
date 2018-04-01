#ifndef __LEXER_H__
#define __LEXER_H__

/*
文件描述：词法分析模块，为语法分析提供一个个的 “单词”
*/

#include "SymbolTable.h"
#include "VirtualMachine.h"

namespace kkli {

	//========================================
	// Lexer: 词法分析模块
	//========================================
	class Lexer {
	private:
		std::string source;  //源代码
		int index;           //当前扫描的位置
		int line;            //行号

	private:
		//获取下一个字符
		inline char get() { return source[++index]; }

	public:
		Lexer(std::string sourceFile, std::string format);

		//获取下一个词法单元
		std::pair<int, int> next(std::string format);

		//获取源代码
		std::string getSouce() const { return source; }
		int getLine() const { return line; }

		//匹配一个词法单元
		//void match(TokenType type);

		//简单的功能函数
		bool isNum(char c) const { return c >= '0' && c <= '9'; }
		bool isWS(char c) const { return c == ' ' || c == '\n' || c == '\t'; }
		bool isAlpha(char c) const {
			return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
		}
	};
}

#endif
