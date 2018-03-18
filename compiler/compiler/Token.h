#ifndef __TOKEN_H__
#define __TOKEN_H__

/*
�ļ���������Ƕ���
*/

#include "stdafx.h"

namespace kkli {

	//�������
	enum {
		ERROR = 0,

		//�û������ʶ��
		ID,

		//����
		NUM,

		//�ؼ���
		CHAR, ELSE, ENUM, IF, INT, RETURN, SIZEOF, WHILE,

		//�����
		ASSIGN, COND,
		LOR,   //�߼��� ||
		LAN,   //�߼��� &&
		OR,    //�� |
		XOR,   //��� ^
		AND,   //�� &
		NOT, EQ, NE, LT, GT, LE, GE,
		SHL, SHR, ADD, SUB, MUL, DIV, MOD,

		//�������Լ�
		INC, DEC,

		//�������
		LPAREN, RPAREN,  // ()
		LBRACK, RBRACK,  // []
		LBRACE, RBRACE,  // {}
		COMMA, COLON, SEMICON, TILDE,  // , : ; ~
		STRING,          //�ַ���
		TOKEN_TYPE_SIZE, //��������

		END = -1    //����Token
	};

	//�������
	enum {
		NUMBER = 1,//����
		FUNC,      //����
		SYS_FUNC,  //ϵͳ�ڲ�����
		GLOBAL,    //ȫ�ֱ���
		LOCAL,     //�ֲ�����
		TOKEN_KLASS_SIZE    //��������
	};

	//��������
	enum {
		CHAR_TYPE,     //char��
		INT_TYPE,      //int��
		PTR_TYPE       //ָ������
	};
	
	//========================================
	// ���
	//========================================

	/*
	˵������Ϊ��֧������������ȫ�֡��ֲ���������ڱ����������������������ֶ�
	     �����뺯���ڲ�ʱ����ǵ����ֶ�ֵ���뱸���ֶ��С������˳�����ʱ���������ֶ�
		 ��ֵ�ָ������ֶ��С�ÿ�ζ��Ǵ����ֶ���ȡֵ��
	*/
	class Token {
	public:
		//���ֶ�
		int type;          //����
		int klass;         //����
		std::string name;  //����
		int dataType;      //��������
		int value;         //ֵ
		int hash;          //hashֵ�����ڱ�ǿ��ٲ���

		//�����ֶ�
		int Btype;
		int Bklass;
		std::string Bname;
		int BdataType;
		int Bvalue;
		int Bhash;

		Token();

	public:
		void saveInfo();      //������Ϣ
		void restoreInfo();   //�ָ���Ϣ
		void clear();         //�����Ϣ

		//��ȡToken��������
		static std::string getTokenTypeName(int type);

		//��ȡTokenKlass��������
		static std::string getTokenKlassName(int klass);

		//��ȡ������������
		static std::string getDataTypeName(int dataType);
	};

}

#endif
