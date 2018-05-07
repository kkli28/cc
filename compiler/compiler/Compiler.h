#ifndef __COMPILER_H__
#define __COMPILER_H__

/*
文件描述：编译器
*/

#include "stdafx.h"
#include "Grammar.h"  //文法描述，没有任何代码
#include "Lexer.h"

namespace kkli {

	//========================================
	// Compiler: 编译器
	//========================================
	class Compiler {
	private:
		Lexer* lexer;
		SymbolTable* table;
		VirtualMachine* vm;
		std::pair<int, int> tokenInfo;
		size_t currFuncIndex;

		//当前处理的定义的类型
		int baseType;

		//处理的表达式类型
		int exprType;

		//函数参数与局部变量相对于bp的位置
		int indexOfBP;

	private:
		void match(int type, std::string format);
		void matchAny(std::string format);

		//全局定义
		void global_decl(std::string format);

		//enum定义
		void enum_decl(std::string format);

		//函数定义
		void func_decl(std::string format);

		//全局变量定义
		void global_var_decl(int dataType, std::string format);

		//全局数组定义
		void global_arr_decl(int dataType, std::string format);

		//函数参数
		void func_param(std::string format);

		//函数体
		void func_body(std::string format);

		//语句
		void statement(int& variableIndex, std::string format);

		//局部变量定义
		void local_var_decl(int& variableIndex, std::string format);

		//表达式
		void expression(int priority, std::string format);

		//验证函数调用参数的合法性
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
