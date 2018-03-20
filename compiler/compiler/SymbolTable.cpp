#include "stdafx.h"
#include "SymbolTable.h"


//�жϷ����Ƿ����
bool kkli::SymbolTable::has(int hash, std::string name) {
	if (OUTPUT_SYMBOL_ACTIONS) {
		Debug::output("SymbolTable::has(" + name + ")");
	}

	current = 0;
	int size = table.size();
	while (current < size) {

		//ֻ�Ƚ�hashֵ��ͬ��Token������Ч���ٱȽ��ַ����ıȽϴ���
		if (table[current].hash == hash) {
			if (table[current].name == name) return true;
		}
		++current;
	}

	table.push_back(Token()); //�½�һ��Token���ڴ洢��Ϣ
	return false;
}

//��ӷ�����Ϣ
void kkli::SymbolTable::addToken(int type, std::string name, int hash) {
	if (OUTPUT_SYMBOL_ACTIONS) {
		Debug::output("SymbolTable::addToken(" + Token::getTokenTypeName(type) + ", " + name + ", " + std::to_string(hash) + ")"
			+ "    current: " + std::to_string(current));
	}

	table[current].type = type;
	table[current].name = name;
	table[current].hash = hash;
}

//���Ʒ�����Ϣ
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

//��ȡ���ű����Ϣ
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
