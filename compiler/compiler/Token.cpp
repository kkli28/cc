#include "stdafx.h"
#include "Token.h"

//构造函数
kkli::Token::Token() : type(0), klass(0), name(""), dataType(0), value(0), hash(0), Btype(0),
Bklass(0), Bname(""), BdataType(0), Bvalue(0), Bhash(0) {}

//保存信息
void kkli::Token::saveInfo() {
	if (OUTPUT_TOKEN_ACTIONS) {
		Debug::output(std::string("Token::saveInfo: ")
			+ "type = " + getTokenTypeName(type)
			+ "  klass = " + getTokenKlassName(klass));
	}

	Btype = type;
	Bklass = klass;
	Bname = name;
	BdataType = dataType;
	Bvalue = value;
	Bhash = hash;
}

//恢复信息
void kkli::Token::restoreInfo() {
	if (OUTPUT_TOKEN_ACTIONS) {
		Debug::output(std::string("Token::restoreInfo(): ")
			+ "type = " + getTokenTypeName(Btype)
			+ "  klass = " + getTokenKlassName(Bklass));
	}

	type = Btype;
	klass = Bklass;
	name = Bname;
	dataType = BdataType;
	value = Bvalue;
	hash = Bhash;
}

//清空信息
void kkli::Token::clear() {
	if (OUTPUT_TOKEN_ACTIONS) {
		Debug::output(std::string("Token::clear(): ")
			+ "type = " + getTokenTypeName(type)
			+ "  klass = " + getTokenKlassName(klass));
	}

	type = 0;
	klass = 0;
	name = "";
	dataType = 0;
	value = 0;
	hash = 0;

	Btype = type;
	Bklass = klass;
	Bname = name;
	BdataType = dataType;
	Bvalue = value;
	Bhash = hash;
}

//获取TokenType类型名称
std::string kkli::Token::getTokenTypeName(int type) {
	const std::string TOKEN_TYPE_STRING[TOKEN_TYPE_SIZE] = {
		"ERR",
		"ID", 
		"NUM", 
		"CHAR", "ELSE", "ENUM", "IF", "INT", "RETURN", "SIZEOF", "WHILE",
		"ASSIGN", "COND", 
		"LOR", 
		"LAN", 
		"OR", 
		"XOR", 
		"AND", 
		"NOT", "EQ", "NE", "LT", "GT", "LE", "GE", 
		"SHL", "SHR", "ADD", "SUB", "MUL", "DIV", "MOD",
		"INC", "DEC",
		"LPAREN", "RPAREN", 
		"LBRACK", "RBRACK", 
		"LBRACE", "RBRACE",
		"COMMA", "COLON", "SEMICON", "TILDE",
		"STRING"
	};
	if (type == END) return "END";
	return TOKEN_TYPE_STRING[type];
}

//获取TokenKlass类型名称
std::string kkli::Token::getTokenKlassName(int klass) {
	std::string TOKEN_KLASS_STRING[TOKEN_KLASS_SIZE] = {
		"INVALID_TOKEN_KLASS", "NUMBER", "FUNC", "SYS_FUNC", "GLOBAL", "LOCAL"
	};
	return TOKEN_KLASS_STRING[klass];
}

//获取DataType名称
std::string kkli::Token::getDataTypeName(int type) {
	std::string result;
	while (type >= PTR_TYPE) {
		type -= 2;
		result += "PTR ";
	}
	if (type == CHAR_TYPE) result += "CHAR_TYPE";
	else result += "INT_TYPE";
	return result;
}
