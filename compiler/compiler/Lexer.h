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
		std::string sourceCode;  //Դ����
		int index;

	public:
		Lexer(std::string sourceFile);

		//��ȡ��һ���ʷ���Ԫ
		std::pair<TokenType, int> next();

		//ƥ��һ���ʷ���Ԫ
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
