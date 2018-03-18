#ifndef __TOKEN_H__
#define __TOKEN_H__

/*
�ļ���������Ƕ���
*/

#include "stdafx.h"

namespace kkli {

	//�������
	enum TokenType {

		//�û������ʶ��
		ID = 0, 

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
		END,             //������
		TOKEN_TYPE_SIZE  //��������
	};

	//�������
	enum TokenKlass {
		NUMBER = 0,//����
		FUNC,      //����
		SYS_FUNC,  //ϵͳ�ڲ�����
		GLOBAL,    //ȫ�ֱ���
		LOCAL,     //�ֲ�����
		TOKEN_KLASS_SIZE    //��������
	};

	//��������
	enum {
		CHAR_TYPE = 0,     //char��
		INT_TYPE,          //int��
		PTR_TYPE,          //ָ������
		DATA_TYPE_SIZE     //��������
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
		TokenType type;        //����
		TokenKlass klass;      //����
		std::string name;      //����
		int dataType;     //��������
		int value;             //ֵ
		int hash;              //hashֵ�����ڱ�ǿ��ٲ���

		//�����ֶ�
		TokenType Btype;
		TokenKlass Bklass;
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
		static std::string getTokenTypeName(TokenType type);

		//��ȡTokenKlass��������
		static std::string getTokenKlassName(TokenKlass klass);

		//��ȡ������������
		static std::string getDataTypeName(int dataType);
	};

}

#endif
