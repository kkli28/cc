#ifndef __STDAFX_H__
#define __STDAFX_H__

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

//是否启用Warning
#define ENABLE_WARNING true

//是否开启输出执行的语句
#define ENABLE_OUTPUT_EXECUTE_INSTRUCTION false

//是否启用调试信息
#define DEBUG_MODE false

//VM调试信息
#define OUTPUT_VM_ACTIONS (DEBUG_MODE && true)
#define OUTPUT_VM_EXECUTE_ACTIONS (OUTPUT_VM_ACTIONS && true)

//Token调试信息
#define OUTPUT_TOKEN_ACTIONS (DEBUG_MODE && true)

//Symbol调试信息
#define OUTPUT_SYMBOL_TABLE_ACTIONS (DEBUG_MODE && true)

//Lexer调试信息
#define OUTPUT_LEXER_ACTIONS (DEBUG_MODE && true)
#define OUTPUT_LEXER_SOURCE_INFO (OUTPUT_LEXER_ACTIONS && true)
#define OUTPUT_LEXER_SYMBOL_INFO (OUTPUT_LEXER_ACTIONS && true)
#define OUTPUT_LEXER_NEXT_INFO (OUTPUT_LEXER_ACTIONS && true)

//Compiler调试信息
#define OUTPUT_COMPILER_ACTIONS (DEBUG_MODE && true)
#define OUTPUT_COMPILER_SYMBOL_INFO (OUTPUT_COMPILER_ACTIONS && true)

//简化调试语句
#define DEBUG_VM(arg1, arg2) if(OUTPUT_VM_ACTIONS){ Debug::output(arg1, arg2); }
#define DEBUG_VM_EXECUTE(arg1, arg2) if(OUTPUT_VM_EXECUTE_ACTIONS) { Debug::output(arg1, arg2); }
#define DEBUG_TOKEN(arg1, arg2) if(OUTPUT_TOKEN_ACTIONS) { Debug::output(arg1, arg2); }
#define DEBUG_SYMBOL_TABLE(arg1, arg2) if(OUTPUT_SYMBOL_TABLE_ACTIONS) { Debug::output(arg1, arg2); }
#define DEBUG_LEXER(arg1, arg2) if(OUTPUT_LEXER_ACTIONS) { Debug::output(arg1, arg2); }
#define DEBUG_LEXER_SOURCE(arg1, arg2) if(OUTPUT_LEXER_SOURCE_INFO) { Debug::output(arg1, arg2); }
#define DEBUG_LEXER_SYMBOL_TABLE(arg1, arg2) if(OUTPUT_LEXER_SYMBOL_INFO) { Debug::output(arg1, arg2); }
#define DEBUG_LEXER_NEXT(arg1, arg2) if(OUTPUT_LEXER_NEXT_INFO) { Debug::output(arg1, arg2); }
#define DEBUG_COMPILER(arg1, arg2) if(OUTPUT_COMPILER_ACTIONS) { Debug::output(arg1, arg2); }
#define DEBUG_COMPILER_SYMBOL(arg1, arg2) if(OUTPUT_COMPILER_SYMBOL_INFO) { Debug::output(arg1, arg2); }

//标准库头文件
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//普通头文件
#include "Utils.h"

#endif
