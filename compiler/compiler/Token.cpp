#include "stdafx.h"
#include "Token.h"

//构造函数
kkli::Token::Token() : type(0), name(""), hash(0), klass(0), dataType(0), value(0) {}

//添加函数参数类型
void kkli::Token::addArgument(int dataType, std::string format) {
	DEBUG_TOKEN("Token::addArgument(" + Token::getDataTypeName(dataType) + ")", format);
	if (klass != FUNC) {
		throw Error("Token::addArgument(" + getDataTypeName(dataType) + ": " + name + " this is not a function.");
	}
	argsDataType.push_back(dataType);
}

//获取TokenType类型名称
std::string kkli::Token::getTokenTypeName(int type) {

	//为避免以后对标记类型的更改，这里暴力列举
	switch (type) {
	case ERROR: return "ERROR";
	case LPAREN: return "LPAREN";
	case RPAREN: return "RPAREN";
	case RBRACK: return "RBRACK";
	case LBRACE: return "LBRACE";
	case RBRACE: return "RBRACE";
	case COMMA: return "COMMA";
	case COLON: return "COLON";
	case SEMICON: return "SEMICON";
	case TILDE: return "TILDE";
	case ID: return "ID";
	case NUM_CHAR: return "NUM_CHAR";
	case NUM_INT: return "NUM_INT";
	case CHAR: return "CHAR";
	case ELSE: return "ELSE";
	case ENUM: return "ENUM";
	case IF: return "IF";
	case INT: return "INT";
	case RETURN: return "RETURN";
	case SIZEOF:return "SIZEOF";
	case WHILE: return "WHILE";
	case ASSIGN: return "ASSIGN";
	case COND: return "COND";
	case LOR: return "LOR";
	case LAN: return "LAN";
	case OR: return "OR";
	case XOR: return "XOR";
	case AND: return "AND";
	case NOT: return "NOT";
	case EQ: return "EQ";
	case NE: return "NE";
	case LT: return "LT";
	case GT: return "GT";
	case LE: return "LE";
	case GE: return "GE";
	case SHL: return "SHL";
	case SHR: return "SHR";
	case ADD:return "ADD";
	case SUB: return "SUB";
	case MUL: return "MUL";
	case DIV: return "DIV";
	case MOD: return "MOD";
	case INC: return "INC";
	case DEC: return "DEC";
	case LBRACK: return "LBRACK";
	case STRING: return "STRING";
	case END: return "END";
	default:
		throw Error("Token::getTokenTypeName()");
	}
}

//获取TokenKlass类型名称
std::string kkli::Token::getTokenKlassName(int klass) {
	switch (klass) {
	case ERROR: return "ERROR";
	case NUMBER: return "NUMBER";
	case FUNC: return "FUNC";
	case SYS_FUNC: return "SYS_FUNC";
	case GLOBAL_VARIABLE: return "GLOBAL_VARIABLE";
	case LOCAL_VARIABLE: return "LOCAL_VARIABLE";
	default:
		throw Error("Token::getTokenKlassName(" + std::to_string(klass) + ")");
	}
}

//获取DataType名称
std::string kkli::Token::getDataTypeName(int type) {
	std::string result = (type == CHAR_TYPE ? "char" : "int");
	while (type >= PTR_TYPE) {
		type -= 2;
		result += " ptr";
	}
	return result;
}
