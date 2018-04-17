
#include "stdafx.h"
#include "Compiler.h"
#include "Test.h"

using namespace std;

int main()
{
	//每次都清空Debug信息
	kkli::Debug::clear();
	
	bool isTestMode = false;
	string sourceFile;
	while (true) {
		cin >> sourceFile;
		if (sourceFile == "TEST") {
			isTestMode = true;
			break;
		}
		else if (cin.bad()) {
			cin.clear();
			cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
			cout << "can't open: " << sourceFile << ", please input again." << endl;
		}
		else break;
	}

	try {
		if (isTestMode) {
			kkli::Test::getInstance()->run();
		}
		else {
			kkli::Compiler compiler(sourceFile);
			compiler.run();
		}
	}
	catch (const kkli::Error& err) {
		cout << err.what() << endl;
	}

	system("pause");
	getchar();
	if (DEBUG_MODE) {
		getchar();
		getchar();
		getchar();
	}
    return 0;
}
