#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

/*
�ļ����������ű���¼��������еķ�����Ϣ
*/

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

		SymbolTable() : currToken() {}

	public:

		//����
		static SymbolTable* getInstance() {
			SymbolTable* table = nullptr;
			if (table == nullptr) {
				table = new SymbolTable();
			}
			return table;
		}

		//�ж��Ƿ���ڷ���
		std::pair<bool, TokenType> has(int hash, std::string name);

		//��ӷ�����Ϣ���ڴʷ��������ܹ���ȡ����Ϣ��
		void addToken(TokenType type, std::string name, int hash);

		//���Ʒ�����Ϣ�����﷨�������ܻ�ȡ����Ϣ��
		void finishToken(TokenKlass klass, int dataType, int value);

		//��ȡ��ǰToken
		Token getCurrToken() const { return currToken; }
	};
}

#endif
