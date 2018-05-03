
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
	auto showHelp = [] {
		std::cout << "\n-test: test all test-files." << std::endl;
		std::cout << "-e: show instruction execute." << std::endl;
		std::cout << "-ev: show instruction execute with register's value." << std::endl;
		std::cout << "-nw: don't show warnings." << std::endl;
		std::cout << "-dt1: show compile details level-1(little)." << std::endl;
		std::cout << "-dt2: show compile details level-2(normal)." << std::endl;
		std::cout << "-dt3: show compile details level-3(everything?)." << std::endl;
		std::cout << "-df: write debug info to debug.txt." << std::endl;
	};
	while (true) {
		std::cout << ">> ";
		string str;
		bool isHelpMode = false;
		while (true) {
			cin >> str;

			//查看支持的指令
			if (str == "-help") {
				showHelp();
				isHelpMode = true;
				break;
			}
			//测试模式
			if (str == "-test") {
				DEBUG_INFO->setTestMode();
				break;
			}
			//查看生成的代码
			else if (str == "-gen") {
				DEBUG_INFO->setGlobalDeclInstGenMode();
			}
			//查看每个全局定义生成的代码
			else if (str == "-gg") {
				DEBUG_INFO->setInstGenMode();
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
			//乱来
			else if (cin.bad()) {
				cin.clear();
				cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
				cout << "can't open: " << str << ", please input again." << endl;
			}
			//输入的是文件名
			else break;
		}

		try {
			//查看帮助模式
			if (isHelpMode) {}
			//测试模式
			else if (DEBUG_INFO->IS_TEST_MODE) {
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

		std::cout << "\n\n";
	}

	system("pause");
    return 0;
}
