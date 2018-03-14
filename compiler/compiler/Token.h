#ifndef __TOKEN_H__
#define __TOKEN_H__

/*
�ļ���������Ƕ���
*/

#include "stdafx.h"
#include <string>

namespace kkli {

	//�������
	enum TokenType {

		//�û������ʶ��
		ID = 0, 
		
		//�ؼ���
		CHAR, ELSE, ENUM, IF, INT, RETURN, SIZEOF, WHILE,

		//�����
		ASSIGN, COND, LOR, LAN, OR, XOR, AND, EQ, NE, LT, GT, LE, GE,
		SHL, SHR, ADD, SUB, MUL, DIV, MOD, 
		
		//�������Լ�
		INC, DEC
	};

	//�������
	enum TokenKlass {
		NUM = 0,   //����
		FUNC,      //����
		SYS_FUNC,  //ϵͳ�ڲ�����
		GLOBAL,    //ȫ�ֱ���
		LOCAL      //�ֲ�����
	};

	//��������
	enum DataType {
		INT_TYPE = 0,     //int��
		CHAR_TYPE,        //char��
		PTR_TYPE          //ָ������
	};

	//========================================
	// �����
	//========================================

	/*
	˵������Ϊ��֧������������ȫ�֡��ֲ���������ڱ����������������������ֶ�
	     �����뺯���ڲ�ʱ����ǵ����ֶ�ֵ���뱸���ֶ��С������˳�����ʱ���������ֶ�
		 ��ֵ�ָ������ֶ��С�ÿ�ζ��Ǵ����ֶ���ȡֵ��
	*/
	class Token {
	public:
		//���ֶ�
		TokenType type;        //����
		TokenKlass klass;      //����
		std::string name;      //����
		DataType dataType;     //��������
		int value;             //ֵ
		int hash;              //hashֵ�����ڱ�ǿ��ٲ���

		//�����ֶ�
		TokenType Btype;
		TokenKlass Bklass;
		std::string Bname;
		DataType BdataType;
		int Bvalue;
		int Bhash;

		Token();

	public:
		void saveInfo();      //������Ϣ
		void restoreInfo();   //�ָ���Ϣ
	};

	//���캯��
	Token::Token() : type(ID), klass(NUM), name(""),
		dataType(INT_TYPE), value(0), hash(0),
		Btype(ID), Bklass(NUM), Bname(""),
		BdataType(INT_TYPE), Bvalue(0), Bhash(0) {}

	//������Ϣ
	void Token::saveInfo() {
		Btype = type;
		Bklass = klass;
		Bname = name;
		BdataType = dataType;
		Bvalue = value;
		Bhash = hash;
	}

	//�ָ���Ϣ
	void Token::restoreInfo() {
		type = Btype;
		klass = Bklass;
		name = Bname;
		dataType = BdataType;
		value = Bvalue;
		hash = Bhash;
	}
}

#endif
