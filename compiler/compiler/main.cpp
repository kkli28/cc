
#include "stdafx.h"
#include "Generator.h"

using namespace std;

int main()
{
	//每次都清空Debug信息
	kkli::Debug::clear();

	string sourceFile;
	while (true) {
		cin >> sourceFile;
		if (cin.bad()) {
			cin.clear();
			cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
			cout << "can't open: " << sourceFile << ", please input again." << endl;
		}
		else break;
	}

	//向符号表中插入预定内容
	
	try {
		kkli::Generator gen(sourceFile);
		gen.gen();

		kkli::VirtualMachine* vm = kkli::VirtualMachine::getInstance();
		vm->pc = kkli::SymbolTable::getInstance()->getMainAddr();
		vm->addInst(I_EXIT);
		//vm->addInst(I_PUSH);
		//int* tempSP = vm->sp;
		vm->run();
	}
	catch (const kkli::Error& err) {
		cout << err.what() << endl;
	}

	system("pause");
    return 0;
}
