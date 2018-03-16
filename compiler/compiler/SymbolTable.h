#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

/*
�ļ����������ű���¼��������еķ�����Ϣ
*/

#include "stdafx.h"
#include "Token.h"

namespace kkli {

	//========================================
	// ���ű�
	//========================================

	/*
	tableΪ�������ɵķ���
	currTokenΪ�������Ƶķ��ţ�����Ϣ����д��Խ�ʷ��������﷨�������޷���
	һ���ھ���д��ɣ������Ҫ�����洢������Ϣ��д�������ٷ�����ű�
	*/
	class SymbolTable {
	private:
		std::vector<Token> table;
		Token currToken;      //��ǰ���ڴ����Token

	public:

		SymbolTable() : currToken() {}

		//�ж��Ƿ���ڷ���
		bool has(std::string name);

		//��ӷ�����Ϣ���ڴʷ��������ܹ���ȡ����Ϣ��
		void addToken(TokenType type, std::string name, int hash);

		//���Ʒ�����Ϣ�����﷨�������ܻ�ȡ����Ϣ��
		void finishToken(TokenKlass klass, DataType dataType, int value);

		//��ȡ��ǰToken
		Token getCurrToken() const { return currToken; }
	};

	//�жϷ����Ƿ����
	bool SymbolTable::has(std::string name) {
		if (OUTPUT_SYMBOL_ACTIONS) {
			Debug::output("SymbolTable::has(" + name + ")");
		}

		//��¼��ķ���
		for (auto token : table) {
			if (token.name == name) return true;
		}
		//���ڴ���ķ���
		if (name == currToken.name) return true;
		return false;
	}
	
	//��ӷ�����Ϣ
	void SymbolTable::addToken(TokenType type, std::string name, int hash) {
		if (OUTPUT_SYMBOL_ACTIONS) {
			Debug::output("SymbolTable::addToken(" + getTokenTypeName(type) + ", " + name + ", " + std::to_string(hash));
		}

		currToken.clear();

		currToken.type = type;
		currToken.name = name;
		currToken.hash = hash;
	}

	//���Ʒ�����Ϣ
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
