#ifndef __TOKEN_H__
#define __TOKEN_H__

/*
�ļ���������Ƕ���
*/

#include "stdafx.h"

namespace kkli {

	//�������
	enum {
		END = -1,
		ERROR = 0,

		LPAREN, RPAREN,  // ()
		RBRACK,          // ]
		LBRACE, RBRACE,  // {}
		COMMA, COLON, SEMICON, TILDE,  // , : ; ~

		//�û������ʶ��
		ID = 128,

		//����ֵ���ַ���ֵ
		NUM_INT,
		NUM_CHAR,

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

		//�������Լ���[
		INC, DEC, LBRACK,

		STRING    //�ַ���
	};

	//�������
	enum {
		NUMBER = 1,//ֵ��������������
		FUNC,      //����
		SYS_FUNC,  //ϵͳ�ڲ�����
		GLOBAL,    //ȫ�ֱ���
		LOCAL      //�ֲ�����
	};

	//��������
	enum {
		CHAR_TYPE,     //char��
		INT_TYPE,      //int��
		PTR_TYPE,      //ָ������
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
		std::string name;  //����
		int hash;          //hashֵ�����ڱ�ǿ��ٲ���
		std::vector<int> argsDataType;    //����������������

		int klass;         //����
		int dataType;      //��������
		int value;         //ֵ

		//�����ֶ�
		int Bklass;
		int BdataType;
		int Bvalue;

		Token();

	private:
		void DEBUG_TOKEN(std::string arg1, std::string arg2) {
			if (DEBUG_INFO->OUTPUT_TOKEN_ACTIONS) {
				Debug::output(arg1, arg2); 
			}
		}

	public:
		void saveInfo(std::string format);      //������Ϣ
		void restoreInfo(std::string format);   //�ָ���Ϣ
		void addArgument(int dataType, std::string format);         //���Ǻ���ʱ���ã���Ӻ�����һ����������

		//��ȡToken��������
		static std::string getTokenTypeName(int type);

		//��ȡTokenKlass��������
		static std::string getTokenKlassName(int klass);

		//��ȡ������������
		static std::string getDataTypeName(int dataType);
	};

}

#endif
