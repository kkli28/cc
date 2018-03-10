#ifndef Utils_h
#define Utils_h

/*
文件说明：提供一些通用的工具（函数和类）
*/

#include "stdafx.h"

namespace kkli {

	//========================================
	// Error: 错误类
	//========================================
	class Error {
	private:
		std::string info;
	public:
		Error(std::string i) : info(i) {}
		std::string what()const { return info; }
	};
}

#endif
