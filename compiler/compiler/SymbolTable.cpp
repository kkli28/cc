#include "stdafx.h"
#include "SymbolTable.h"


//�жϷ����Ƿ����
std::pair<bool, kkli::TokenType> kkli::SymbolTable::has(int hash, std::string name) {
	if (OUTPUT_SYMBOL_ACTIONS) {
		Debug::output("SymbolTable::has(" + name + ")");
	}

	//��¼��ķ���
	for (auto token : table) {

		//ֻ�Ƚ�hashֵ��ͬ��Token������Ч���ٱȽ��ַ����ıȽϴ���
		if (token.hash == hash) {
			if (token.name == name) return { true, token.type };
		}
	}

	//���ڴ���ķ���
	if (currToken.hash == hash) {
		if (name == currToken.name) return { true, currToken.type };
	}
	return { false, TokenType::ID };
}

//��ӷ�����Ϣ
void kkli::SymbolTable::addToken(TokenType type, std::string name, int hash) {
	if (OUTPUT_SYMBOL_ACTIONS) {
		Debug::output("SymbolTable::addToken(" + Token::getTokenTypeName(type) + ", " + name + ", " + std::to_string(hash));
	}

	currToken.clear();

	currToken.type = type;
	currToken.name = name;
	currToken.hash = hash;
}

//���Ʒ�����Ϣ
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
