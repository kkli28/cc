#include "stdafx.h"
#include "SymbolTable.h"

//�жϷ����Ƿ����
bool kkli::SymbolTable::has(bool isDef, int hash, std::string name, std::string format) {
	DEBUG_SYMBOL_TABLE(std::string("SymbolTable::has(")
		+ (isDef ? "def, " : "use, ") + name + ")", format);

	current = 0;
	auto size = table.size();

	//������ţ���ֻ�ܲ��ҵ�ǰ�����������ⲿ�������Ƿ��Ѷ���÷��š����û�ж��壬����ӷ��ţ��������ⲿӦ�ñ��ض������
	if (isDef) {
		while (current < size) {
			if (table[current].hash == hash && table[current].name == name) {
				//�ؼ��֣���ֱ���ж�Ϊ�ҵ�
				if (table[current].scope[0] == KEY_WORD_SCOPE) {
					return true;
				}
				//��ǰ���������ҵ��˸÷��ţ�����Ҫ���ⲿ�ж������ض���
				if (table[current].scope == scope) {
					return true;
				}
			}
			++current;
		}
		//�½�Token
		table.push_back(Token());
		table[current].scope = scope;
		DEBUG_SYMBOL_TABLE("[new] " + name + " scope: " + getScopeStr(scope), FORMAT(format));
		return false;
	}

	//ʹ�÷��ţ���������ⲿ�����������
	//ע�⣬��Ҫ������Ҳ���ʵ���ڲ���������ű��ⲿ������������ȼ��ߵ��ص�
	else {
		current = size;
		while (--current >= 0) {
			if (table[current].hash == hash && table[current].name == name) {
				//�ؼ��֣���ֱ���ж�Ϊ�ҵ�
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

//����������
void kkli::SymbolTable::enterScope(std::string format) {
	DEBUG_SYMBOL_TABLE("SymbolTable::enterScope()", format);
	++scopeIndex;
	scope.push_back(scopeIndex);

	DEBUG_SYMBOL_TABLE("[curr scope]: " + getScopeStr(scope), FORMAT(format));
}

//�뿪������
void kkli::SymbolTable::leaveScope(std::string format) {
	DEBUG_SYMBOL_TABLE("SymbolTable::leaveScope()", format);

	if (scope.size() == 0) {
		throw Error("SymbolTable::leave(): no scope to leave.");
	}
	scope.pop_back();
	DEBUG_SYMBOL_TABLE("[curr scope]: " + getScopeStr(scope), FORMAT(format));
}

//��ȡ���ű����Ϣ
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
