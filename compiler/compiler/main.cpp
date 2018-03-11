// compiler.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "VirtualMachine.h"

using namespace std;
using namespace kkli;

int main()
{
	kkli::VirtualMachine vm;
	vm.addInst(I_IMM);
	vm.addInst(10);
	vm.addInst(I_PUSH);
	vm.addInst(I_IMM);
	vm.addInst(20);
	vm.addInst(I_ADD);
	vm.addInst(I_PUSH);
	vm.addInst(I_EXIT);
	vm.run();

	system("pause");
    return 0;
}
