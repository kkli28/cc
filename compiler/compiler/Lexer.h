#ifndef __LEXER_H__
#define __LEXER_H__

/*
文件描述：词法分析模块，为语法分析提供一个个的 “单词”
*/

#include "stdafx.h"
#include "Token.h"

namespace kkli {

	//========================================
	// Lexer: 词法分析模块
	//========================================
	class Lexer {
	private:
		std::string sourceCode;  //源代码
		int index;

	public:
		Lexer(std::string sourceFile);

		//获取下一个词法单元
		std::pair<TokenType, int> next();

		//匹配一个词法单元
		void match(TokenType type);
	};

	Lexer::Lexer(std::string sourceFile) {
		std::ifstream inFile(sourceFile);
		char buff[100000];
		inFile.read(buff, 100000);
		sourceCode.assign(buff);

		std::cout << sourceCode.size() << std::endl;
		index = 0;
	}
}

#endif
