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
		size_t mainIndex;  //main����Token������

		int scopeIndex;          //��ǰ�������ǣ���5
		std::vector<int> scope;  //��ǰ������·������0/1/4/5

	private:
		//DEBUG
		void DEBUG_SYMBOL_TABLE(std::string arg1, std::string arg2) {
			if (DEBUG_INFO->OUTPUT_SYMBOL_TABLE_ACTIONS) {
				Debug::output(arg1, arg2); 
			}
		}

	public:
		SymbolTable() :current(-1), mainIndex(-1), scopeIndex(0) {
			scope.push_back(scopeIndex);  //ȫ��������Ϊ 0
		}

		static std::string getScopeStr(const std::vector<int> scope);

		//�ж��Ƿ���ڷ���
		bool has(bool isDef, int hash, std::string name, std::string format);

		//������
		void enterScope(std::string format);
		void leaveScope(std::string format);
		std::vector<int> getCurrScope() const { return scope; }

		//��ȡ��ǰ���ţ�ע�⣡��֮������push_back����token�����ý����ܲ�����Ч��
		Token& getCurrentToken(std::string format) {
			DEBUG_SYMBOL_TABLE("SymbolTable::getCurrentToken(): [klass]: " + Token::getTokenKlassName(table[current].klass), FORMAT(format));
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

		//����main����Token����λ��
		void setMainToken(std::string format) {
			DEBUG_SYMBOL_TABLE("SymbolTable::setMainToken(), mainIndex = " + std::to_string(current), format);
			mainIndex = current;
		}
		Token& getMainToken(std::string format) {
			DEBUG_SYMBOL_TABLE("SymbolTable::getMainToken(): mainIndex = " + std::to_string(mainIndex), format);
			return table[mainIndex];
		}
	};
}

#endif
