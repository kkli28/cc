#include "stdafx.h"
#include "SymbolTable.h"


//判断符号是否存在
std::pair<bool, kkli::TokenType> kkli::SymbolTable::has(int hash, std::string name) {
	if (OUTPUT_SYMBOL_ACTIONS) {
		Debug::output("SymbolTable::has(" + name + ")");
	}

	//已录入的符号
	for (auto token : table) {

		//只比较hash值相同的Token，可有效减少比较字符串的比较次数
		if (token.hash == hash) {
			if (token.name == name) return { true, token.type };
		}
	}

	//还在处理的符号
	if (currToken.hash == hash) {
		if (name == currToken.name) return { true, currToken.type };
	}
	return { false, TokenType::ID };
}

//添加符号信息
void kkli::SymbolTable::addToken(TokenType type, std::string name, int hash) {
	if (OUTPUT_SYMBOL_ACTIONS) {
		Debug::output("SymbolTable::addToken(" + Token::getTokenTypeName(type) + ", " + name + ", " + std::to_string(hash));
	}

	currToken.clear();

	currToken.type = type;
	currToken.name = name;
	currToken.hash = hash;
}

//完善符号信息
void kkli::SymbolTable::finishToken(TokenKlass klass, int dataType, int value) {
	if (OUTPUT_SYMBOL_ACTIONS) {
		Debug::output("SymbolTable::finishToken("
			+ Token::getTokenKlassName(klass)
			+ ", " + Token::getDataTypeName(dataType)
			+ ", " + std::to_string(value) + ")");
	}

	currToken.klass = klass;
	currToken.dataType = dataType;
	currToken.value = value;

	table.push_back(currToken);
}
