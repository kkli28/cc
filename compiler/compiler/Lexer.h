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
		SymbolTable* table;  //符号表
		VirtualMachine* vm;  //虚拟机

		int prevPrevIndex;   //前前扫描的位置
		int prevIndex;       //前一个扫描的位置
		int index;           //当前扫描的位置
		int line;            //行号

	private:
		//获取下一个字符
		inline char get() { return source[++index]; }

		//更新索引
		void updateIndex(std::string format);

	public:
		Lexer(std::string sourceFile, SymbolTable* tb, VirtualMachine* v);

		//获取下一个词法单元
		std::pair<int, int> next(std::string format);

		//为局部变量设计的回退功能
		//局部变量定义int a = b + c; 被识别为int a; a = b + c; 可方便地实现局部变量的声明时定义
		void rollBack(std::string format);

		//获取源代码
		std::string getSouce() const { return source; }
		int getLine() const { return line; }

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
