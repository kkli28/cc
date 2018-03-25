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
		int current;
		int* mainAddr;    //main函数的地址

	public:

		//单例
		static SymbolTable* getInstance() {
			static SymbolTable* table = nullptr;
			if (table == nullptr) {
				table = new SymbolTable();
			}
			return table;
		}

		//判断是否存在符号
		bool has(int hash, std::string name);

		//获取当前符号
		Token& getCurrentToken() { return table[current]; }

		//获取符号表信息
		std::string getSymbolTableInfo() const;

		//获取内部结构table
		std::vector<Token>& getTable() { return table; }

		//设置main函数地址
		void setMainAddr(int* addr) { mainAddr = addr; }
		int* getMainAddr() { return mainAddr; }
	};
}

#endif
