#ifndef __TOKEN_H__
#define __TOKEN_H__

/*
文件描述：标记定义
*/

#include "stdafx.h"
#include <string>

namespace kkli {

	//标记类型
	enum TokenType {

		//用户定义标识符
		ID = 0, 
		
		//关键字
		CHAR, ELSE, ENUM, IF, INT, RETURN, SIZEOF, WHILE,

		//运算符
		ASSIGN, COND, LOR, LAN, OR, XOR, AND, EQ, NE, LT, GT, LE, GE,
		SHL, SHR, ADD, SUB, MUL, DIV, MOD, 
		
		//自增，自减
		INC, DEC,

		//其他标记
		LPAREN, RPAREN,  // ()
		LBRACK, RBRACK,  // []
		LBRACE, RBRACE,  // {}
		COMMA, COLON, SEMICON,  // , : ;
		TOKEN_TYPE_SIZE    //仅计数用
	};

	//获取TokenType的名字
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

	//标记种类
	enum TokenKlass {
		NUM = 0,   //数字
		FUNC,      //函数
		SYS_FUNC,  //系统内部函数
		GLOBAL,    //全局变量
		LOCAL,     //局部变量
		TOKEN_KLASS_SIZE    //仅计数用
	};

	//获取TokenKlass的名字
	std::string getTokenKlassName(TokenKlass klass) {
		std::string TOKEN_KLASS_STRING[TOKEN_KLASS_SIZE] = {
			"NUM", "FUNC", "SYS_FUNC", "GLOBAL", "LOCAL"
		};

		if (klass < 0 || klass >= TOKEN_KLASS_SIZE) throw new Error("In getTokenKlassName(): Wrong klass !");
		return TOKEN_KLASS_STRING[klass];
	}

	//数据类型
	enum DataType {
		CHAR_TYPE = 0,     //char型
		INT_TYPE,          //int型
		PTR_TYPE,          //指针类型
		DATA_TYPE_SIZE     //仅计数用
	};

	//获取DataType的名字
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
	// 标记
	//========================================

	/*
	说明：因为仅支持两种作用域（全局、局部），因此在标记内设置这两种作用域的字段
	     当进入函数内部时，标记的主字段值放入备份字段中。而当退出函数时，将备份字段
		 的值恢复到主字段中。每次都是从主字段中取值。
	*/
	class Token {
	public:
		//主字段
		TokenType type;        //类型
		TokenKlass klass;      //种类
		std::string name;      //名字
		DataType dataType;     //数据类型
		int value;             //值
		int hash;              //hash值，便于标记快速查找

		//备份字段
		TokenType Btype;
		TokenKlass Bklass;
		std::string Bname;
		DataType BdataType;
		int Bvalue;
		int Bhash;

		Token();

	public:
		void saveInfo();      //保存信息
		void restoreInfo();   //恢复信息
		void clear();         //清空信息
	};

	//构造函数
	Token::Token() : type(ID), klass(NUM), name(""),
		dataType(CHAR_TYPE), value(0), hash(0),
		Btype(ID), Bklass(NUM), Bname(""),
		BdataType(CHAR_TYPE), Bvalue(0), Bhash(0) {}

	//保存信息
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

	//恢复信息
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

	//清空信息
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
