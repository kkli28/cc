#ifndef __VIRTUAL_MACHINE_H__
#define __VIRTUAL_MACHINE_H__

/*
文件描述：虚拟机定义。虚拟机拥有内存，寄存器，以及可以执行指令的 “CPU”。
*/

#include "stdafx.h"
#include "Instruction.h"

namespace kkli {

	//========================================
	// 虚拟机
	//========================================
	class VirtualMachine {
	private:

		const static int SEGMENT_SIZE = 256 * 1024; //内存段大小
		int* data;        //数据段
		int* text;        //代码段
		int* stack;       //栈
		int* nextData;    //下一个存放数据的位置
		int* nextText;    //下一个存放指令的位置

		//程序中经常要用到寄存器，因此将其设置为public
	public:
		int* sp;    //sp寄存器
		int* bp;    //bp寄存器
		int* pc;    //pc寄存器
		int ax;     //ax寄存器（栈顶缓存）
		
	public:
		//禁止复制、赋值
		VirtualMachine(const VirtualMachine& rhs) = delete;
		const VirtualMachine& operator=(const VirtualMachine& rhs) = delete;

		VirtualMachine();
		
		//添加数据、指令
		void addData(int elem);
		void addInst(int elem);
		
		//删除顶部的指令
		void deleteTopInst();

		//getter
		int* getDataSegment() const { return data; }
		int* getTextSegment() const { return text; }
		int* getStack() const { return stack; }

		//栈操作
		int pop();
		void push(int elem);

		//执行指令
		int run();

		//获取虚拟机信息
		std::string getInfo() const;
	};

	const int VirtualMachine::SEGMENT_SIZE;
}

#endif
