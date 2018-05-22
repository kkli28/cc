#include "stdafx.h"
#include "SymbolTable.h"

//判断符号是否存在
bool kkli::SymbolTable::has(bool isDef, int hash, std::string name, std::string format) {
	DEBUG_SYMBOL_TABLE(std::string("SymbolTable::has(")
		+ (isDef ? "def, " : "use, ") + name + ")", format);

	current = 0;
	auto size = table.size();

	//定义符号，则只能查找当前作用域，无论外部作用域是否已定义该符号。如果没有定义，则添加符号，否则在外部应该报重定义错误
	if (isDef) {
		while (current < size) {
			if (table[current].hash == hash && table[current].name == name) {
				//关键字，则直接判定为找到
				if (table[current].scope[0] == KEY_WORD_SCOPE) {
					return true;
				}
				//当前作用域内找到了该符号，则需要在外部判定符号重定义
				if (table[current].scope == scope) {
					return true;
				}
			}
			++current;
		}
		//新建Token
		table.push_back(Token());
		table[current].scope = scope;
		DEBUG_SYMBOL_TABLE("[new] " + name + " scope: " + getScopeStr(scope), FORMAT(format));
		return false;
	}

	//使用符号，则允许从外部作用域里查找
	//注意，需要逆向查找才能实现内部作用域符号比外部作用域符号优先级高的特点
	else {
		current = size;
		while (--current >= 0) {
			if (table[current].hash == hash && table[current].name == name) {
				//关键字，则直接判定为找到
				if (table[current].scope[0] == KEY_WORD_SCOPE) {
					return true;
				}
				std::vector<int> currScope = scope;
				while (currScope.size() >= table[current].scope.size()) {
					if (table[current].scope == currScope) {
						DEBUG_SYMBOL_TABLE("[has], scope: " + getScopeStr(currScope), FORMAT(format));
						return true;
					}
					currScope.pop_back();
				}
			}
		}
		return false;
	}
}

std::string kkli::SymbolTable::getScopeStr(const std::vector<int> scope) {
	std::string result;
	int size = scope.size();
	if (size == 0) {
		return "/";
	}
	result += std::to_string(scope[0]);
	for (int i = 1; i < size; ++i) {
		result += "/" + std::to_string(scope[i]);
	}
	return result;
}

//进入作用域
void kkli::SymbolTable::enterScope(std::string format) {
	DEBUG_SYMBOL_TABLE("SymbolTable::enterScope()", format);
	++scopeIndex;
	scope.push_back(scopeIndex);

	DEBUG_SYMBOL_TABLE("[curr scope]: " + getScopeStr(scope), FORMAT(format));
}

//离开作用域
void kkli::SymbolTable::leaveScope(std::string format) {
	DEBUG_SYMBOL_TABLE("SymbolTable::leaveScope()", format);

	if (scope.size() == 0) {
		throw Error("SymbolTable::leave(): no scope to leave.");
	}
	scope.pop_back();
	DEBUG_SYMBOL_TABLE("[curr scope]: " + getScopeStr(scope), FORMAT(format));
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
		result += "name: " + table[i].name + "\n";
		result += "hash: " + std::to_string(table[i].hash) + "\n";
		result += "klass: " + Token::getTokenKlassName(table[i].klass) + "\n";
		result += "dataType: " + Token::getDataTypeName(table[i].dataType) + "\n";
		result += "value: " + std::to_string(table[i].value) + "\n";
		result += "scope: " + getScopeStr(table[i].scope) + "\n";
	}
	return std::move(result);
}
