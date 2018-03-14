// compiler.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "VirtualMachine.h"

using namespace std;
using namespace kkli;

int main()
{
	kkli::VirtualMachine vm;
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
	vm.addInst(I_LEV);
	vm.addInst(I_PUSH);
	vm.addInst(I_EXIT);
	vm.run();

	system("pause");
    return 0;
}
