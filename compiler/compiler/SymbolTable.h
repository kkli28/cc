#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

/*
文件描述：符号表，记录编译过程中的符号信息
*/

#include "stdafx.h"
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

	public:

		SymbolTable() : currToken() {}

		//判断是否存在符号
		bool has(std::string name);

		//添加符号信息（在词法分析中能够获取的信息）
		void addToken(TokenType type, std::string name, int hash);

		//完善符号信息（在语法分析才能获取的信息）
		void finishToken(TokenKlass klass, DataType dataType, int value);

		//获取当前Token
		Token getCurrToken() const { return currToken; }
	};

	//判断符号是否存在
	bool SymbolTable::has(std::string name) {
		if (OUTPUT_SYMBOL_ACTIONS) {
			Debug::output("SymbolTable::has(" + name + ")");
		}

		//已录入的符号
		for (auto token : table) {
			if (token.name == name) return true;
		}
		//还在处理的符号
		if (name == currToken.name) return true;
		return false;
	}
	
	//添加符号信息
	void SymbolTable::addToken(TokenType type, std::string name, int hash) {
		if (OUTPUT_SYMBOL_ACTIONS) {
			Debug::output("SymbolTable::addToken(" + getTokenTypeName(type) + ", " + name + ", " + std::to_string(hash));
		}

		currToken.clear();

		currToken.type = type;
		currToken.name = name;
		currToken.hash = hash;
	}

	//完善符号信息
	void SymbolTable::finishToken(TokenKlass klass, DataType dataType, int value) {
		if (OUTPUT_SYMBOL_ACTIONS) {
			Debug::output("SymbolTable::finishToken("
				+ getTokenKlassName(klass)
				+ ", " + getDataTypeName(dataType)
				+ ", " + std::to_string(value) + ")");
		}

		currToken.klass = klass;
		currToken.dataType = dataType;
		currToken.value = value;

		table.push_back(currToken);
	}
}

#endif
