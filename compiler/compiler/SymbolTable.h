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
		int current;
		int mainIndex;  //main����Token������

	public:

		//����
		static SymbolTable* getInstance() {
			static SymbolTable* table = nullptr;
			if (table == nullptr) {
				table = new SymbolTable();
			}
			return table;
		}

		//�ж��Ƿ���ڷ���
		bool has(int hash, std::string name, std::string format);

		//��ȡ��ǰ����
		Token& getCurrentToken(std::string format) { 
			return table[current]; 
		}

		//��ȡ���ű���Ϣ
		std::string getSymbolTableInfo() const;

		//��ȡ�ڲ��ṹtable
		std::vector<Token>& getTable() { return table; }

		//����main����λ��
		void setMainToken(std::string format) {
			if (OUTPUT_SYMBOL_ACTIONS) {
				Debug::output("SymbolTable::setMainToken(), current = " + std::to_string(int(current)), format);
			}
			mainIndex = current;
		}
		Token& getMainToken(std::string format) { 
			if (OUTPUT_SYMBOL_ACTIONS) {
				Debug::output("SymbolTable::getMainToken() " + std::to_string(int(mainIndex)), format);
			}
			return table[mainIndex];
		}
	};
}

#endif
