#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

/*
文件描述：符号表，记录编译过程中的符号信息
*/

#include "Token.h"

namespace kkli {

	//========================================
	// 符号表
	//========================================

	/*
	table为已添加完成的符号
	currToken为正在完善的符号，其信息的填写跨越词法分析和语法分析，无法在
	一趟内就填写完成，因此需要单独存储，当信息填写完整后，再放入符号表
	*/
	class SymbolTable {
	private:
		std::vector<Token> table;
		Token currToken;      //当前正在处理的Token

		SymbolTable() : currToken() {}

	public:

		//单例
		static SymbolTable* getInstance() {
			SymbolTable* table = nullptr;
			if (table == nullptr) {
				table = new SymbolTable();
			}
			return table;
		}

		//判断是否存在符号
		std::pair<bool, TokenType> has(int hash, std::string name);

		//添加符号信息（在词法分析中能够获取的信息）
		void addToken(TokenType type, std::string name, int hash);

		//完善符号信息（在语法分析才能获取的信息）
		void finishToken(TokenKlass klass, int dataType, int value);

		//获取当前Token
		Token getCurrToken() const { return currToken; }
	};
}

#endif
