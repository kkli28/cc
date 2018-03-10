// compiler.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Scanner.h"

using namespace std;

int main()
{
	kkli::Scanner* scanner = kkli::Scanner::getInstance("input/test.txt");
	char c;
	while ((c=scanner->scan()) != EOF) {
		cout << c << "|";
	}
	cout << endl;
	system("pause");
    return 0;
}
