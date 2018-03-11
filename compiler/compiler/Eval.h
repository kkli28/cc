#ifndef __EVAL_H__
#define __EVAL_H__

/*
�ļ����������������
*/

#include "stdafx.h"
#include "Instruction.h"

namespace kkli {

	//�������ǰ������
	class Eval;

	//������Ĵ����
	class Text {
	private:
		int data[Eval::POOL_SIZE];  //������ڴ�
		int* next;					//���������һ��ָ����λ��

	public:
		Text(const Text& rhs) = delete;
		const Text& operator=(const Text& rhs) = delete;

		Text() { next = data; }
		
	};

	//����������ݶ�
	class Data {
	private:
	public:
	};

	//�������ջ
	class Stack {
	private:
	public:
	};

	//========================================
	// �����
	//========================================
	class Eval {
	private:
		const std::string INSTRUCTION_STRING[INSTRUCTION_SIZE] = {
			"LEA ", "IMM ", "JMP ", "CALL", "JZ  ", "JNZ ", "ENT ", "ADJ ",  //��������ָ��
			"LEV ", "LI  ", "LC  ", "SI  ", "SC  ", "PUSH",
			"OR  ", "XOR ", "AND ", "EQ  ", "NE  ", "LT  ", "GT  ", "LE  ", "GE  ", "SHL ", "SHR ",
			"ADD ", "SUB ", "MUL ", "DIV ", "MOD ", //��������
			"PRTF", "MALC", "EXIT" //���ú���
		};
		Eval();

	public:
		Eval() = delete;
		Eval(const Eval& rhs) = delete;
		const Eval& operator=(const Eval& rhs) = delete;

		//����
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

		//ÿ���ε��ڴ��С
		const static int POOL_SIZE = 1024 * 256;
	};

	//��̬�����ⲿ����
	Text Eval::text;
	Data Eval::data;
	Stack Eval::stack;
	const int Eval::POOL_SIZE;
}

#endif
