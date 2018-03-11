
#include "stdafx.h"
#include "VirtualMachine.h"

//���캯��
kkli::VirtualMachine::VirtualMachine() {

	//����
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::VirtualMachine()");
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

	//��ʼ���Ĵ���
	sp = stack + SEGMENT_SIZE;
	bp = sp;
	pc = text;
	ax = 0;
}

//�������
void kkli::VirtualMachine::addData(int elem) {

	//����
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::addData: ", elem);
	}

	*nextData = elem;
	++nextData;
}

//���ָ��
void kkli::VirtualMachine::addInst(int elem) {

	//����
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::addText: ", elem);
	}

	*nextText = elem;
	++nextText;
}

//ɾ������ָ��
void kkli::VirtualMachine::deleteTopInst() {
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::deleteTopInst: ", *(nextText - 1));
	}

	--nextText;
	if (nextText < text) throw new Error("VirtualMachine::deleteTopInst: no instruction to delete!");
}

//��ջ
int kkli::VirtualMachine::pop() {

	//����
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::pop: ", *(sp));
	}

	if (sp == stack) throw new Error("Error: empty stack!");
	return *(sp++);
}

//��ջ
void kkli::VirtualMachine::push(int elem) {

	//����
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::push: ", elem);
	}

	if (sp == stack + SEGMENT_SIZE) throw new Error("Error: stack overflow!");
	*sp = elem;
	*(--sp) = elem;
}

//��ȡ�ڴ���Ϣ
std::string kkli::VirtualMachine::getInfo() const {

	//����
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::getInfo()");
	}

	//ƴװ�������Ϣ
	std::string str;
	str += "SEGMENT_SIZE: " + std::to_string(SEGMENT_SIZE) + "\n";
	str += "data count: " + std::to_string(nextData - data) + "\n";
	str += "text count: " + std::to_string(nextText - text) + "\n";
	str += "stack count: " + std::to_string(sp - stack) + "\n";
	str += "sp: " + ((sp == stack) ? "emtpy stack" : std::to_string(*(sp - 1))) + "\n";
	str += "bp: " + std::to_string(*bp) + "\n";
	str += "pc: " + std::to_string(*pc) + "\n";
	str += "ax: " + std::to_string(ax) + "\n";
	return std::move(str);
}

//ִ��ָ��
int kkli::VirtualMachine::run() {

	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::run() called!");
	}

	int inst;        //ָ��
	int cycle = 0;   //ִ������

	//ѭ��ִ��ָ�ֱ�������˳�ָ���δָ֪��
	while (1) {
		++cycle;

		//ȡָ��
		inst = *pc;
		++pc;

		//���ִ�е�ָ��
		if (OUTPUT_VM_ACTIONS) {
			std::string debugInfo = "execute: ";
			debugInfo += INSTRUCTION_STRING[inst];
			if (inst <= I_ADJ) debugInfo += "  " + std::to_string(*pc);
			Debug::output(debugInfo);
		}

		//ȡ������
		if (inst == I_IMM) {
			ax = *pc;
			++pc;
		}

		//����charֵ
		else if (inst == I_LC) {
			ax = *(reinterpret_cast<char*>(ax));
		}

		//����intֵ
		else if (inst == I_LI) {
			ax = *(reinterpret_cast<char*>(ax));
		}

		//�洢charֵ
		else if (inst == I_SC) {
			char* addr = reinterpret_cast<char*>(*sp);
			++sp;
			*addr = ax;
		}

		//�洢intֵ
		else if (inst == I_SI) {
			int* addr = reinterpret_cast<int*>(*sp);
			++sp;
			*addr = ax;
		}

		//��ջ
		else if (inst == I_PUSH) {
			*(--sp) = ax;
		}

		//��������ת
		else if (inst == I_JMP) {
			pc = reinterpret_cast<int*>(*pc);
		}

		//����ת
		else if (inst == I_JZ) {
			if (ax != 0) pc = pc + 1;
			else pc = reinterpret_cast<int*>(*pc);
		}

		//������ת
		else if (inst == I_JNZ) {
			if (ax == 0) pc = pc + 1;
			else pc = reinterpret_cast<int*>(*pc);
		}

		//��������
		else if (inst == I_CALL) {
			*(--sp) = reinterpret_cast<int>(pc + 1);
			pc = reinterpret_cast<int*>(*pc);
		}

		//���뺯��֮ǰ
		else if (inst == I_ENT) {
			*(--sp) = reinterpret_cast<int>(bp);
			bp = sp;
			sp = sp - *pc;
			++pc;
		}

		//��������ʱ�Ĳ�����ջ
		else if (inst == I_ADJ) {
			sp = sp + *pc;
			++pc;
		}

		//��������
		else if (inst == I_LEV) {
			sp = bp;  //�ָ�sp
			bp = reinterpret_cast<int*>(*sp);  //�ָ�bp
			++sp;
			pc = reinterpret_cast<int*>(*sp);  //�ָ�pc
			++sp;
		}

		//��ȡ����bp�Ĵ�����λ��
		else if (inst == I_LEA) {
			ax = reinterpret_cast<int>(bp + *pc);
			++pc;
		}

		/*
		���ж�Ԫ���㣬��һ����������*sp�У��ڶ�����������ax��
		*/

		//������
		else if (inst == I_OR) {
			ax = (*sp | ax);
			++sp;
		}

		//�������
		else if (inst == I_XOR) {
			ax = (*sp ^ ax);
			++sp;
		}

		//������
		else if (inst == I_AND) {
			ax = (*sp & ax);
			++sp;
		}

		//��ȱȽ�
		else if (inst == I_EQ) {
			ax = (*sp == ax);
			++sp;
		}

		//���ȱȽ�
		else if (inst == I_NE) {
			ax = (*sp != ax);
			++sp;
		}

		//С�ڱȽ�
		else if (inst == I_LT) {
			ax = (*sp < ax);
			++sp;
		}
		
		//С�ڵ��ڱȽ�
		else if (inst == I_LE) {
			ax = (*sp <= ax);
			++sp;
		}

		//���ڱȽ�
		else if (inst == I_GT) {
			ax = (*sp > ax);
			++sp;
		}

		//���ڵ��ڱȽ�
		else if (inst == I_GE) {
			ax = (*sp >= ax);
			++sp;
		}

		//��������
		else if (inst == I_SHL) {
			ax = (*sp << ax);
			++sp;
		}

		//��������
		else if (inst == I_SHR) {
			ax = (*sp >> ax);
			++sp;
		}

		//�������
		else if (inst == I_ADD) {
			ax = (*sp + ax);
			++sp;
		}

		//�������
		else if (inst == I_SUB) {
			ax = (*sp - ax);
			++sp;
		}

		//�������
		else if (inst == I_MUL) {
			ax = (*sp * ax);
			++sp;
		}

		//�������
		else if (inst == I_DIV) {
			ax = (*sp / ax);
			++sp;
		}

		//��ģ����
		else if (inst == I_MOD) {
			ax = (*sp % ax);
			++sp;
		}

		//���
		else if (inst == I_PRTF) {
			int* temp = sp + pc[1];
			ax = printf(reinterpret_cast<char*>(temp[-1]),
				temp[-2], temp[-3], temp[-4], temp[-5], temp[-6]);
		}

		//��̬�ڴ����
		else if (inst == I_MALC) {
			ax = reinterpret_cast<int>(malloc(*sp));
		}

		//�˳�����
		else if (inst == I_EXIT) {
			printf("exit(%d)", *sp);
			return *sp;
		}
		
		//�����ָ��
		else {
			printf("unknown instruction: %d\n", inst);
			return -1;
		}
	}
}
