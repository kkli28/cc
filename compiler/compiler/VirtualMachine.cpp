#include "stdafx.h"
#include "VirtualMachine.h"
#include "Token.h"

//���캯��
kkli::VirtualMachine::VirtualMachine() {

	//��ʼ��������
	data = new char[SEGMENT_SIZE];
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

	needDataAlignment = false;
}

//���char������
void kkli::VirtualMachine::addDataChar(char elem, std::string format) {
	DEBUG_VM("VirtualMachine::addCharData(" + std::to_string(elem) + ")", format);

	if (nextData > data + SEGMENT_SIZE) throw Error("VirtualMachine::addCharData(): data overflow");

	*(reinterpret_cast<int*>(nextData)) = elem;
	++nextData;

	//char��������Ӻ󣬲��ڶ���λ�ã���˺����ж�������ʱ��������ж���
	needDataAlignment = true;
}

//���int������
void kkli::VirtualMachine::addDataInt(int elem, std::string format) {
	DEBUG_VM("VirtualMachine::addIntData(" + std::to_string(elem) + ")", format);
	
	if (needDataAlignment) {
		dataAlignment(FORMAT(format));
	}
	
	if (nextData > data + SEGMENT_SIZE) {
		throw Error("VirtualMachine::addDataInt(): data overflow.");
	}
	*(reinterpret_cast<int*>(nextData)) = elem;
	nextData += 4;
}

//���char������
void kkli::VirtualMachine::addDataCharArray(int count, const std::vector<char>& values, std::string format) {
	DEBUG_VM("VirtualMachine::addDataArray(" + std::to_string(count) + ")", format);

	if (count <= 0) {
		throw Error("VirtualMachine::addDataCharAray(" + std::to_string(count) + ": invalid count.");
	}

	if (values.size() > count) {
		throw Error("VirtualMachine::addDataCharArray(): too many values for array.");
	}

	if (nextData + count > data + SEGMENT_SIZE) {
		throw Error("VirtualMachine::addDataCharArray(" + std::to_string(count) + "): data overflow.");
	}

	char* dataPtr = nextData;
	for (int i = 0; i < values.size(); ++i) {
		*dataPtr = values[i];
		++dataPtr;
	}
	nextData += count;
	needDataAlignment = true;
}

//���int������
void kkli::VirtualMachine::addDataIntArray(int count, const std::vector<int>& values, std::string format) {
	DEBUG_VM("VirtualMachine::addDataIntArray(" + std::to_string(count) + ")", format);

	if (count <= 0) {
		throw Error("VirtualMachine::addDataIntArray(" + std::to_string(count) + ")");
	}

	if (values.size() > count) {
		throw Error("VirtualMachine::adDataIntArray(): too many values for array.");
	}

	if (needDataAlignment) {
		dataAlignment(FORMAT(format));
	}

	if (nextData + count * 4 > data + SEGMENT_SIZE) {
		throw Error("VirtualMachine::addDataIntArray(" + std::to_string(count) + "): data overflow.");
	}

	int* dataPtr = reinterpret_cast<int*>(nextData);
	for (int i = 0; i < values.size(); ++i) {
		*dataPtr = values[i];
		++dataPtr;
	}
	nextData += count * 4;
}

//���ָ��
void kkli::VirtualMachine::addInst(int elem, std::string format) {
	DEBUG_VM("VirtualMachine::addInst(" + getInstructionName(elem) + ")", format);

	if (nextText >= text + SEGMENT_SIZE) throw Error("VirtualMachine::addInst("+std::to_string(elem)+"): text overflow");
	*nextText = elem;
	++nextText;
}

//���ָ��Ĳ�����
void kkli::VirtualMachine::addInstData(int elem, std::string format) {
	DEBUG_VM("VirtualMachine::addInstData(" + std::to_string(elem) + ")", format);

	if (nextText >= text + SEGMENT_SIZE) throw Error("VirtualMachine::addInstData(" + std::to_string(elem) + "): text overflow.");
	*nextText = elem;
	++nextText;
}

//��ȡ����ָ��
int kkli::VirtualMachine::getTopInst(std::string format) {
	DEBUG_VM("VirtualMachine::getTopInst(): " + getInstructionName(*(nextText - 1)), format);
	if (nextText <= text) throw Error("VirtualMachine::getTopInst(): no instruction to get!");
	return *(nextText - 1);
}

//ɾ������ָ��
void kkli::VirtualMachine::deleteTopInst(std::string format) {
	DEBUG_VM("VirtualMachine::deleteTopInst(): " + getInstructionName(*(nextText - 1)), format);

	--nextText;
	if (nextText < text) throw Error("VirtualMachine::deleteTopInst(): no instruction to delete!");
}

//���ݶ���
void kkli::VirtualMachine::dataAlignment(std::string format) {
	DEBUG_VM("[before data_align] nextData = "
		+ std::to_string(reinterpret_cast<int>(nextData)) + ".", FORMAT(format));
	
	nextData = reinterpret_cast<char*>((reinterpret_cast<int>(nextData) + 4) & (-4));

	DEBUG_VM("[after data_align] nextData = "
		+ std::to_string(reinterpret_cast<int>(nextData)) + ".", FORMAT(format));

	if (nextData > data + SEGMENT_SIZE) throw Error("VirtualMachine::addIntData(): data overflow");
	needDataAlignment = false;
}

