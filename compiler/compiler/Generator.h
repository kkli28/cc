#ifndef __GENERATOR_H__
#define __GENERATOR_H__

/*
文件描述：语法制导的翻译，采用递归下降的语法分析技术，并实现代码生成。
*/

#include "stdafx.h"
#include "Grammar.h"  //文法描述，没有任何代码
#include "Lexer.h"

namespace kkli {

	//========================================
	// Generator: 代码生成器
	//========================================
	class Generator {
	private:
		Lexer lexer;
		SymbolTable* table;
		VirtualMachine* vm;
		std::pair<int, int> tokenInfo;

		//当前处理的定义的类型
		int baseType;

	private:
		void match(int type);

		//全局定义
		void global_decl();

		//enum定义
		void enum_decl();

		//函数定义
		void func_decl();

	public:
		Generator(std::string sourceFile);
		void run();
	};
}

#endif
