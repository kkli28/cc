#include "Memory.h"

//构造函数
kkli::Memory::Memory() {

	//调试
	if (OUTPUT_MEMORY_ACTIONS) {
		Debug::output("Memory::Memory()");
	}

	//初始化三个段
	data = new int[SEGMENT_SIZE];
	text = new int[SEGMENT_SIZE];
	stack = new int[SEGMENT_SIZE];

	//初始化数据
	for (int i = 0; i < SEGMENT_SIZE; ++i) {
		data[i] = 0;
		text[i] = 0;
		stack[i] = 0;
	}

	nextData = data;
	nextText = text;
	sp = stack;
	bp = sp;
	pc = text;
	ax = 0;
}

//添加数据
void kkli::Memory::addData(int elem) {

	//调试
	if (OUTPUT_MEMORY_ACTIONS) {
		Debug::output("Memory::addData: ", elem);
	}

	*nextData = elem;
	++nextData;
}

//添加代码
void kkli::Memory::addText(int elem) {

	//调试
	if (OUTPUT_MEMORY_ACTIONS) {
		Debug::output("Memory::addText: ", elem);
	}

	*nextText = elem;
	++nextText;
}

//出栈
int kkli::Memory::pop() {

	//调试
	if (OUTPUT_MEMORY_ACTIONS) {
		Debug::output("Memory::pop: ", *(sp - 1));
	}

	if (sp == stack) throw new Error("Error: empty stack!");
	return *(--sp);
}

//入栈
void kkli::Memory::push(int elem) {

	//调试
	if (OUTPUT_MEMORY_ACTIONS) {
		Debug::output("Memory::push: ", elem);
	}

	if (sp == stack + SEGMENT_SIZE) throw new Error("Error: stack overflow!");
	*sp = elem;
	++sp;
}

//获取内存信息
std::string kkli::Memory::getInfo() const {

	//调试
	if (OUTPUT_MEMORY_ACTIONS) {
		Debug::output("Memory::getInfo()");
	}

	//拼装内存信息
	std::string str;
	str += "SEGMENT_SIZE: " + std::to_string(SEGMENT_SIZE) + "\n";
	str += "data: " + std::to_string(reinterpret_cast<int>(data)) + "\n";
	str 
}