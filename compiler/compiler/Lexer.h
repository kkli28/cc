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
		std::string source;  //源代码
		int index;           //当前扫描的位置

	public:
		Lexer(std::string sourceFile);

		//获取下一个词法单元
		std::pair<TokenType, int> next();

		//匹配一个词法单元
		void match(TokenType type);
	};

	Lexer::Lexer(std::string sourceFile) {
		if (OUTPUT_LEXER_ACTIONS) {
			Debug::output("Lexer::Lexer(" + sourceFile + ")");
		}

		std::ifstream inFile(sourceFile);
		inFile >> std::noskipws;    //不跳过空白

		char buff[1000000];
		int i = 0;
		while (!inFile.eof()) inFile >> buff[i++];

		source = std::move(std::string(buff, i));

		if (OUTPUT_LEXER_ACTIONS) {
			Debug::output("    words: " + std::to_string(i));
		}
	}

	//获取下一个词法单元
	std::pair<TokenType, int> Lexer::next() {
		if (OUTPUT_LEXER_ACTIONS) {
			Debug::output("Lexer::next()");
		}

		//TODO:
	}
}

#endif
