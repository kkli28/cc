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
		size_t current;
		size_t mainIndex;  //main����Token������

	public:

		//����
		static SymbolTable* getInstance() {
			static SymbolTable* table = nullptr;
			if (table == nullptr) {
				table = new SymbolTable();
			}
			return table;
		}

		//����������Ϣ
		void reset(std::string format);

		//�ж��Ƿ���ڷ���
		bool has(int hash, std::string name, std::string format);

		//��ȡ��ǰ���ţ�ע�⣡��֮������push_back����token�����ý����ܲ�����Ч��
		Token& getCurrentToken(std::string format) { 
			return table[current]; 
		}
		size_t getCurrent(std::string format) {  //ֻ��ȡcurrentӦ���Ǹ��õ�ѡ��
			return current;
		}

		//��ȡToken
		Token& getToken(size_t index) {
			if (index < 0 || index >= table.size()) {
				throw Error("SymbolTable::getToken(" + std::to_string(index) + "), invalid index, table size is " + std::to_string(table.size()) + ".");
			}
			return table[index];
		}

		//��ȡ���ű���Ϣ
		std::string getSymbolTableInfo() const;

		//��ȡ�ڲ��ṹtable
		std::vector<Token>& getTable() { return table; }

		//����main����λ��
		void setMainToken(std::string format) {
			DEBUG_SYMBOL_TABLE("SymbolTable::setMainToken(), mainIndex = " + std::to_string(int(current)), format);
			mainIndex = current;
		}
		Token& getMainToken(std::string format) { 
			DEBUG_SYMBOL_TABLE("SymbolTable::getMainToken() " + std::to_string(int(mainIndex)), format);
			return table[mainIndex];
		}
	};
}

#endif
