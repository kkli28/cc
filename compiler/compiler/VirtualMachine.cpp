#include "stdafx.h"
#include "VirtualMachine.h"
#include "Token.h"

//构造函数
kkli::VirtualMachine::VirtualMachine() {

	//初始化三个段
	data = new char[SEGMENT_SIZE];
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

	//初始化寄存器
	sp = stack + SEGMENT_SIZE;
	bp = sp;
	pc = text;
	ax = 0;

	needDataAlignment = false;
}

//添加char型数据
void kkli::VirtualMachine::addDataChar(char elem, std::string format) {
	DEBUG_VM("VirtualMachine::addCharData(" + std::to_string(elem) + ")", format);

	if (nextData > data + SEGMENT_SIZE) throw Error("VirtualMachine::addCharData(): data overflow");

	*(reinterpret_cast<int*>(nextData)) = elem;
	++nextData;

	//char型数据添加后，不在对齐位置，因此后续有对齐需求时，必须进行对齐
	needDataAlignment = true;
}

//添加int型数据
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

//添加char型数组
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

//添加int型数组
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

//添加指令
void kkli::VirtualMachine::addInst(int elem, std::string format) {
	DEBUG_VM("VirtualMachine::addInst(" + getInstructionName(elem) + ")", format);

	if (nextText >= text + SEGMENT_SIZE) throw Error("VirtualMachine::addInst("+std::to_string(elem)+"): text overflow");
	*nextText = elem;
	++nextText;
}

//添加指令的操作数
void kkli::VirtualMachine::addInstData(int elem, std::string format) {
	DEBUG_VM("VirtualMachine::addInstData(" + std::to_string(elem) + ")", format);

	if (nextText >= text + SEGMENT_SIZE) throw Error("VirtualMachine::addInstData(" + std::to_string(elem) + "): text overflow.");
	*nextText = elem;
	++nextText;
}

//获取顶部指令
int kkli::VirtualMachine::getTopInst(std::string format) {
	DEBUG_VM("VirtualMachine::getTopInst(): " + getInstructionName(*(nextText - 1)), format);
	if (nextText <= text) throw Error("VirtualMachine::getTopInst(): no instruction to get!");
	return *(nextText - 1);
}

//删除顶部指令
void kkli::VirtualMachine::deleteTopInst(std::string format) {
	DEBUG_VM("VirtualMachine::deleteTopInst(): " + getInstructionName(*(nextText - 1)), format);

	--nextText;
	if (nextText < text) throw Error("VirtualMachine::deleteTopInst(): no instruction to delete!");
}

//数据对齐
void kkli::VirtualMachine::dataAlignment(std::string format) {
	DEBUG_VM("[before data_align] nextData = "
		+ std::to_string(reinterpret_cast<int>(nextData)) + ".", FORMAT(format));
	
	nextData = reinterpret_cast<char*>((reinterpret_cast<int>(nextData) + 4) & (-4));

	DEBUG_VM("[after data_align] nextData = "
		+ std::to_string(reinterpret_cast<int>(nextData)) + ".", FORMAT(format));

	if (nextData > data + SEGMENT_SIZE) throw Error("VirtualMachine::addIntData(): data overflow");
	needDataAlignment = false;
}

//获取下一个写入数据的位置
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

//获取信息
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

