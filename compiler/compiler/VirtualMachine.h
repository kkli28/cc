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
			"I_LEA ", "I_IMM ", "I_JMP ", "I_CALL", "I_JZ  ", "I_JNZ ", "I_ENT ", "I_ADJ ",  //带参数的指令
			"I_LEV ", "I_LI  ", "I_LC  ", "I_SI  ", "I_SC  ", "I_PUSH",
			"I_OR  ", "I_XOR ", "I_AND ", "I_EQ  ", "I_NE  ", "I_LT  ", "I_GT  ", "I_LE  ", "I_GE  ", "I_SHL ", "I_SHR ",
			"I_ADD ", "I_SUB ", "I_MUL ", "I_DIV ", "I_MOD ", //五则运算
			"I_PRTF", "I_MALC", "I_EXIT" //内置函数
		};
		//内存中各段的大小
		const int SEGMENT_SIZE = 2560 * 1024;

	private:
		char* data;       //数据段
		int* text;        //代码段
		int* stack;       //栈
		char* nextData;    //下一个存放数据的位置
		int* nextText;    //下一个存放指令的位置
		bool needDataAlignment;  //是否需要数据对其

		//进行数据对齐
		void dataAlignment(std::string format);

		//程序中经常要用到寄存器，因此将其设置为public
	public:
		int* sp;    //sp寄存器
		int* bp;    //bp寄存器
		int* pc;    //pc寄存器
		int ax;     //ax寄存器（栈顶缓存）

		//禁止构造、复制、赋值
		VirtualMachine(const VirtualMachine& rhs) = delete;
		const VirtualMachine& operator=(const VirtualMachine& rhs) = delete;
		VirtualMachine();
		
	public:

		//单例
		static VirtualMachine* getInstance() {
			static VirtualMachine* vm = nullptr;
			if (vm == nullptr) {
				vm = new VirtualMachine();
			}
			return vm;
		}

		//重置所有信息
		void reset(std::string format);
		
		//添加数据、指令、指令的操作数
		void addDataChar(char elem, std::string format);
		void addDataInt(int elem, std::string format);
		void addDataCharArray(int count, const std::vector<char>& values, std::string format);
		void addDataIntArray(int count, const std::vector<int>& values, std::string format);
		void addInst(int elem, std::string format);
		void addInstData(int elem, std::string format);
		
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
		int run(std::string format);

		//获取虚拟机信息
		std::string getInfo() const;

		//获取指令信息
		const std::vector<std::string>& getInstructionInfo() const { return INSTRUCTION_NAME; }

		//获取指令名称
		std::string getInstructionName(int i) const { return INSTRUCTION_NAME[i]; }
	};
}

#endif