//��ȡ��һ��д�����ݵ�λ��
char* kkli::VirtualMachine::getNextDataPos(int dataType, std::string format) {
	DEBUG_VM("VirtualMachine::getNextDataPos(" + Token::getDataTypeName(dataType) + ")", FORMAT(format));
	if (dataType == CHAR_TYPE) {
		return nextData;
	}
	else {
		if (needDataAlignment) dataAlignment(FORMAT(format));
		return nextData;
	}
}

//��ȡ��Ϣ
std::string kkli::VirtualMachine::getInfo() const {
	std::string str;
	str += "\n[VM info]:";
	str += "[SEGMENT_SIZE]: " + std::to_string(SEGMENT_SIZE) + "\n";
	str += "[data]: " + std::to_string(reinterpret_cast<int>(data)) + "\n";
	str += "[text]: " + std::to_string(reinterpret_cast<int>(text)) + "\n";
	str += "[stack]: " + std::to_string(reinterpret_cast<int>(stack)) + "\n";
	str += "[data count]: " + std::to_string(nextData - data) + "\n";
	str += "[text count]: " + std::to_string(nextText - text) + "\n";
	str += "[stack count]: " + std::to_string(sp - stack) + "\n";
	str += "[sp]: " + ((sp == stack) ? "emtpy stack" : std::to_string(*sp)) + "\n";
	str += "[bp]: " + std::to_string(*bp) + "\n";
	str += "[pc]: " + std::to_string(*pc) + "\n";
	str += "[ax]: " + std::to_string(ax) + "\n";
	return std::move(str);
}

//ִ��ָ��
int kkli::VirtualMachine::run() {
	DEBUG_VM("VirtualMachine::run()", "");

	std::string format = "";

	int inst;        //ָ��
	int cycle = 0;   //ִ������

	//ѭ��ִ��ָ�ֱ�������˳�ָ���δָ֪��
	while (1) {
		++cycle;
		
		//ȡָ��
		inst = *pc;
		++pc;

		//���ִ�е�ָ��
		std::string instInfo = getInstructionName(inst);
		if (inst <= I_ADJ) instInfo += "  " + std::to_string(*pc);
		DEBUG_VM(instInfo, format);

		//TEST
		//std::cout << instInfo << std::endl;
		
		//ȡ������
		if (inst == I_IMM) {
			ax = *pc;
			++pc;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//����charֵ
		else if (inst == I_LC) {
			ax = *(reinterpret_cast<char*>(ax));
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//����intֵ
		else if (inst == I_LI) {
			ax = *(reinterpret_cast<int*>(ax));
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//�洢charֵ
		else if (inst == I_SC) {
			char* addr = reinterpret_cast<char*>(*sp);
			++sp;
			*addr = ax;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//�洢intֵ
		else if (inst == I_SI) {
			int* addr = reinterpret_cast<int*>(*sp);
			++sp;
			*addr = ax;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//��ջ
		else if (inst == I_PUSH) {
			*(--sp) = ax;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
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

		//���뺯����
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
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//�������
		else if (inst == I_XOR) {
			ax = (*sp ^ ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//������
		else if (inst == I_AND) {
			ax = (*sp & ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//��ȱȽ�
		else if (inst == I_EQ) {
			ax = (*sp == ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//���ȱȽ�
		else if (inst == I_NE) {
			ax = (*sp != ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//С�ڱȽ�
		else if (inst == I_LT) {
			ax = (*sp < ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//С�ڵ��ڱȽ�
		else if (inst == I_LE) {
			ax = (*sp <= ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//���ڱȽ�
		else if (inst == I_GT) {
			ax = (*sp > ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//���ڵ��ڱȽ�
		else if (inst == I_GE) {
			ax = (*sp >= ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//��������
		else if (inst == I_SHL) {
			ax = (*sp << ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//��������
		else if (inst == I_SHR) {
			ax = (*sp >> ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//�������
		else if (inst == I_ADD) {
			ax = (*sp + ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//�������
		else if (inst == I_SUB) {
			ax = (*sp - ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//�������
		else if (inst == I_MUL) {
			ax = (*sp * ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//�������
		else if (inst == I_DIV) {
			ax = (*sp / ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//��ģ����
		else if (inst == I_MOD) {
			ax = (*sp % ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//���
		else if (inst == I_PRTF) {
			int* temp = sp + pc[1];
			ax = printf(reinterpret_cast<char*>(temp[-1]),
				temp[-2], temp[-3], temp[-4], temp[-5], temp[-6]);
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//��̬�ڴ����
		else if (inst == I_MALC) {
			ax = reinterpret_cast<int>(malloc(*sp));
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//�˳�����
		else if (inst == I_EXIT) {
			printf("exit(%d)\n", ax);
			return ax;
		}

		//�����ָ��
		else {
			throw Error("VirtualMachine::run(): unknown instruction!");
		}
	}
}
