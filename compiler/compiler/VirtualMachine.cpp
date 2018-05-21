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
	DEBUG_VM("VirtualMachine::addDataChar(" + std::to_string(elem) + ")", format);

	if (nextData > data + SEGMENT_SIZE) throw Error("VirtualMachine::addCharData(): data overflow");

	*nextData = elem;
	++nextData;

	//char��������Ӻ󣬲��ڶ���λ�ã���˺����ж�������ʱ��������ж���
	needDataAlignment = true;
}

//���int������
void kkli::VirtualMachine::addDataInt(int elem, std::string format) {
	DEBUG_VM("VirtualMachine::addDataInt(" + std::to_string(elem) + ")", format);
	
	if (needDataAlignment) {
		dataAlignment(FORMAT(format));
	}
	
	if (nextData > data + SEGMENT_SIZE) {
		throw Error("VirtualMachine::addDataInt(): data overflow.");
	}
	*(reinterpret_cast<int*>(nextData)) = elem;
	nextData += 4;
}

void kkli::VirtualMachine::addDataDefaultChars(int count, std::string format) {
	DEBUG_VM("VirtualMachine::addDataChars(" + std::to_string(count) + ")", format);

	nextData += count;
	if (nextData > data + SEGMENT_SIZE) throw Error("VirtualMachine::addCharData(): data overflow");
	needDataAlignment = true;
}

void kkli::VirtualMachine::addDataDefaultInts(int count, std::string format) {
	DEBUG_VM("VirtualMachine::addDataInts(" + std::to_string(count) + ")", format);

	if (needDataAlignment) {
		dataAlignment(FORMAT(format));
	}
	nextData += count * 4;
	if (nextData > data + SEGMENT_SIZE) {
		throw Error("VirtualMachine::addDataInt(): data overflow.");
	}
}

void kkli::VirtualMachine::setChars(char* addr, std::vector<char>&& chars, std::string format) {
	DEBUG_VM("VirtualMachine::addChars().", format);
	int size = chars.size();
	if (addr < data) {
		throw Error("VirtualMachine::setChars(): addr " + std::to_string(reinterpret_cast<int>(addr)) + " is not in data segment.");
	}
	else if (addr + size > data + SEGMENT_SIZE) {
		throw Error("VirtualMachine::setChars(): data overflow.");
	}
	for (auto c : chars) {
		*addr = c;
		++addr;
	}
}

void kkli::VirtualMachine::setInts(char* addr, std::vector<int>&& ints, std::string format) {
	DEBUG_VM("VirtualMachine::addInts().", format);
	int size = ints.size();
	if (addr < data) {
		throw Error("VirtualMachine::setInts(): addr " + std::to_string(reinterpret_cast<int>(addr)) + " is not in data segment.");
	}
	else if (addr + size * 4 > data + SEGMENT_SIZE) {
		throw Error("VirtualMachine::setInts(): data overflow.");
	}
	for (auto i : ints) {
		*reinterpret_cast<int*>(addr) = i;
		addr += 4;
	}
}

