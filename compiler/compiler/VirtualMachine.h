#ifndef __VIRTUAL_MACHINE_H__
#define __VIRTUAL_MACHINE_H__

/*
�ļ���������������塣�����ӵ���ڴ棬�Ĵ������Լ�����ִ��ָ��� ��CPU����
*/

#include "stdafx.h"
#include "Instruction.h"

namespace kkli {

	//========================================
	// �����
	//========================================
	class VirtualMachine {
	private:

		const static int SEGMENT_SIZE = 256 * 1024; //�ڴ�δ�С
		int* data;        //���ݶ�
		int* text;        //�����
		int* stack;       //ջ
		int* nextData;    //��һ��������ݵ�λ��
		int* nextText;    //��һ�����ָ���λ��

		//�����о���Ҫ�õ��Ĵ�������˽�������Ϊpublic
	public:
		int* sp;    //sp�Ĵ���
		int* bp;    //bp�Ĵ���
		int* pc;    //pc�Ĵ���
		int ax;     //ax�Ĵ�����ջ�����棩
		
	public:
		//��ֹ���ơ���ֵ
		VirtualMachine(const VirtualMachine& rhs) = delete;
		const VirtualMachine& operator=(const VirtualMachine& rhs) = delete;

		VirtualMachine();
		
		//������ݡ�ָ��
		void addData(int elem);
		void addInst(int elem);
		
		//ɾ��������ָ��
		void deleteTopInst();

		//getter
		int* getDataSegment() const { return data; }
		int* getTextSegment() const { return text; }
		int* getStack() const { return stack; }

		//ջ����
		int pop();
		void push(int elem);

		//ִ��ָ��
		int run();

		//��ȡ�������Ϣ
		std::string getInfo() const;
	};

	const int VirtualMachine::SEGMENT_SIZE;
}

#endif
