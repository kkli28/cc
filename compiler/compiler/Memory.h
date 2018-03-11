#ifndef __MEMORY_H__
#define __MEMORY_H__

/*
文件描述：内存，包括数据段、代码段、栈。虚拟机需要一个内存。
*/

#include "stdafx.h"

namespace kkli {

	//========================================
	// 内存
	//========================================
	class Memory {
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
		Memory(const Memory& rhs) = delete;
		const Memory& operator=(const Memory& rhs) = delete;
		Memory();
		
		//添加数据、指令、及指令的操作数
		void addData(int elem);
		void addText(int elem);

		//获取数据
		int* getDataSegment() const { return data; }
		int* getTextSegment() const { return text; }
		int* getStack() const { return stack; }

		//栈操作
		int pop();
		void push(int elem);

		//获取内存信息
		std::string getInfo() const;
	};

	const int Memory::SEGMENT_SIZE;
}

#endif