//���ָ��
void kkli::VirtualMachine::addInst(int elem, std::string format) {
	DEBUG_VM("VirtualMachine::addInst(" + getInstructionName(elem) + ")", format);

	if (nextText >= text + SEGMENT_SIZE) throw Error("VirtualMachine::addInst("+std::to_string(elem)+"): text overflow");
	*nextText = elem;
	++nextText;

	//�����������Чָ��
	if (elem == I_NAI) {
		throw Error("kkli::VirtualMachine::addInst(): inst is I_NAI.");
	}
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

//����ȫ�ֶ������ɴ������ʼ��
void kkli::VirtualMachine::setGlobalDeclInstTag(bool isStart) {
	if (isStart) {
		instTag.push_back({ reinterpret_cast<int>(nextText), 0 });
	}
	else {
		instTag.back().second = reinterpret_cast<int>(nextText);
	}
}

//��ȡÿ��ȫ�ֶ����������ɵ�ָ��
std::string kkli::VirtualMachine::getGlobalDeclGenInst(bool hasLineNumber) {
	std::string result;

	int* begAddr = reinterpret_cast<int*>(instTag.back().first);
	int* endAddr = reinterpret_cast<int*>(instTag.back().second);

	//ȫ�ֶ����� int i = 0; �������ɴ���
	if (begAddr == endAddr) {
		return "";
	}
	int index = 0;
	while (begAddr < endAddr) {
		int inst = *begAddr;
		++begAddr;
		result += (hasLineNumber ? std::to_string(++index) + "\t" : "") + getInstructionName(inst);
		if (inst <= I_ADJ) {
			result += "  " + std::to_string(*begAddr);
			++begAddr;
		}
		result.push_back('\n');
	}
	return std::move(result);
}

//��ȡ���ɵ�����ָ��
std::string kkli::VirtualMachine::getGenInst(bool hasLineNumber) {
	std::string result;
	int* addr = text;
	int index = 0;
	while (addr < text + SEGMENT_SIZE && *addr != I_NAI) {
		int inst = *addr;
		++addr;
		result += (hasLineNumber ? std::to_string(++index) + "\t" : "") + getInstructionName(inst);
		if (inst <= I_ADJ) {
			result += " " + std::to_string(*addr);
			++addr;
		}
		result.push_back('\n');
	}
	return std::move(result);
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
	std::cout << "-- result --" << std::endl;

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
		if (DEBUG_INFO->OUTPUT_VM_EXECUTE_ACTIONS) {
			Debug::output(instInfo, "");
		}
		
		//ȡ������
		if (inst == I_IMM) {
			ax = *pc;
			++pc;
			DEBUG_REGISTER(format);
		}

		//����charֵ
		else if (inst == I_LC) {
			ax = *(reinterpret_cast<char*>(ax));
			DEBUG_REGISTER(format);
		}

		//����intֵ
		else if (inst == I_LI) {
			ax = *(reinterpret_cast<int*>(ax));
			DEBUG_REGISTER(format);
		}

		//�洢charֵ
		else if (inst == I_SC) {
			char* addr = reinterpret_cast<char*>(*sp);
			++sp;
			*addr = ax;
			DEBUG_REGISTER(format);
		}

		//�洢intֵ
		else if (inst == I_SI) {
			int* addr = reinterpret_cast<int*>(*sp);
			++sp;
			*addr = ax;
			DEBUG_REGISTER(format);
		}

		//��ջ
		else if (inst == I_PUSH) {
			*(--sp) = ax;
			DEBUG_REGISTER(format);
		}

		//��������ת
		else if (inst == I_JMP) {
			pc = reinterpret_cast<int*>(*pc);
			DEBUG_REGISTER(format);
		}

		//����ת
		else if (inst == I_JZ) {
			if (ax != 0) pc = pc + 1;
			else pc = reinterpret_cast<int*>(*pc);
			DEBUG_REGISTER(format);
		}

		//������ת
		else if (inst == I_JNZ) {
			if (ax == 0) pc = pc + 1;
			else pc = reinterpret_cast<int*>(*pc);
			DEBUG_REGISTER(format);
		}

		//��������
		else if (inst == I_CALL) {
			*(--sp) = reinterpret_cast<int>(pc + 1);
			pc = reinterpret_cast<int*>(*pc);
			DEBUG_REGISTER(format);
		}

		//���뺯����
		else if (inst == I_ENT) {
			*(--sp) = reinterpret_cast<int>(bp);
			bp = sp;
			sp = sp - *pc;
			++pc;
			DEBUG_REGISTER(format);
		}
		
		//��������ʱ�Ĳ�����ջ
		else if (inst == I_ADJ) {
			sp = sp + *pc;
			++pc;
			DEBUG_REGISTER(format);
		}

		//��������
		else if (inst == I_LEV) {
			sp = bp;  //�ָ�sp
			bp = reinterpret_cast<int*>(*sp);  //�ָ�bp
			++sp;
			pc = reinterpret_cast<int*>(*sp);  //�ָ�pc
			++sp;
			DEBUG_REGISTER(format);
		}

		//��ȡ����bp�Ĵ�����λ��
		else if (inst == I_LEA) {
			ax = reinterpret_cast<int>(bp + *pc);
			++pc;
			DEBUG_REGISTER(format);
		}

		/*
		���ж�Ԫ���㣬��һ����������*sp�У��ڶ�����������ax��
		*/

		//������
		else if (inst == I_OR) {
			ax = (*sp | ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//�������
		else if (inst == I_XOR) {
			ax = (*sp ^ ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//������
		else if (inst == I_AND) {
			ax = (*sp & ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//��ȱȽ�
		else if (inst == I_EQ) {
			ax = (*sp == ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//���ȱȽ�
		else if (inst == I_NE) {
			ax = (*sp != ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//С�ڱȽ�
		else if (inst == I_LT) {
			ax = (*sp < ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//С�ڵ��ڱȽ�
		else if (inst == I_LE) {
			ax = (*sp <= ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//���ڱȽ�
		else if (inst == I_GT) {
			ax = (*sp > ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//���ڵ��ڱȽ�
		else if (inst == I_GE) {
			ax = (*sp >= ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//��������
		else if (inst == I_SHL) {
			ax = (*sp << ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//��������
		else if (inst == I_SHR) {
			ax = (*sp >> ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//�������
		else if (inst == I_ADD) {
			ax = (*sp + ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//�������
		else if (inst == I_SUB) {
			ax = (*sp - ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//�������
		else if (inst == I_MUL) {
			ax = (*sp * ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//�������
		else if (inst == I_DIV) {
			ax = (*sp / ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//��ģ����
		else if (inst == I_MOD) {
			ax = (*sp % ax);
			++sp;
			DEBUG_REGISTER(format);
		}

		//���
		else if (inst == I_PRTF) {
			/*
			Ϊ����pc[1]����Ϊprintf�����ú��������ɵĴ���ṹΪ��
			I_PRTF
			I_ADJ <param_count>
			��inst == I_PRTFʱ��*pcΪI_ADJ����pc[1]Ϊ<param_count>
			��˿���ͨ������������֪��PRTF�Ĳ�����������tempΪ��һ��������ǰһ��λ��
			��ʱtemp[-1]Ϊ��һ��������temp[-2]Ϊ�ڶ����������Դ�����
			*/
			int* temp = sp + pc[1];
			ax = printf(reinterpret_cast<char*>(temp[-1]),
				temp[-2], temp[-3], temp[-4], temp[-5], temp[-6]);
			DEBUG_REGISTER(format);
		}

		//��̬�ڴ����
		else if (inst == I_MALC) {
			ax = reinterpret_cast<int>(malloc(*sp));
			DEBUG_REGISTER(format);
		}

		//�˳�����
		else if (inst == I_EXIT) {
			printf("exit(%d)\n", ax);
			DEBUG_VM_EXECUTE_DETAIL("ax: " + std::to_string(ax), FORMAT(format));
			DEBUG_VM_EXECUTE_DETAIL("\nexit(" + std::to_string(ax) + ")", FORMAT(format));
			return ax;
		}

		//�������ݣ�ֻ֧��������������һ������Ϊ��ʽ���ַ������ڶ�������Ϊ��Ҫд�����ݵ�λ��
		else if (inst == I_SCANF) {
			int* temp = sp + pc[1];
			ax = scanf(reinterpret_cast<char*>(temp[-1]), temp[-2]);
			DEBUG_REGISTER(format);
		}

		//����������ȡһ��char
		else if (inst == I_GETC) {
			ax = getchar();
			DEBUG_VM_EXECUTE_DETAIL("ax: " + std::to_string(ax), FORMAT(format));
		}

		//��һ��char�ŵ������
		else if (inst == I_PUTC) {
			int* temp = sp + pc[1];
			ax = putchar(temp[-1]);
			DEBUG_VM_EXECUTE_DETAIL("ax: " + std::to_string(ax), FORMAT(format));
		}

		//�����ָ��
		else {
			throw Error("VirtualMachine::run(): unknown instruction!");
		}
	}
}
