#include "stdafx.h"
#include "Token.h"


//构造函数
kkli::Token::Token() : type(ID), klass(NUMBER), name(""), dataType(CHAR_TYPE), value(0), hash(0), 
Btype(ID), Bklass(NUMBER), Bname(""), BdataType(CHAR_TYPE), Bvalue(0), Bhash(0) {}

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

	type = TokenType::ID;
	klass = TokenKlass::NUMBER;
	name = "";
	dataType = INT_TYPE;
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
std::string kkli::Token::getTokenTypeName(TokenType type) {
	const std::string TOKEN_TYPE_STRING[TOKEN_TYPE_SIZE] = {
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
		"STRING",
		"END"
	};
	return TOKEN_TYPE_STRING[type];
}

//获取TokenKlass类型名称
std::string kkli::Token::getTokenKlassName(TokenKlass klass) {
	std::string TOKEN_KLASS_STRING[TOKEN_KLASS_SIZE] = {
		"NUMBER", "FUNC", "SYS_FUNC", "GLOBAL", "LOCAL"
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
