#ifndef __LEXER_H__
#define __LEXER_H__

/*
文件描述：词法分析模块，为语法分析提供一个个的 “单词”
*/

#include "SymbolTable.h"
#include "VirtualMachine.h"

namespace kkli {

	//========================================
	// Lexer: 词法分析模块
	//========================================
	class Lexer {
	private:
		std::string source;  //源代码
		std::string sourceBackUp;  //源代码备份，因为局部变量定义会修改源代码，以便正确获得每个全局定义的源代码
		SymbolTable* table;  //符号表
		VirtualMachine* vm;  //虚拟机

		int prevPrevIndex;   //前前扫描的位置
		int prevIndex;       //前一个扫描的位置
		int index;           //当前扫描的位置
		int line;            //行号

		int gdBegIndex;      //前一个全局定义的开始位置
		int gdEndIndex;      //xxx结束位置

		/*
		若是定义语句如 int a = 1; 则识别 a 时若在当前scope中没有找到名字
		a，则添加Token，否则返回该a。

		若不是定义语句如 a = 1; 则识别 a 时，若当前scope中没有找到名字，则
		从外部scope中找，不会添加Token。
		*/
		bool isDefinition;   //标记当前解析的语句是否是定义语句

	private:
		//获取下一个字符
		inline char get() { return source[++index]; }

		//更新索引
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

		//获取下一个词法单元
		std::pair<int, int> next(std::string format);

		//设置当前分析的是定义语句
		void setIsDefinition(bool isDef, std::string format) { 
			DEBUG_LEXER(std::string("Lexer::setIsDefinition(") + (isDef ? "true" : "false") + ")", format);
			isDefinition = isDef; 
		}

		//为局部变量设计的回退功能
		//局部变量定义int a = b + c; 被识别为int a; a = b + c; 可方便地实现局部变量的声明时定义
		void rollBack(std::string format);

		//获取源代码
		std::string getSouce() const { return sourceBackUp; }
		int getLine() const { return line; }

		//设置全局定义开始位置
		void setGlobalDeclTag(bool isStart) {
			isStart ? gdBegIndex = prevIndex : gdEndIndex = prevIndex;
		}

		//获取前一个全局定义
		std::string getGlobalDecl() {
			return std::move(std::string(sourceBackUp.begin() + gdBegIndex, sourceBackUp.begin() + gdEndIndex));
		}

		//匹配一个词法单元
		//void match(TokenType type);

		//简单的功能函数
		bool isNum(char c) const { return c >= '0' && c <= '9'; }
		bool isWS(char c) const { return c == ' ' || c == '\n' || c == '\t'; }
		bool isAlpha(char c) const {
			return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
		}
	};
}

#endif
