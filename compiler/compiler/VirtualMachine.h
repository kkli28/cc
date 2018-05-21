#ifndef __VIRTUAL_MACHINE_H__
#define __VIRTUAL_MACHINE_H__

#include "stdafx.h"
#include "Instruction.h"

/*
文件描述：虚拟机的定义
*/

namespace kkli {

	//========================================
	// 虚拟机
	//========================================
	class VirtualMachine {
	private:

		//指令名称
		const std::vector<std::string> INSTRUCTION_NAME = {
			"I_NAI",

			//带参数的指令
			"I_LEA ", "I_IMM ", "I_JMP ", "I_CALL", "I_JZ  ", "I_JNZ ", "I_ENT ", "I_ADJ ",

			//不带参数的指令
			"I_LEV ", "I_LI  ", "I_LC  ", "I_SI  ", "I_SC  ", "I_PUSH",
			"I_OR  ", "I_XOR ", "I_AND ", "I_EQ  ", "I_NE  ", "I_LT  ", "I_GT  ", "I_LE  ", "I_GE  ", "I_SHL ", "I_SHR ",
			"I_ADD ", "I_SUB ", "I_MUL ", "I_DIV ", "I_MOD ",
			"I_PRTF", "I_MALC", "I_EXIT", "I_SCANF", "I_GETC", "I_PUTC" //内置函数
		};

		//内存中各段的大小
		const int SEGMENT_SIZE = 2560 * 1024;

	private:
		int* sp;    //sp寄存器
		int* bp;    //bp寄存器
		int* pc;    //pc寄存器
		int ax;     //ax寄存器（栈顶缓存）

		char* data;       //数据段
		int* text;        //代码段
		int* stack;       //栈

		char* nextData;    //下一个存放数据的位置
		int* nextText;    //下一个存放指令的位置
		bool needDataAlignment;  //是否需要数据对齐
		std::vector<std::pair<int, int>> instTag;  //每个全局定义生成的指令

		//进行数据对齐
		void dataAlignment(std::string format);

		//展示VM的 [调用] 信息
		void DEBUG_VM(std::string arg1, std::string arg2) {
			if (DEBUG_INFO->OUTPUT_VM_ACTIONS) {
				Debug::output(arg1, arg2); 
			}
		}
		//展示VM的详细 [执行] 信息
		void DEBUG_VM_EXECUTE_DETAIL(std::string arg1, std::string arg2) {
			if (DEBUG_INFO->OUTPUT_VM_EXECUTE_DETAIL_ACTIONS) { 
				Debug::output(arg1, arg2); 
			}
		}
		//展示VM的详细 [执行] 信息，并且显示每个寄存器当前值
		void DEBUG_REGISTER(std::string format) {
			DEBUG_VM_EXECUTE_DETAIL("ax: " + std::to_string(ax), FORMAT(format));
			DEBUG_VM_EXECUTE_DETAIL("bp: " + std::to_string(reinterpret_cast<int>(bp)), FORMAT(format));
			DEBUG_VM_EXECUTE_DETAIL("sp: " + std::to_string(reinterpret_cast<int>(sp)), FORMAT(format));
			DEBUG_VM_EXECUTE_DETAIL("pc: " + std::to_string(reinterpret_cast<int>(pc)), FORMAT(format));
		}

	public:
		static const int MAX_INT_ARRAY_SIZE = 60'0000;
		static const int MAX_CHAR_ARRAY_SIZE = 250'0000;

		VirtualMachine();

		//设置pc寄存器的值
		void setPC(int* addr) { pc = addr; }

		//添加数据、指令、指令的操作数
		void addDataChar(char elem, std::string format);
		void addDataDefaultChars(int count, std::string format);
		void addDataInt(int count, std::string format);
		void addDataDefaultInts(int count, std::string format);
		void addInst(int elem, std::string format);
		void addInstData(int elem, std::string format);

		//向data域的某个位置写入数据
		void setChars(char* addr, std::vector<char>&& chars, std::string format);
		void setInts(char* addr, std::vector<int>&& chars, std::string format);
		
		//操作顶部的指令
		int getTopInst(std::string format);
		void deleteTopInst(std::string format);
		
		//getter
		char* getDataSegment() { return data; }
		int* getTextSegment() { return text; }
		int* getStack() { return stack; }

		//获取下一个存放位置
		char* getNextDataPos(int dataType, std::string format) ;
		int* getNextTextPos() const { return nextText; }

		//执行指令
		int run();

		//获取虚拟机信息
		std::string getInfo() const;

		/*
		标记全局定义的开始与结束
		每当全局定义开始时，设置该全局定义代码开始位置
		每当全局定义结束时，设置该全局定义代码结束位置
		若需要打印全局定义所生成的代码，则可通过输出两个标记间的代码来实现
		*/
		void setGlobalDeclInstTag(bool isStart);

		//获取所有生成的指令
		std::string getGlobalDeclGenInst(bool hasLineNumber = true);
		std::string getGenInst(bool hasLineNumber = true);

		//获取指令名称
		std::string getInstructionName(int i) const {
			if (i < 0) return "WRONG_INSTRUCTION: " + std::to_string(i);
			else if (i >= INSTRUCTION_SIZE) return "INSTRUCTION_WRONG: " + std::to_string(i);
			return INSTRUCTION_NAME[i]; 
		}
	};
}

#endif