//执行指令
int kkli::VirtualMachine::run() {
	DEBUG_VM("VirtualMachine::run()", "");

	std::string format = "";

	int inst;        //指令
	int cycle = 0;   //执行周期

	//循环执行指令，直到遇到退出指令或未知指令
	while (1) {
		++cycle;
		
		//取指令
		inst = *pc;
		++pc;

		//输出执行的指令
		std::string instInfo = getInstructionName(inst);
		if (inst <= I_ADJ) instInfo += "  " + std::to_string(*pc);
		DEBUG_VM(instInfo, format);

		//TEST
		//std::cout << instInfo << std::endl;
		
		//取立即数
		if (inst == I_IMM) {
			ax = *pc;
			++pc;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//加载char值
		else if (inst == I_LC) {
			ax = *(reinterpret_cast<char*>(ax));
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//加载int值
		else if (inst == I_LI) {
			ax = *(reinterpret_cast<int*>(ax));
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//存储char值
		else if (inst == I_SC) {
			char* addr = reinterpret_cast<char*>(*sp);
			++sp;
			*addr = ax;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//存储int值
		else if (inst == I_SI) {
			int* addr = reinterpret_cast<int*>(*sp);
			++sp;
			*addr = ax;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//入栈
		else if (inst == I_PUSH) {
			*(--sp) = ax;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//无条件跳转
		else if (inst == I_JMP) {
			pc = reinterpret_cast<int*>(*pc);
		}

		//零跳转
		else if (inst == I_JZ) {
			if (ax != 0) pc = pc + 1;
			else pc = reinterpret_cast<int*>(*pc);
		}

		//非零跳转
		else if (inst == I_JNZ) {
			if (ax == 0) pc = pc + 1;
			else pc = reinterpret_cast<int*>(*pc);
		}

		//函数调用
		else if (inst == I_CALL) {
			*(--sp) = reinterpret_cast<int>(pc + 1);
			pc = reinterpret_cast<int*>(*pc);
		}

		//进入函数后
		else if (inst == I_ENT) {
			*(--sp) = reinterpret_cast<int>(bp);
			bp = sp;
			sp = sp - *pc;
			++pc;
		}

		//函数返回时的参数出栈
		else if (inst == I_ADJ) {
			sp = sp + *pc;
			++pc;
		}

		//函数返回
		else if (inst == I_LEV) {
			sp = bp;  //恢复sp
			bp = reinterpret_cast<int*>(*sp);  //恢复bp
			++sp;
			pc = reinterpret_cast<int*>(*sp);  //恢复pc
			++sp;
		}

		//获取基于bp寄存器的位置
		else if (inst == I_LEA) {
			ax = reinterpret_cast<int>(bp + *pc);
			++pc;
		}

		/*
		所有二元运算，第一个操作数在*sp中，第二个操作数在ax中
		*/

		//或运算
		else if (inst == I_OR) {
			ax = (*sp | ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//异或运算
		else if (inst == I_XOR) {
			ax = (*sp ^ ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//与运算
		else if (inst == I_AND) {
			ax = (*sp & ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//相等比较
		else if (inst == I_EQ) {
			ax = (*sp == ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//不等比较
		else if (inst == I_NE) {
			ax = (*sp != ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//小于比较
		else if (inst == I_LT) {
			ax = (*sp < ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//小于等于比较
		else if (inst == I_LE) {
			ax = (*sp <= ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//大于比较
		else if (inst == I_GT) {
			ax = (*sp > ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//大于等于比较
		else if (inst == I_GE) {
			ax = (*sp >= ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//左移运算
		else if (inst == I_SHL) {
			ax = (*sp << ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//右移运算
		else if (inst == I_SHR) {
			ax = (*sp >> ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//相加运算
		else if (inst == I_ADD) {
			ax = (*sp + ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//相减运算
		else if (inst == I_SUB) {
			ax = (*sp - ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//相乘运算
		else if (inst == I_MUL) {
			ax = (*sp * ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//相除运算
		else if (inst == I_DIV) {
			ax = (*sp / ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//相模运算
		else if (inst == I_MOD) {
			ax = (*sp % ax);
			++sp;
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//输出
		else if (inst == I_PRTF) {
			int* temp = sp + pc[1];
			ax = printf(reinterpret_cast<char*>(temp[-1]),
				temp[-2], temp[-3], temp[-4], temp[-5], temp[-6]);
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//动态内存分配
		else if (inst == I_MALC) {
			ax = reinterpret_cast<int>(malloc(*sp));
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
		}

		//退出程序
		else if (inst == I_EXIT) {
			printf("exit(%d)\n", ax);
			return ax;
		}

		//错误的指令
		else {
			throw Error("VirtualMachine::run(): unknown instruction!");
		}
	}
}
