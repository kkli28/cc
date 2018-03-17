#ifndef __UTILS_H__
#define __UTILS_H__

/*
文件说明：提供一些通用的工具（函数和类）
*/

#include "stdafx.h"

namespace kkli {

	//========================================
	// Error: 错误类，程序中异常信息都用这个
	//========================================
	class Error {
	private:
		std::string info;
	public:
		Error(std::string i) : info(i) {}
		std::string what()const { return "[Error] " + info; }
	};

	//========================================
	// Debug: 调试类，主要用于输出调试信息
	//========================================
	class Debug {
	private:
	public:
		static void output(const std::string& desc) {
			static std::ofstream outFile("output/debug.txt", std::ios::app);
			outFile << desc << std::endl;
		}
		static void output(const std::string& desc, int data) {
			static std::ofstream outFile("output/debug.txt", std::ios::app);
			outFile << desc << data << std::endl;
		}
		static void clear() {
			static std::ofstream outFile("output/debug.txt", std::ios::ate);
			outFile << std::endl;
		}
	};
}

#endif
