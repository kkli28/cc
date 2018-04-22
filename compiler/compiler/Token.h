#ifndef __TOKEN_H__
#define __TOKEN_H__

/*
文件描述：标记定义
*/

#include "stdafx.h"

namespace kkli {

	//标记类型
	enum {
		END = -1,
		ERROR = 0,

		LPAREN, RPAREN,  // ()
		RBRACK,          // ]
		LBRACE, RBRACE,  // {}
		COMMA, COLON, SEMICON, TILDE,  // , : ; ~

		//用户定义标识符
		ID = 128,

		//整型值、字符型值
		NUM_INT,
		NUM_CHAR,

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

		//自增，自减，[
		INC, DEC, LBRACK,

		STRING    //字符串
	};

	//标记种类
	enum {
		NUMBER = 1,//值（立即数）类型
		FUNC,      //函数
		SYS_FUNC,  //系统内部函数
		GLOBAL,    //全局变量
		LOCAL      //局部变量
	};

	//数据类型
	enum {
		CHAR_TYPE,     //char型
		INT_TYPE,      //int型
		PTR_TYPE,      //指针类型
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
		int type;          //类型
		std::string name;  //名字
		int hash;          //hash值，便于标记快速查找
		std::vector<int> argsDataType;    //函数参数及其类型

		int klass;         //种类
		int dataType;      //数据类型
		int value;         //值

		//备份字段
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
		void saveInfo(std::string format);      //保存信息
		void restoreInfo(std::string format);   //恢复信息
		void addArgument(int dataType, std::string format);         //仅是函数时调用，添加函数的一个参数类型

		//获取Token类型名称
		static std::string getTokenTypeName(int type);

		//获取TokenKlass类型名称
		static std::string getTokenKlassName(int klass);

		//获取数据类型名称
		static std::string getDataTypeName(int dataType);
	};

}

#endif
