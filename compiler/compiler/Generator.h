#ifndef __GENERATOR_H__
#define __GENERATOR_H__

/*
�ļ��������﷨�Ƶ��ķ��룬���õݹ��½����﷨������������ʵ�ִ������ɡ�
*/

#include "stdafx.h"
#include "Grammar.h"  //�ķ�������û���κδ���
#include "Lexer.h"

namespace kkli {

	//========================================
	// Generator: ����������
	//========================================
	class Generator {
	private:
		Lexer lexer;
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
		void statement(std::string format);

		//���ʽ
		void expression(int priority, std::string format);

		//��֤�������ò����ĺϷ���
		void validFunctionCall(const Token& funcToken, const std::vector<int>& dataTypes, std::string format) const;

	public:
		Generator(std::string sourceFile);
		void gen(std::string format);
	};
}

#endif
