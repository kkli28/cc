#ifndef __COMPILER_H__
#define __COMPILER_H__

/*
�ļ�������������
*/

#include "stdafx.h"
#include "Grammar.h"  //�ķ�������û���κδ���
#include "Lexer.h"

namespace kkli {

	//========================================
	// Compiler: ������
	//========================================
	class Compiler {
	private:
		Lexer* lexer;
		SymbolTable* table;
		VirtualMachine* vm;
		std::pair<int, int> tokenInfo;
		size_t currFuncIndex;

		//��ǰ����Ķ��������
		int baseType;

		//����ı��ʽ����
		int exprType;

		//����������ֲ����������bp��λ��
		int indexOfBP;

	private:
		void match(int type, std::string format);
		void matchAny(std::string format);

		//ȫ�ֶ���
		void global_decl(std::string format);

		//enum����
		void enum_decl(std::string format);

		//��������
		void func_decl(std::string format);

		//ȫ�ֱ�������
		void global_var_decl(int dataType, std::string format);

		//ȫ�����鶨��
		void global_arr_decl(int dataType, std::string format);

		//��������
		void func_param(std::string format);

		//������
		void func_body(std::string format);

		//���
		void statement(int& variableIndex, std::string format);

		//�ֲ���������
		void local_var_decl(int& variableIndex, std::string format);

		//���ʽ
		void expression(int priority, std::string format);

		//��֤�������ò����ĺϷ���
		void validFunctionCall(const Token& funcToken, const std::vector<int>& dataTypes, std::string format) const;

		//DEBUG
		void DEBUG_COMPILER(std::string arg1, std::string arg2) const {
			if (DEBUG_INFO->OUTPUT_COMPILER_ACTIONS) {
				Debug::output(arg1, arg2); 
			}
		}
		void DEBUG_COMPILER_SYMBOL(std::string arg1, std::string arg2) const {
			if (DEBUG_INFO->OUTPUT_COMPILER_SYMBOL_INFO) {
				Debug::output(arg1, arg2);
			}
		}

	public:
		Compiler(std::string sourceFile);
		~Compiler() {
			delete lexer;
			delete table;
			delete vm;
		}

		//Go Go GO!
		void run();
	};
}

#endif
