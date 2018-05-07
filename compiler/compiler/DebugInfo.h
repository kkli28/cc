#ifndef __DEBUG_INFO_H__
#define __DEBUG_INFO_H__

/*
文件说明：调试信息类，用于记录调试选项
*/

#include "stdafx.h"

#define DEBUG_INFO kkli::DebugInfo::getInstance()

namespace kkli {

	//========================================
	// 调试信息类
	//========================================

	class DebugInfo {
	public:
		//共15个

		//是否是测试模式
		bool IS_TEST_MODE;

		//是否启用Warning
		bool ENABLE_WARNING;

		//查看生成的代码
		bool IS_SHOW_INST_GEN_MODE;

		//查看每个全局定义生成指令的模式
		bool IS_SHOW_GLOBAL_DECL_INST_GEN_MODE;

		//VM调试信息
		bool OUTPUT_VM_ACTIONS;
		bool OUTPUT_VM_EXECUTE_ACTIONS;
		bool OUTPUT_VM_EXECUTE_DETAIL_ACTIONS;

		//Token调试信息
		bool OUTPUT_TOKEN_ACTIONS;

		//Symbol调试信息
		bool OUTPUT_SYMBOL_TABLE_ACTIONS;

		//Lexer调试信息
		bool OUTPUT_LEXER_ACTIONS;
		bool OUTPUT_LEXER_SOURCE_INFO;
		bool OUTPUT_LEXER_SYMBOL_INFO;
		bool OUTPUT_LEXER_NEXT_INFO;

		//Compiler调试信息
		bool OUTPUT_COMPILER_ACTIONS;
		bool OUTPUT_COMPILER_SYMBOL_INFO;

		DebugInfo() { reset(); }

	public:

		static DebugInfo* getInstance() {
			static DebugInfo* ptr = nullptr;
			if (ptr == nullptr) {
				ptr = new DebugInfo();
			}
			return ptr;
		}

		void set(std::vector<int> infos) {
			auto itb = [](int i) -> bool { return i; };
			int i = 0;
			IS_TEST_MODE = itb(infos[i]);
			ENABLE_WARNING = itb(infos[++i]);
			IS_SHOW_INST_GEN_MODE = itb(infos[++i]);
			IS_SHOW_GLOBAL_DECL_INST_GEN_MODE = itb(infos[++i]);
			OUTPUT_VM_ACTIONS = itb(infos[++i]);
			OUTPUT_VM_EXECUTE_ACTIONS = itb(infos[++i]);
			OUTPUT_VM_EXECUTE_DETAIL_ACTIONS = itb(infos[++i]);
			OUTPUT_TOKEN_ACTIONS = itb(infos[++i]);
			OUTPUT_SYMBOL_TABLE_ACTIONS = itb(infos[++i]);
			OUTPUT_LEXER_ACTIONS = itb(infos[++i]);
			OUTPUT_LEXER_SOURCE_INFO = itb(infos[++i]);
			OUTPUT_LEXER_SYMBOL_INFO = itb(infos[++i]);
			OUTPUT_LEXER_NEXT_INFO = itb(infos[++i]);
			OUTPUT_COMPILER_ACTIONS = itb(infos[++i]);
			OUTPUT_COMPILER_SYMBOL_INFO = itb(infos[++i]);
		}

		void reset() {
			set(std::vector<int>{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
		}
		
		void setTestMode() {
			IS_TEST_MODE = true;
		}

		void setInstGenMode() {
			//reset();
			IS_SHOW_INST_GEN_MODE = true;
		}

		void setGlobalDeclInstGenMode() {
			//reset();
			IS_SHOW_GLOBAL_DECL_INST_GEN_MODE = true;
		}

		void setDetail1Mode() {
			OUTPUT_COMPILER_ACTIONS = true;
		}

		void setDetail2Mode() {
			OUTPUT_VM_ACTIONS = true;
			OUTPUT_TOKEN_ACTIONS = true;
			OUTPUT_SYMBOL_TABLE_ACTIONS = true;
			OUTPUT_LEXER_ACTIONS = true;
			OUTPUT_LEXER_NEXT_INFO = true;
			OUTPUT_COMPILER_ACTIONS = true;
		}

		void setDetail3Mode() {
			set(std::vector<int>{IS_TEST_MODE, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
		}

		void seeCodeExecute() {
			OUTPUT_VM_EXECUTE_ACTIONS = true;
		}

		void seeCodeExecuteDetail() {
			OUTPUT_VM_EXECUTE_ACTIONS = true;
			OUTPUT_VM_EXECUTE_DETAIL_ACTIONS = true;
		}

		void ignoreWarning() {
			ENABLE_WARNING = false;
		}
	};
}

#endif
