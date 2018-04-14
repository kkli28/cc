#include "stdafx.h"
#include "SymbolTable.h"

//判断符号是否存在
bool kkli::SymbolTable::has(int hash, std::string name, std::string format) {
	DEBUG_SYMBOL_TABLE("SymbolTable::has(" + name + ")", format);

	current = 0;
	auto size = table.size();

	while (current < size) {

		//只比较hash值相同的Token，可有效减少比较字符串的比较次数
		if (table[current].hash == hash) {
			if (table[current].name == name) {
				DEBUG_SYMBOL_TABLE("[has]", FORMAT(format));

				return true;
			}
		}
		++current;
	}

	table.push_back(Token()); //新建一个Token用于存储信息

	DEBUG_SYMBOL_TABLE("[new]", FORMAT(format));
	return false;
}

//重置所有信息
void kkli::SymbolTable::reset(std::string format) {
	DEBUG_SYMBOL_TABLE("SymbolTable::reset()", format);
	table.clear();
	current = 0;
	mainIndex = 0;
}

//获取符号表的信息
std::string kkli::SymbolTable::getSymbolTableInfo() const {
	std::string result = "\n[SymbolTable info]:\n";
	result += "[size]: " + std::to_string(table.size()) + "\n";
	result += "[info]:\n";
	result += "[addr]: " + std::to_string(int(&table[0])) + "\n";
	int size = table.size();
	for (int i = 0; i < size; ++i) {
		result += "\nIndex: " + std::to_string(i) + "\n";
		result += "[addr]: " + std::to_string(int(&table[i])) + "\n";
		result += "type: " + Token::getTokenTypeName(table[i].type) + "\n";
		result += "klass: " + Token::getTokenKlassName(table[i].klass) + "\n";
		result += "name: " + table[i].name + "\n";
		result += "dataType: " + Token::getDataTypeName(table[i].dataType) + "\n";
		result += "value: " + std::to_string(table[i].value) + "\n";
		result += "hash: " + std::to_string(table[i].hash) + "\n";
		result += "Bklass: " + Token::getTokenKlassName(table[i].Bklass) + "\n";
		result += "BdataTpe: " + Token::getDataTypeName(table[i].BdataType) + "\n";
		result += "Bvalue: " + std::to_string(table[i].value) + "\n";
	}
	return std::move(result);
}
