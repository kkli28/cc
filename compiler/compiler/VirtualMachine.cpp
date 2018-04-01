#include "stdafx.h"
#include "VirtualMachine.h"

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
}

//添加char型数据
void kkli::VirtualMachine::addDataChar(char elem, std::string format) {
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::addCharData(" + std::to_string(elem) + ")", format);
	}

	if (nextData == data + SEGMENT_SIZE) throw Error("VirtualMachine::addCharData(): data overflow");

	*(reinterpret_cast<int*>(nextData)) = elem;
	++nextData;
}

//添加int型数据
void kkli::VirtualMachine::addDataInt(int elem, std::string format) {
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::addIntData(" + std::to_string(elem) + ")", format);
	}

	if (OUTPUT_VM_ACTIONS) {
		Debug::output("[before data_align] nextData = "
			+ std::to_string(reinterpret_cast<int>(nextData)) + ".", FORMAT(format));
	}

	//数据对齐
	nextData = reinterpret_cast<char*>((reinterpret_cast<int>(data) + 4) & (-4));
	
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("[after data_align] nextData = "
			+ std::to_string(reinterpret_cast<int>(nextData)) + ".", FORMAT(format));
	}

	if (nextData == data + SEGMENT_SIZE) throw Error("VirtualMachine::addIntData(): data overflow");

	*(reinterpret_cast<int*>(nextData)) = elem;
	nextData += 4;
}

//添加指令
void kkli::VirtualMachine::addInst(int elem, std::string format) {

	//调试
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::addInst(" + getInstructionName(elem) + ")", format);
	}

	if (nextText == text + SEGMENT_SIZE) throw Error("VirtualMachine::addText(): text overflow");
	*nextText = elem;
	++nextText;
}

//添加指令的操作数
void kkli::VirtualMachine::addInstData(int elem, std::string format) {
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::addInstData(" + std::to_string(elem) + ")", format);
	}

	if (nextText == text + SEGMENT_SIZE) throw Error("VirtualMachine::addText(): text overflow");

	*nextText = elem;
	++nextText;
}

//获取顶部指令
int kkli::VirtualMachine::getTopInst(std::string format) {
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::getTopInst(): " + getInstructionName(*(nextText - 1)), format);
	}
	if (nextText - 1 < text) throw Error("VirtualMachine::getTopInst(): no instruction to get!");
	return *(nextText - 1);
}

//删除顶部指令
void kkli::VirtualMachine::deleteTopInst(std::string format) {
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::deleteTopInst(): " + getInstructionName(*(nextText - 1)), format);
	}

	--nextText;
	if (nextText < text) throw Error("VirtualMachine::deleteTopInst(): no instruction to delete!");
}

//出栈
int kkli::VirtualMachine::pop(std::string format) {

	//调试
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::pop(): " + std::to_string(*sp), format);
	}

	if (sp == stack) throw Error("empty stack!");
	return *(sp++);
}

//入栈
void kkli::VirtualMachine::push(int elem, std::string format) {

	//调试
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::push(" + std::to_string(elem) + ")", format);
	}

	if (sp == stack + SEGMENT_SIZE) throw Error("stack overflow!");
	*sp = elem;
	*(--sp) = elem;
}

//获取内存信息
std::string kkli::VirtualMachine::getInfo() const {

	//拼装虚拟机信息
	std::string str;
	str += "\n[VM info]:";
	str += "[SEGMENT_SIZE]: " + std::to_string(SEGMENT_SIZE) + "\n";
	str += "[data]: " + std::to_string(int(data)) + "\n";
	str += "[text]: " + std::to_string(int(text)) + "\n";
	str += "[stack]: " + std::to_string(int(stack)) + "\n";
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
int kkli::VirtualMachine::run(std::string format) {
	if (OUTPUT_VM_ACTIONS) {
		Debug::output("VirtualMachine::run()", format);
	}
	
	int inst;        //指令
	int cycle = 0;   //执行周期

	//循环执行指令，直到遇到退出指令或未知指令
	while (1) {
		++cycle;

		//取指令
		inst = *pc;
		++pc;

		//输出执行的指令
		if (OUTPUT_VM_ACTIONS) {
			std::string debugInfo = "execute: ";
			debugInfo += getInstructionName(inst);
			if (inst <= I_ADJ) debugInfo += "  " + std::to_string(*pc);
			Debug::output(debugInfo, FORMAT(format));
		}

		//取立即数
		if (inst == I_IMM) {
			ax = *pc;
			++pc;
		}

		//加载char值
		else if (inst == I_LC) {
			ax = *(reinterpret_cast<char*>(ax));
		}

		//加载int值
		else if (inst == I_LI) {
			ax = *(reinterpret_cast<int*>(ax));
		}

		//存储char值
		else if (inst == I_SC) {
			char* addr = reinterpret_cast<char*>(*sp);
			++sp;
			*addr = ax;
		}

		//存储int值
		else if (inst == I_SI) {
			int* addr = reinterpret_cast<int*>(*sp);
			++sp;
			*addr = ax;
		}

		//入栈
		else if (inst == I_PUSH) {
			*(--sp) = ax;
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

		//进入函数之前
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
		}

		//异或运算
		else if (inst == I_XOR) {
			ax = (*sp ^ ax);
			++sp;
		}

		//与运算
		else if (inst == I_AND) {
			ax = (*sp & ax);
			++sp;
		}

		//相等比较
		else if (inst == I_EQ) {
			ax = (*sp == ax);
			++sp;
		}

		//不等比较
		else if (inst == I_NE) {
			ax = (*sp != ax);
			++sp;
		}

		//小于比较
		else if (inst == I_LT) {
			ax = (*sp < ax);
			++sp;
		}
		
		//小于等于比较
		else if (inst == I_LE) {
			ax = (*sp <= ax);
			++sp;
		}

		//大于比较
		else if (inst == I_GT) {
			ax = (*sp > ax);
			++sp;
		}

		//大于等于比较
		else if (inst == I_GE) {
			ax = (*sp >= ax);
			++sp;
		}

		//左移运算
		else if (inst == I_SHL) {
			ax = (*sp << ax);
			++sp;
		}

		//右移运算
		else if (inst == I_SHR) {
			ax = (*sp >> ax);
			++sp;
		}

		//相加运算
		else if (inst == I_ADD) {
			ax = (*sp + ax);
			++sp;
		}

		//相减运算
		else if (inst == I_SUB) {
			ax = (*sp - ax);
			++sp;
		}

		//相乘运算
		else if (inst == I_MUL) {
			ax = (*sp * ax);
			++sp;
		}

		//相除运算
		else if (inst == I_DIV) {
			ax = (*sp / ax);
			++sp;
		}

		//相模运算
		else if (inst == I_MOD) {
			ax = (*sp % ax);
			++sp;
		}

		//输出
		else if (inst == I_PRTF) {
			int* temp = sp + pc[1];
			ax = printf(reinterpret_cast<char*>(temp[-1]),
				temp[-2], temp[-3], temp[-4], temp[-5], temp[-6]);
		}

		//动态内存分配
		else if (inst == I_MALC) {
			ax = reinterpret_cast<int>(malloc(*sp));
		}

		//退出程序
		else if (inst == I_EXIT) {
			printf("exit(%d)", *sp);
			return *sp;
		}
		
		//错误的指令
		else {
			throw Error("unknown instruction!");
		}

		//std::cout << getInfo() << std::endl;;
	}
}
