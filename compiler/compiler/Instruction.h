#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

/*
文件描述：虚拟机指令定义
*/

namespace kkli {

	//========================================
	// 虚拟机指令集
	//========================================
	enum Instruction {

		//////////////////////////////////////////////////
		// Part1: 带参数的指令，如 “IMM 2” 表示取立即数2
		//////////////////////////////////////////////////

		/*
		作用：将相对于BP寄存器的地址放入AX寄存器中。

		用法：I_LEA 12
		将BP寄存器的地址加上12放入AX中。
		函数参数、局部变量等都是相对于BP寄存器存放的，在使用时需要通过BP寄存器获取
		*/
		I_LEA,

		/*
		作用：获取立即数

		用法：I_IMM 2
		将立即数2放入AX寄存器中。立即数存放在PC寄存器所指的位置。
		*/
		I_IMM,

		/*
		作用：无条件跳转

		用法：I_JMP 408F408F，设置PC寄存器的值为408F408F
		*/
		I_JMP,

		/*
		作用：调用函数

		用法：I_CALL 408F408F，调用指令首地址为408F408F的函数。
		调用时在栈中存储下一条指令（PC+1）的位置，并设置PC的值为函数地址。
		*/
		I_CALL,

		/*
		作用：非零则跳转

		用法：I_JZ 408F408F
		若AX中的值为0，则跳转到408F408F，否则不跳转
		*/
		I_JZ,

		/*
		作用：零则跳转

		用法：I_JNZ 408F408F
		若AX中的值不为0，则跳转到408F408F，否则不跳转
		*/
		I_JNZ,

		/*
		作用：进入函数内部

		用法：I_ENT 12
		存储BP寄存器的值，BP寄存器指向旧栈顶，SP预留12个字节的空间用以存放局部变量。
		*/
		I_ENT,

		/*
		作用：函数函数出栈

		用法：I_ADJ 12
		SP的值加12，将函数的参数（共占12个字节）出栈。
		*/
		I_ADJ,

		//////////////////////////////////////////////////
		// Part2: 无参数的指令
		//////////////////////////////////////////////////

		/*
		作用：函数返回。
		将BP寄存器从栈中恢复，将进入函数调用前的下一条指令地址从栈中恢复。
		*/
		I_LEV,

		/*
		作用：从内存中取int值。
		取AX寄存器所指的int值，放入AX寄存器中。
		*/
		I_LI,

		/*
		作用：从内存中取char值。
		取AX寄存器所指的char值，放入AX寄存器中。
		*/
		I_LC,

		/*
		作用：存储int值。
		将AX寄存器中的值存储在SP寄存器所指的地址中。
		*/
		I_SI,

		/*
		作用：存储char值。
		将AX寄存器中的值存储在SP寄存器所指的地址中。
		*/
		I_SC,

		/*
		作用：将AX寄存器中的值入栈。
		*/
		I_PUSH,

		/*
		作用：或运算。
		对栈顶值及AX寄存器中的值进行或运算，结果存放于AX寄存器中。
		*/
		I_OR,

		/*
		作用：异或运算。
		对栈顶值及AX寄存器中的值进行异或运算，结果存放于AX寄存器中。
		*/
		I_XOR,

		/*
		作用：与运算。
		对栈顶值及AX寄存器中的值进行与运算，结果存放于AX寄存器中。
		*/
		I_AND,

		/*
		作用：相等比较。
		对栈顶值及AX寄存器中的值进行相等比较，结果存放于AX寄存器中。
		*/
		I_EQ,

		/*
		作用：不等比较。
		对栈顶值及AX寄存器中的值进行不等比较，结果存放于AX寄存器中。
		*/
		I_NE,

		/*
		作用：小于比较。
		对栈顶值及AX寄存器中的值进行小于比较，结果存放于AX寄存器中。
		*/
		I_LT,

		/*
		作用：大于比较。
		对栈顶值及AX寄存器中的值进行大于比较，结果存放于AX寄存器中。
		*/
		I_GT,

		/*
		作用：小于等于比较。
		对栈顶值及AX寄存器中的值进行小于等于比较，结果存放于AX寄存器中。
		*/
		I_LE,

		/*
		作用：大于等于比较。
		对栈顶值及AX寄存器中的值进行大于等于比较，结果存放于AX寄存器中。
		*/
		I_GE,

		/*
		作用：左移运算。
		将栈顶的值左移（AX寄存器中存放的值）位，结果放入AX寄存器中。
		*/
		I_SHL,

		/*
		作用：右移运算。
		将栈顶的值右移（AX寄存器中存放的值）位，结果放入AX寄存器中。
		*/
		I_SHR,

		/*
		作用：相加运算。
		将栈顶的值与AX寄存器中的值相加，结果放入AX寄存器中。
		*/
		I_ADD,

		/*
		作用：相减运算。
		将栈顶的值减去AX寄存器中的值，结果放入AX寄存器中。
		*/
		I_SUB,

		/*
		作用：相乘运算。
		将栈顶的值与AX寄存器中的值相乘，结果放入AX寄存器中。
		*/
		I_MUL,

		/*
		作用：相除运算。
		将栈顶的值除以AX寄存器中的值，结果放入AX寄存器中。
		*/
		I_DIV,

		/*
		作用：求模运算。
		将栈顶的值模上AX寄存器中的值，结果放入AX寄存器中。
		*/
		I_MOD,

		/*
		作用：打印输出。
		调用C函数printf实现输出功能，最多6个控制参数（共7个，其中第一个为控制字符串）。
		*/
		I_PRTF,

		/*
		作用：动态内存分配。
		调用C函数malloc实现动态内存分配。
		*/
		I_MALC,

		/*
		作用：退出程序。
		调用C函数exit退出程序。
		*/
		I_EXIT,

		/*
		作用：非指令集的一部分，仅用于统计指令的条数
		*/
		INSTRUCTION_SIZE

		//TODO: 尝试提供scanf
	};
}

#endif
