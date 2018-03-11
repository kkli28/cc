#ifndef __EVAL_H__
#define __EVAL_H__

/*
文件描述：虚拟机定义
*/

#include "stdafx.h"
#include "Instruction.h"

namespace kkli {

	//虚拟机的前置声明
	class Eval;

	//虚拟机的代码段
	class Text {
	private:
		int data[Eval::POOL_SIZE];  //代码段内存
		int* next;					//代码段中下一条指令存放位置

	public:
		Text(const Text& rhs) = delete;
		const Text& operator=(const Text& rhs) = delete;

		Text() { next = data; }
		
	};

	//虚拟机的数据段
	class Data {
	private:
	public:
	};

	//虚拟机的栈
	class Stack {
	private:
	public:
	};

	//========================================
	// 虚拟机
	//========================================
	class Eval {
	private:
		const std::string INSTRUCTION_STRING[INSTRUCTION_SIZE] = {
			"LEA ", "IMM ", "JMP ", "CALL", "JZ  ", "JNZ ", "ENT ", "ADJ ",  //带参数的指令
			"LEV ", "LI  ", "LC  ", "SI  ", "SC  ", "PUSH",
			"OR  ", "XOR ", "AND ", "EQ  ", "NE  ", "LT  ", "GT  ", "LE  ", "GE  ", "SHL ", "SHR ",
			"ADD ", "SUB ", "MUL ", "DIV ", "MOD ", //五则运算
			"PRTF", "MALC", "EXIT" //内置函数
		};
		Eval();

	public:
		Eval() = delete;
		Eval(const Eval& rhs) = delete;
		const Eval& operator=(const Eval& rhs) = delete;

		//单例
		static Eval* getInstance() {
			static Eval* eval = nullptr;
			if (eval == nullptr) {
				eval = new Eval();
			}
			return eval;
		}
		
	public:
		static Text text;
		static Data data;
		static Stack stack;

		//每个段的内存大小
		const static int POOL_SIZE = 1024 * 256;
	};

	//静态变量外部定义
	Text Eval::text;
	Data Eval::data;
	Stack Eval::stack;
	const int Eval::POOL_SIZE;
}

#endif
