#include "Memory.h"

//���캯��
kkli::Memory::Memory() {

	//����
	if (OUTPUT_MEMORY_ACTIONS) {
		Debug::output("Memory::Memory()");
	}

	//��ʼ��������
	data = new int[SEGMENT_SIZE];
	text = new int[SEGMENT_SIZE];
	stack = new int[SEGMENT_SIZE];

	//��ʼ������
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

//�������
void kkli::Memory::addData(int elem) {

	//����
	if (OUTPUT_MEMORY_ACTIONS) {
		Debug::output("Memory::addData: ", elem);
	}

	*nextData = elem;
	++nextData;
}

//��Ӵ���
void kkli::Memory::addText(int elem) {

	//����
	if (OUTPUT_MEMORY_ACTIONS) {
		Debug::output("Memory::addText: ", elem);
	}

	*nextText = elem;
	++nextText;
}

//��ջ
int kkli::Memory::pop() {

	//����
	if (OUTPUT_MEMORY_ACTIONS) {
		Debug::output("Memory::pop: ", *(sp - 1));
	}

	if (sp == stack) throw new Error("Error: empty stack!");
	return *(--sp);
}

//��ջ
void kkli::Memory::push(int elem) {

	//����
	if (OUTPUT_MEMORY_ACTIONS) {
		Debug::output("Memory::push: ", elem);
	}

	if (sp == stack + SEGMENT_SIZE) throw new Error("Error: stack overflow!");
	*sp = elem;
	++sp;
}

//��ȡ�ڴ���Ϣ
std::string kkli::Memory::getInfo() const {

	//����
	if (OUTPUT_MEMORY_ACTIONS) {
		Debug::output("Memory::getInfo()");
	}

	//ƴװ�ڴ���Ϣ
	std::string str;
	str += "SEGMENT_SIZE: " + std::to_string(SEGMENT_SIZE) + "\n";
	str += "data: " + std::to_string(reinterpret_cast<int>(data)) + "\n";
	str 
}