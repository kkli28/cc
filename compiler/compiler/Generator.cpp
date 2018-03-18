#include "Generator.h"

//¹¹Ôìº¯Êý
kkli::Generator::Generator(std::string sourceFile)
	: lexer(sourceFile) {
	table = SymbolTable::getInstance();
	vm = VirtualMachine::getInstance();
}

void kkli::Generator::run() {

}