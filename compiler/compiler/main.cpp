// compiler.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "VirtualMachine.h"
#include "Lexer.h"

using namespace std;
using namespace kkli;

int main()
{
	kkli::VirtualMachine vm;
	/*
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
	*/

	Lexer lexer("input/test.txt");

	system("pause");
    return 0;
}
