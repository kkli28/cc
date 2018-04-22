#ifndef __DEBUG_INFO_H__
#define __DEBUG_INFO_H__

#include "stdafx.h"

#define DEBUG_INFO kkli::DebugInfo::getInstance()

namespace kkli {

	//========================================
	// 调试信息类
	//========================================

	class DebugInfo {
	public:

		//是否是测试模式
		bool IS_TEST_MODE;

		//是否启用Warning
		bool ENABLE_WARNING;

		//是否开启输出执行的语句
		bool ENABLE_OUTPUT_EXECUTE_INSTRUCTION;

		//VM调试信息
		bool OUTPUT_VM_ACTIONS;
		bool OUTPUT_VM_EXECUTE_ACTIONS;

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

		void set(std::vector<bool> infos) {
			IS_TEST_MODE = infos[0];
			ENABLE_WARNING = infos[1];
			ENABLE_OUTPUT_EXECUTE_INSTRUCTION = infos[2];
			OUTPUT_VM_ACTIONS = infos[3];
			OUTPUT_VM_EXECUTE_ACTIONS = infos[4];
			OUTPUT_TOKEN_ACTIONS = infos[5];
			OUTPUT_SYMBOL_TABLE_ACTIONS = infos[6];
			OUTPUT_LEXER_ACTIONS = infos[7];
			OUTPUT_LEXER_SOURCE_INFO = infos[8];
			OUTPUT_LEXER_SYMBOL_INFO = infos[9];
			OUTPUT_LEXER_NEXT_INFO = infos[10];
			OUTPUT_COMPILER_ACTIONS = infos[11];
			OUTPUT_COMPILER_SYMBOL_INFO = infos[12];
		}

		void reset() {
			set(std::vector<bool>{ false, true, false, false, false, false,
				false, false, false, false, false, false, false });
		}
		
		void setTestMode() {
			IS_TEST_MODE = true;
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
			set(std::vector<bool>{IS_TEST_MODE, true, true, true, true, true, true, true, true, true, true, true, true});
		}

		void seeCodeExecute() {
			ENABLE_OUTPUT_EXECUTE_INSTRUCTION = true;
		}

		void seeCodeExecuteDetail() {
			ENABLE_OUTPUT_EXECUTE_INSTRUCTION = true;
			OUTPUT_VM_EXECUTE_ACTIONS = true;
		}

		void ignoreWarning() {
			ENABLE_WARNING = false;
		}
	};
}

#endif
