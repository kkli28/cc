#ifndef __LEXER_H__
#define __LEXER_H__

/*
�ļ��������ʷ�����ģ�飬Ϊ�﷨�����ṩһ������ �����ʡ�
*/

#include "stdafx.h"
#include "Token.h"

namespace kkli {

	//========================================
	// Lexer: �ʷ�����ģ��
	//========================================
	class Lexer {
	private:
		std::string source;  //Դ����
		int index;           //��ǰɨ���λ��

	public:
		Lexer(std::string sourceFile);

		//��ȡ��һ���ʷ���Ԫ
		std::pair<TokenType, int> next();

		//ƥ��һ���ʷ���Ԫ
		void match(TokenType type);
	};

	Lexer::Lexer(std::string sourceFile) {
		if (OUTPUT_LEXER_ACTIONS) {
			Debug::output("Lexer::Lexer(" + sourceFile + ")");
		}

		std::ifstream inFile(sourceFile);
		inFile >> std::noskipws;    //�������հ�

		char buff[1000000];
		int i = 0;
		while (!inFile.eof()) inFile >> buff[i++];

		source = std::move(std::string(buff, i));

		if (OUTPUT_LEXER_ACTIONS) {
			Debug::output("    words: " + std::to_string(i));
		}
	}

	//��ȡ��һ���ʷ���Ԫ
	std::pair<TokenType, int> Lexer::next() {
		if (OUTPUT_LEXER_ACTIONS) {
			Debug::output("Lexer::next()");
		}

		//TODO:
	}
}

#endif
