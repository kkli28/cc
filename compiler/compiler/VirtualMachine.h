#ifndef __VIRTUAL_MACHINE_H__
#define __VIRTUAL_MACHINE_H__

#include "stdafx.h"
#include "Instruction.h"

/*
�ļ�������������Ķ���
*/

namespace kkli {

	//========================================
	// �����
	//========================================
	class VirtualMachine {
	private:

		//ָ������
		const std::vector<std::string> INSTRUCTION_NAME = {
			"I_LEA ", "I_IMM ", "I_JMP ", "I_CALL", "I_JZ  ", "I_JNZ ", "I_ENT ", "I_ADJ ",  //��������ָ��
			"I_LEV ", "I_LI  ", "I_LC  ", "I_SI  ", "I_SC  ", "I_PUSH",
			"I_OR  ", "I_XOR ", "I_AND ", "I_EQ  ", "I_NE  ", "I_LT  ", "I_GT  ", "I_LE  ", "I_GE  ", "I_SHL ", "I_SHR ",
			"I_ADD ", "I_SUB ", "I_MUL ", "I_DIV ", "I_MOD ", //��������
			"I_PRTF", "I_MALC", "I_EXIT" //���ú���
		};

		//�ڴ��и��εĴ�С
		const int SEGMENT_SIZE = 2560 * 1024;

	private:
		char* data;       //���ݶ�
		int* text;        //�����
		int* stack;       //ջ
		char* nextData;    //��һ��������ݵ�λ��
		int* nextText;    //��һ�����ָ���λ��
		bool needDataAlignment;  //�Ƿ���Ҫ���ݶ���

		//�������ݶ���
		void dataAlignment(std::string format);

		//DEBUG_VM
		void DEBUG_VM(std::string arg1, std::string arg2) {
			if (DEBUG_INFO->OUTPUT_VM_ACTIONS) {
				Debug::output(arg1, arg2); 
			}
		}
		//DEBUG_VM_EXECUTE
		void DEBUG_VM_EXECUTE(std::string arg1, std::string arg2) {
			if (DEBUG_INFO->OUTPUT_VM_EXECUTE_ACTIONS) { 
				Debug::output(arg1, arg2); 
			}
		}
		//DEBUG_REGISTER
		void DEBUG_REGISTER(std::string format) {
			DEBUG_VM_EXECUTE("ax: " + std::to_string(ax), FORMAT(format));
			DEBUG_VM_EXECUTE("bp: " + std::to_string(reinterpret_cast<int>(bp)), FORMAT(format));
			DEBUG_VM_EXECUTE("sp: " + std::to_string(reinterpret_cast<int>(sp)), FORMAT(format));
			DEBUG_VM_EXECUTE("pc: " + std::to_string(reinterpret_cast<int>(pc)), FORMAT(format));
		}

	public:
		//�����о���Ҫ�õ��Ĵ�������˽�������Ϊpublic����ʹ��
		int* sp;    //sp�Ĵ���
		int* bp;    //bp�Ĵ���
		int* pc;    //pc�Ĵ���
		int ax;     //ax�Ĵ�����ջ�����棩

		static const int MAX_INT_ARRAY_SIZE = 60'0000;
		static const int MAX_CHAR_ARRAY_SIZE = 250'0000;

	public:

		VirtualMachine();

		//������ݡ�ָ�ָ��Ĳ�����
		void addDataChar(char elem, std::string format);
		void addDataDefaultChars(int count, std::string format);
		void addDataInt(int count, std::string format);
		void addDataDefaultInts(int count, std::string format);
		void addInst(int elem, std::string format);
		void addInstData(int elem, std::string format);

		//��data���ĳ��λ��д������
		void setChars(char* addr, std::vector<char>&& chars, std::string format);
		void setInts(char* addr, std::vector<int>&& chars, std::string format);
		
		//����������ָ��
		int getTopInst(std::string format);
		void deleteTopInst(std::string format);
		
		//getter
		char* getDataSegment() { return data; }
		int* getTextSegment() { return text; }
		int* getStack() { return stack; }

		//��ȡ��һ�����λ��
		char* getNextDataPos(int dataType, std::string format) ;
		int* getNextTextPos() const { return nextText; }

		//ִ��ָ��
		int run();

		//��ȡ�������Ϣ
		std::string getInfo() const;

		//��ȡָ����Ϣ
		const std::vector<std::string>& getInstructionInfo() const { return INSTRUCTION_NAME; }

		//��ȡָ������
		std::string getInstructionName(int i) const { 
			if (i < 0) return "WRONG_INSTRUCTION";
			else if (i >= INSTRUCTION_SIZE) return "INSTRUCTION_DATA";
			return INSTRUCTION_NAME[i]; 
		}
	};
}

#endif
