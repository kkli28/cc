#ifndef __STDAFX_H__
#define __STDAFX_H__

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

//是否启用Warning
#define ENABLE_WARNING false

//是否启用调试信息
#define DEBUG_MODE false

//VM调试信息
#define OUTPUT_VM_ACTIONS (DEBUG_MODE && false)
#define OUTPUT_VM_EXECUTE_ACTIONS (OUTPUT_VM_ACTIONS && false)

//Token调试信息
#define OUTPUT_TOKEN_ACTIONS (DEBUG_MODE && false)

//Symbol调试信息
#define OUTPUT_SYMBOL_TABLE_ACTIONS (DEBUG_MODE && false)

//Lexer调试信息
#define OUTPUT_LEXER_ACTIONS (DEBUG_MODE && false)
#define OUTPUT_LEXER_SOURCE_INFO (OUTPUT_LEXER_ACTIONS && false)
#define OUTPUT_LEXER_SYMBOL_INFO (OUTPUT_LEXER_ACTIONS && false)
#define OUTPUT_LEXER_NEXT_INFO (OUTPUT_LEXER_ACTIONS && false)

//Compiler调试信息
#define OUTPUT_COMPILER_ACTIONS (DEBUG_MODE && false)
#define OUTPUT_COMPILER_SYMBOL_INFO (OUTPUT_COMPILER_ACTIONS && false)

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
