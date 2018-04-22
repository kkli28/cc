
#include "stdafx.h"
#include "Compiler.h"
#include "Test.h"

using namespace std;
bool kkli::Debug::OUTPUT_TO_CONSOLE = true;

int main()
{
	//每次都清空Debug信息
	kkli::Debug::clear();
	DEBUG_INFO->reset();
	
	string str;
	while (true) {
		cin >> str;

		//测试模式
		if (str == "TEST") {
			DEBUG_INFO->setTestMode();
			break;
		}
		//查看代码执行过程
		else if (str == "-e") { //TODO: ERROR
			DEBUG_INFO->seeCodeExecute();
		}
		//查看代码执行过程中的值 （ax=...）
		else if (str == "-ev") {
			DEBUG_INFO->seeCodeExecuteDetail();
		}
		//无视warning模式（不输出warning）
		else if (str == "-nw") {
			DEBUG_INFO->ignoreWarning();
		}
		//一般详细模式
		else if (str == "-dt1") {
			DEBUG_INFO->setDetail1Mode();
		}
		//十分详细模式
		else if (str == "-dt2") {
			DEBUG_INFO->setDetail2Mode();
		}
		//最详细模式
		else if (str == "-dt3") {
			DEBUG_INFO->setDetail3Mode();
		}
		//debug信息输出到debug.txt文件
		else if (str == "-df") {
			kkli::Debug::OUTPUT_TO_CONSOLE = false;
		}
		else if (cin.bad()) {
			cin.clear();
			cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
			cout << "can't open: " << str << ", please input again." << endl;
		}
		else break;
	}

	try {
		//测试模式
		if (DEBUG_INFO->IS_TEST_MODE) {
			kkli::Test::getInstance()->run();
		}
		//编译模式
		else {
			kkli::Compiler compiler(str);
			compiler.run();
		}
	}
	catch (const kkli::Error& err) {
		cout << err.what() << endl;
	}

	system("pause");
	getchar();
	getchar();
	getchar();
	getchar();
    return 0;
}
