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
		INC, DEC
	};

	//标记种类
	enum TokenKlass {
		NUM = 0,   //数字
		FUNC,      //函数
		SYS_FUNC,  //系统内部函数
		GLOBAL,    //全局变量
		LOCAL      //局部变量
	};

	//数据类型
	enum DataType {
		INT_TYPE = 0,     //int型
		CHAR_TYPE,        //char型
		PTR_TYPE          //指针类型
	};

	//========================================
	// 标记类
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
	};

	//构造函数
	Token::Token() : type(ID), klass(NUM), name(""),
		dataType(INT_TYPE), value(0), hash(0),
		Btype(ID), Bklass(NUM), Bname(""),
		BdataType(INT_TYPE), Bvalue(0), Bhash(0) {}

	//保存信息
	void Token::saveInfo() {
		Btype = type;
		Bklass = klass;
		Bname = name;
		BdataType = dataType;
		Bvalue = value;
		Bhash = hash;
	}

	//恢复信息
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
