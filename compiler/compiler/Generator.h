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

		//��ǰ����Ķ��������
		int baseType;

		//����������ֲ����������bp��λ��
		int indexOfBP;

	private:
		void match(int type);

		//ȫ�ֶ���
		void global_decl();

		//enum����
		void enum_decl();

		//��������
		void func_decl();

		//��������
		void func_param();

		//������
		void func_body();

	public:
		Generator(std::string sourceFile);
		void run();
	};
}

#endif
