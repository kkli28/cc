#ifndef __DEBUG_INFO_H__
#define __DEBUG_INFO_H__

/*
�ļ�˵����������Ϣ�࣬���ڼ�¼����ѡ��
*/

#include "stdafx.h"

#define DEBUG_INFO kkli::DebugInfo::getInstance()

namespace kkli {

	//========================================
	// ������Ϣ��
	//========================================

	class DebugInfo {
	public:

		//�Ƿ��ǲ���ģʽ
		bool IS_TEST_MODE;

		//�鿴���ɵĴ���
		bool IS_SHOW_INST_GEN_MODE;

		//�鿴ÿ��ȫ�ֶ�������ָ���ģʽ
		bool IS_SHOW_GLOBAL_DECL_INST_GEN_MODE;

		//�Ƿ�����Warning
		bool ENABLE_WARNING;

		//VM������Ϣ
		bool OUTPUT_VM_ACTIONS;
		bool OUTPUT_VM_EXECUTE_ACTIONS;
		bool OUTPUT_VM_EXECUTE_DETAIL_ACTIONS;

		//Token������Ϣ
		bool OUTPUT_TOKEN_ACTIONS;

		//Symbol������Ϣ
		bool OUTPUT_SYMBOL_TABLE_ACTIONS;

		//Lexer������Ϣ
		bool OUTPUT_LEXER_ACTIONS;
		bool OUTPUT_LEXER_SOURCE_INFO;
		bool OUTPUT_LEXER_SYMBOL_INFO;
		bool OUTPUT_LEXER_NEXT_INFO;

		//Compiler������Ϣ
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
			int i = 0;
			IS_TEST_MODE = infos[i];
			ENABLE_WARNING = infos[++i];
			IS_SHOW_INST_GEN_MODE = infos[++i];
			IS_SHOW_GLOBAL_DECL_INST_GEN_MODE = infos[++i];
			OUTPUT_VM_ACTIONS = infos[++i];
			OUTPUT_VM_EXECUTE_ACTIONS = infos[++i];
			OUTPUT_VM_EXECUTE_DETAIL_ACTIONS = infos[++i];
			OUTPUT_TOKEN_ACTIONS = infos[++i];
			OUTPUT_SYMBOL_TABLE_ACTIONS = infos[++i];
			OUTPUT_LEXER_ACTIONS = infos[++i];
			OUTPUT_LEXER_SOURCE_INFO = infos[++i];
			OUTPUT_LEXER_SYMBOL_INFO = infos[++i];
			OUTPUT_LEXER_NEXT_INFO = infos[++i];
			OUTPUT_COMPILER_ACTIONS = infos[++i];
			OUTPUT_COMPILER_SYMBOL_INFO = infos[++i];
		}

		void reset() {
			set(std::vector<bool>{ false, true, false, false, false, false, false, false,
				false, false, false, false, false, false, false });
		}
		
		void setTestMode() {
			IS_TEST_MODE = true;
		}

		void setInstGenMode() {
			reset();
			IS_SHOW_INST_GEN_MODE = true;
		}

		void setGlobalDeclInstGenMode() {
			reset();
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
			set(std::vector<bool>{IS_TEST_MODE, true, true, true, true, true, true, true, true, true, true, true, true});
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
