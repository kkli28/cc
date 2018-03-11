#ifndef __MEMORY_H__
#define __MEMORY_H__

/*
�ļ��������ڴ棬�������ݶΡ�����Ρ�ջ���������Ҫһ���ڴ档
*/

#include "stdafx.h"

namespace kkli {

	//========================================
	// �ڴ�
	//========================================
	class Memory {
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
		Memory(const Memory& rhs) = delete;
		const Memory& operator=(const Memory& rhs) = delete;
		Memory();
		
		//������ݡ�ָ���ָ��Ĳ�����
		void addData(int elem);
		void addText(int elem);

		//��ȡ����
		int* getDataSegment() const { return data; }
		int* getTextSegment() const { return text; }
		int* getStack() const { return stack; }

		//ջ����
		int pop();
		void push(int elem);

		//��ȡ�ڴ���Ϣ
		std::string getInfo() const;
	};

	const int Memory::SEGMENT_SIZE;
}

#endif
