#ifndef __TOKEN_H__
#define __TOKEN_H__

/*
文件描述：标记定义
*/

#include "stdafx.h"

namespace kkli {

	//标记类型
	enum TokenType {

		//用户定义标识符
		ID = 0, 

		//数字
		NUM,
		
		//关键字
		CHAR, ELSE, ENUM, IF, INT, RETURN, SIZEOF, WHILE,

		//运算符
		ASSIGN, COND, 
		LOR,   //逻辑或 ||
		LAN,   //逻辑与 &&
		OR,    //或 |
		XOR,   //异或 ^
		AND,   //与 &
		NOT, EQ, NE, LT, GT, LE, GE,
		SHL, SHR, ADD, SUB, MUL, DIV, MOD, 
		
		//自增，自减
		INC, DEC,

		//其他标记
		LPAREN, RPAREN,  // ()
		LBRACK, RBRACK,  // []
		LBRACE, RBRACE,  // {}
		COMMA, COLON, SEMICON, TILDE,  // , : ; ~
		STRING,          //字符串
		END,             //结束符
		TOKEN_TYPE_SIZE  //仅计数用
	};

	//标记种类
	enum TokenKlass {
		NUMBER = 0,//数字
		FUNC,      //函数
		SYS_FUNC,  //系统内部函数
		GLOBAL,    //全局变量
		LOCAL,     //局部变量
		TOKEN_KLASS_SIZE    //仅计数用
	};

	//数据类型
	enum {
		CHAR_TYPE = 0,     //char型
		INT_TYPE,          //int型
		PTR_TYPE,          //指针类型
		DATA_TYPE_SIZE     //仅计数用
	};
	
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
		int dataType;     //数据类型
		int value;             //值
		int hash;              //hash值，便于标记快速查找

		//备份字段
		TokenType Btype;
		TokenKlass Bklass;
		std::string Bname;
		int BdataType;
		int Bvalue;
		int Bhash;

		Token();

	public:
		void saveInfo();      //保存信息
		void restoreInfo();   //恢复信息
		void clear();         //清空信息

		//获取Token类型名称
		static std::string getTokenTypeName(TokenType type);

		//获取TokenKlass类型名称
		static std::string getTokenKlassName(TokenKlass klass);

		//获取数据类型名称
		static std::string getDataTypeName(int dataType);
	};

}

#endif
