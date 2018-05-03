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
		std::string sourceBackUp;  //Դ���뱸�ݣ���Ϊ�ֲ�����������޸�Դ���룬�Ա���ȷ���ÿ��ȫ�ֶ����Դ����
		SymbolTable* table;  //���ű�
		VirtualMachine* vm;  //�����

		int prevPrevIndex;   //ǰǰɨ���λ��
		int prevIndex;       //ǰһ��ɨ���λ��
		int index;           //��ǰɨ���λ��
		int line;            //�к�

		int gdBegIndex;      //ǰһ��ȫ�ֶ���Ŀ�ʼλ��
		int gdEndIndex;      //xxx����λ��

	private:
		//��ȡ��һ���ַ�
		inline char get() { return source[++index]; }

		//��������
		void updateIndex(std::string format);

		//DEBUG
		void DEBUG_LEXER(std::string arg1, std::string arg2) {
			if (DEBUG_INFO->OUTPUT_LEXER_ACTIONS) {
				Debug::output(arg1, arg2);
			}
		}
		void DEBUG_LEXER_SOURCE(std::string arg1, std::string arg2) {
			if (DEBUG_INFO->OUTPUT_LEXER_SOURCE_INFO) {
				Debug::output(arg1, arg2); 
			}
		}
		void DEBUG_LEXER_SYMBOL_TABLE(std::string arg1, std::string arg2) {
			if (DEBUG_INFO->OUTPUT_LEXER_SYMBOL_INFO) {
				Debug::output(arg1, arg2);
			}
		}
		void DEBUG_LEXER_NEXT(std::string arg1, std::string arg2) {
			if (DEBUG_INFO->OUTPUT_LEXER_NEXT_INFO) {
				Debug::output(arg1, arg2);
			}
		}

	public:
		Lexer(std::string sourceFile, SymbolTable* tb, VirtualMachine* v);

		//��ȡ��һ���ʷ���Ԫ
		std::pair<int, int> next(std::string format);

		//Ϊ�ֲ�������ƵĻ��˹���
		//�ֲ���������int a = b + c; ��ʶ��Ϊint a; a = b + c; �ɷ����ʵ�־ֲ�����������ʱ����
		void rollBack(std::string format);

		//��ȡԴ����
		std::string getSouce() const { return sourceBackUp; }
		int getLine() const { return line; }

		//����ȫ�ֶ��忪ʼλ��
		void setGlobalDeclTag(bool isStart) {
			isStart ? gdBegIndex = prevIndex : gdEndIndex = prevIndex;
		}

		//��ȡǰһ��ȫ�ֶ���
		std::string getGlobalDecl() {
			return std::move(std::string(sourceBackUp.begin() + gdBegIndex, sourceBackUp.begin() + gdEndIndex));
		}

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
