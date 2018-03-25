
#include "stdafx.h"
#include "VirtualMachine.h"
#include "SymbolTable.h"
#include "Lexer.h"

int main()
{
	//每次都清空Debug信息
	kkli::Debug::clear();

	kkli::VirtualMachine vm;
	///*
	vm.addInst(I_ENT);
	vm.addInst(3);
	vm.addInst(I_LEA);
	vm.addInst(-1);
	vm.addInst(I_PUSH);
	vm.addInst(I_IMM);
	vm.addInst(1);
	vm.addInst(I_SI);
	vm.addInst(I_LEA);
	vm.addInst(-2);
	vm.addInst(I_PUSH);
	vm.addInst(I_IMM);
	vm.addInst(2);
	vm.addInst(I_SI);
	vm.addInst(I_LEA);
	vm.addInst(-3);
	vm.addInst(I_PUSH);
	vm.addInst(I_IMM);
	vm.addInst(3);
	vm.addInst(I_SI);
	vm.addInst(I_LEA);
	vm.addInst(-1);
	vm.addInst(I_PUSH);
	vm.addInst(I_LEA);
	vm.addInst(-1);
	vm.addInst(I_LI);
	vm.addInst(I_PUSH);
	vm.addInst(I_LEA);
	vm.addInst(-2);
	vm.addInst(I_LI);
	vm.addInst(I_MUL);
	vm.addInst(I_SI);
	vm.addInst(I_LEA);
	vm.addInst(-2);
	vm.addInst(I_PUSH);
	vm.addInst(I_LEA);
	vm.addInst(-2);
	vm.addInst(I_LI);
	vm.addInst(I_PUSH);
	vm.addInst(I_LEA);
	vm.addInst(-3);
	vm.addInst(I_LI);
	vm.addInst(I_MUL);
	vm.addInst(I_SI);
	vm.addInst(I_LEA);
	vm.addInst(-3);
	vm.addInst(I_PUSH);
	vm.addInst(I_LEA);
	vm.addInst(-3);
	vm.addInst(I_LI);
	vm.addInst(I_PUSH);
	vm.addInst(I_LEA);
	vm.addInst(-1);
	vm.addInst(I_LI);
	vm.addInst(I_SUB);
	vm.addInst(I_SI);
	vm.addInst(I_LEA);
	vm.addInst(-1);
	vm.addInst(I_LI);
	vm.addInst(I_PUSH);
	vm.addInst(I_LEA);
	vm.addInst(-2);
	vm.addInst(I_LI);
	vm.addInst(I_ADD);
	vm.addInst(I_PUSH);
	vm.addInst(I_LEA);
	vm.addInst(-3);
	vm.addInst(I_LI);
	vm.addInst(I_ADD);

	//因为没有初始化pc指向main函数起点，因此不能LEV
	//vm.addInst(I_LEV);
	vm.addInst(I_PUSH);
	vm.addInst(I_EXIT);
	vm.run();
	//*/

	/*
	kkli::Lexer lexer("input/test.txt");
	std::cout << lexer.getSouce() << std::endl;
	*/
	/*
	try {
		auto p = lexer.next();
		while (p.first != kkli::TokenType::END) {
			std::cout << "[" << kkli::Token::getTokenTypeName(p.first) << "]  [" << p.second << "]" << std::endl;
			p = lexer.next();
		}
	}
	catch (const kkli::Error& e) {
		std::cout << e.what() << std::endl;
	}
	//kkli::SymbolTable* table = kkli::SymbolTable::getInstance();
	//std::cout << table->getSymbolTableInfo() << std::endl;
	*/
	system("pause");
    return 0;
}
