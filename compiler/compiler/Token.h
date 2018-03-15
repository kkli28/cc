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
		INC, DEC,

		//�������
		LPAREN, RPAREN,  // ()
		LBRACK, RBRACK,  // []
		LBRACE, RBRACE,  // {}
		COMMA, COLON, SEMICON,  // , : ;
		TOKEN_TYPE_SIZE    //��������
	};

	//��ȡTokenType������
	std::string getTokenTypeName(TokenType type) {
		std::string TOKEN_TYPE_STRING[TOKEN_TYPE_SIZE] = {
			"ID", "CHAR", "ELSE", "ENUM", "IF", "INT", "RETURN", "SIZEOF", "WHILE",
			"ASSIGN", "COND", "LOR", "LAN", "OR", "XOR", "AND", "EQ", "NE", "LT",
			"GT", "LE", "GE", "SHL", "SHR", "ADD", "SUB", "MUL", "DIV", "MOD",
			"INC", "DEC", "LPAREN", "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE",
			"COMMA", "COLON", "SEMICON"
		};

		if (type < 0 || type >= TOKEN_TYPE_SIZE) throw new Error("In getTokenTypeName(): Wrong type !");
		return TOKEN_TYPE_STRING[type];
	}

	//�������
	enum TokenKlass {
		NUM = 0,   //����
		FUNC,      //����
		SYS_FUNC,  //ϵͳ�ڲ�����
		GLOBAL,    //ȫ�ֱ���
		LOCAL,     //�ֲ�����
		TOKEN_KLASS_SIZE    //��������
	};

	//��ȡTokenKlass������
	std::string getTokenKlassName(TokenKlass klass) {
		std::string TOKEN_KLASS_STRING[TOKEN_KLASS_SIZE] = {
			"NUM", "FUNC", "SYS_FUNC", "GLOBAL", "LOCAL"
		};

		if (klass < 0 || klass >= TOKEN_KLASS_SIZE) throw new Error("In getTokenKlassName(): Wrong klass !");
		return TOKEN_KLASS_STRING[klass];
	}

	//��������
	enum DataType {
		CHAR_TYPE = 0,     //char��
		INT_TYPE,          //int��
		PTR_TYPE,          //ָ������
		DATA_TYPE_SIZE     //��������
	};

	//��ȡDataType������
	std::string getDataTypeName(DataType dataType) {
		std::string DATA_TYPE_STRING[DATA_TYPE_SIZE] = {
			"INT_TYPE", "CHAR_TYPE", "PTR_TYPE"
		};

		std::string result;
		int type = dataType;
		while (type >= PTR_TYPE) {
			type -= 2;
			result += "PTR ";
		}
		if (type == CHAR_TYPE) result += "CHAR_TYPE";
		else result += "INT_TYPE";
		return result;
	}

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
		void clear();         //�����Ϣ
	};

	//���캯��
	Token::Token() : type(ID), klass(NUM), name(""),
		dataType(CHAR_TYPE), value(0), hash(0),
		Btype(ID), Bklass(NUM), Bname(""),
		BdataType(CHAR_TYPE), Bvalue(0), Bhash(0) {}

	//������Ϣ
	void Token::saveInfo() {
		if (OUTPUT_TOKEN_ACTIONS) {
			Debug::output(std::string("Token::saveInfo: ")
				+ "type = " + getTokenTypeName(type)
				+ "klass = " + getTokenKlassName(klass));
		}

		Btype = type;
		Bklass = klass;
		Bname = name;
		BdataType = dataType;
		Bvalue = value;
		Bhash = hash;
	}

	//�ָ���Ϣ
	void Token::restoreInfo() {
		if (OUTPUT_TOKEN_ACTIONS) {
			Debug::output(std::string("Token::restoreInfo(): ")
				+ "type = " + getTokenTypeName(Btype)
				+ "klass = " + getTokenKlassName(Bklass));
		}

		type = Btype;
		klass = Bklass;
		name = Bname;
		dataType = BdataType;
		value = Bvalue;
		hash = Bhash;
	}

	//�����Ϣ
	void Token::clear() {
		if (OUTPUT_TOKEN_ACTIONS) {
			Debug::output(std::string("Token::clear(): ")
				+ "type = " + getTokenTypeName(type)
				+ "klass = " + getTokenKlassName(klass));
		}

		type = TokenType::ID;
		klass = TokenKlass::NUM;
		name = "";
		dataType = DataType::INT_TYPE;
		value = 0;
		hash = 0;

		Btype = type;
		Bklass = klass;
		Bname = name;
		BdataType = dataType;
		Bvalue = value;
		Bhash = hash;
	}
}

#endif
