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

		//�����о���Ҫ�õ��Ĵ�������˽�������Ϊpublic
	public:
		int* sp;    //sp�Ĵ���
		int* bp;    //bp�Ĵ���
		int* pc;    //pc�Ĵ���
		int ax;     //ax�Ĵ�����ջ�����棩

		//��ֹ���졢���ơ���ֵ
		VirtualMachine(const VirtualMachine& rhs) = delete;
		const VirtualMachine& operator=(const VirtualMachine& rhs) = delete;
		VirtualMachine();
		
	public:

		//����
		static VirtualMachine* getInstance() {
			static VirtualMachine* vm = nullptr;
			if (vm == nullptr) {
				vm = new VirtualMachine();
			}
			return vm;
		}

		//����������Ϣ
		void reset(std::string format);
		
		//������ݡ�ָ�ָ��Ĳ�����
		void addDataChar(char elem, std::string format);
		void addDataInt(int elem, std::string format);
		void addDataCharArray(int count, const std::vector<char>& values, std::string format);
		void addDataIntArray(int count, const std::vector<int>& values, std::string format);
		void addInst(int elem, std::string format);
		void addInstData(int elem, std::string format);
		
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
		int run(std::string format);

		//��ȡ�������Ϣ
		std::string getInfo() const;

		//��ȡָ����Ϣ
		const std::vector<std::string>& getInstructionInfo() const { return INSTRUCTION_NAME; }

		//��ȡָ������
		std::string getInstructionName(int i) const { return INSTRUCTION_NAME[i]; }
	};
}

#endif
