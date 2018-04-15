#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

/*
文件描述：符号表，记录编译过程中的符号信息
*/

#include "Token.h"

namespace kkli {

	//========================================
	// 符号表
	//========================================

	/*
	table为已添加完成的符号
	currToken为正在完善的符号，其信息的填写跨越词法分析和语法分析，无法在
	一趟内就填写完成，因此需要单独存储，当信息填写完整后，再放入符号表
	*/
	class SymbolTable {
	private:
		std::vector<Token> table;
		size_t current;
		size_t mainIndex;  //main函数Token的索引

	public:

		//单例
		static SymbolTable* getInstance() {
			static SymbolTable* table = nullptr;
			if (table == nullptr) {
				table = new SymbolTable();
			}
			return table;
		}

		//重置所有信息
		void reset(std::string format);

		//判断是否存在符号
		bool has(int hash, std::string name, std::string format);

		//获取当前符号（注意！若之后再有push_back，则token的引用将可能不再生效）
		Token& getCurrentToken(std::string format) { 
			return table[current]; 
		}
		size_t getCurrent(std::string format) {  //只获取current应该是更好的选项
			return current;
		}

		//获取Token
		Token& getToken(size_t index) {
			if (index < 0 || index >= table.size()) {
				throw Error("SymbolTable::getToken(" + std::to_string(index) + "), invalid index, table size is " + std::to_string(table.size()) + ".");
			}
			return table[index];
		}

		//获取符号表信息
		std::string getSymbolTableInfo() const;

		//获取内部结构table
		std::vector<Token>& getTable() { return table; }

		//设置main函数位置
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
