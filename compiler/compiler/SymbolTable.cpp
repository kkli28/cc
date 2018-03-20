#include "stdafx.h"
#include "SymbolTable.h"


//判断符号是否存在
bool kkli::SymbolTable::has(int hash, std::string name) {
	if (OUTPUT_SYMBOL_ACTIONS) {
		Debug::output("SymbolTable::has(" + name + ")");
	}

	current = 0;
	int size = table.size();
	while (current < size) {

		//只比较hash值相同的Token，可有效减少比较字符串的比较次数
		if (table[current].hash == hash) {
			if (table[current].name == name) return true;
		}
		++current;
	}

	table.push_back(Token()); //新建一个Token用于存储信息
	return false;
}

//添加符号信息
void kkli::SymbolTable::addToken(int type, std::string name, int hash) {
	if (OUTPUT_SYMBOL_ACTIONS) {
		Debug::output("SymbolTable::addToken(" + Token::getTokenTypeName(type) + ", " + name + ", " + std::to_string(hash) + ")"
			+ "    current: " + std::to_string(current));
	}

	table[current].type = type;
	table[current].name = name;
	table[current].hash = hash;
}

//完善符号信息
void kkli::SymbolTable::finishToken(int klass, int dataType, int value) {
	if (OUTPUT_SYMBOL_ACTIONS) {
		Debug::output("SymbolTable::finishToken("
			+ Token::getTokenKlassName(klass)
			+ ", " + Token::getDataTypeName(dataType)
			+ ", " + std::to_string(value) + ")" + "    current: " + std::to_string(current));
	}

	table[current].klass = klass;
	table[current].dataType = dataType;
	table[current].value = value;
}

//获取符号表的信息
std::string kkli::SymbolTable::getSymbolTableInfo() const {
	std::string result = "[SymbolTable info]:\n";
	result += "token count: " + std::to_string(table.size()) + "\n";
	result += "token info:\n";
	int i = 0;
	for (auto token : table) {
		result += "Index: " + std::to_string(i++) + "\n";
		result += "type: " + Token::getTokenTypeName(token.type) + "\n";
		result += "klass: " + Token::getTokenKlassName(token.klass) + "\n";
		result += "dataType: " + Token::getDataTypeName(token.dataType) + "\n";
		result += "name: " + token.name + "\n";
		result += "value: " + std::to_string(token.value) + "\n";
		result += "hash: " + std::to_string(token.hash) + "\n\n";
	}
	return std::move(result);
}
