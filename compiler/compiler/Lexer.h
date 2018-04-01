#ifndef __LEXER_H__
#define __LEXER_H__

/*
�ļ��������ʷ�����ģ�飬Ϊ�﷨�����ṩһ������ �����ʡ�
*/

#include "SymbolTable.h"
#include "VirtualMachine.h"

namespace kkli {

	//========================================
	// Lexer: �ʷ�����ģ��
	//========================================
	class Lexer {
	private:
		std::string source;  //Դ����
		int index;           //��ǰɨ���λ��
		int line;            //�к�

	private:
		//��ȡ��һ���ַ�
		inline char get() { return source[++index]; }

	public:
		Lexer(std::string sourceFile, std::string format);

		//��ȡ��һ���ʷ���Ԫ
		std::pair<int, int> next(std::string format);

		//��ȡԴ����
		std::string getSouce() const { return source; }
		int getLine() const { return line; }

		//ƥ��һ���ʷ���Ԫ
		//void match(TokenType type);

		//�򵥵Ĺ��ܺ���
		bool isNum(char c) const { return c >= '0' && c <= '9'; }
		bool isWS(char c) const { return c == ' ' || c == '\n' || c == '\t'; }
		bool isAlpha(char c) const {
			return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
		}
	};
}

#endif
