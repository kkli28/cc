#ifndef __UTILS_H__
#define __UTILS_H__

/*
文件说明：提供一些通用的工具（函数和类）
*/

#include "stdafx.h"

namespace kkli {

#define FORMAT(f) (f+"    ")
#define WARNING kkli::Warning::getInstance()

	//========================================
	// Error: 错误类，用于生成程序中异常信息
	//========================================
	class Error {
	private:
		std::string info;  //错误信息

	public:
		Error(std::string i) :info(i) {}
		Error(int line, std::string i) {
			info = "Line: " + std::to_string(line) + ". " + i;
		}
		virtual std::string what() const { return "\n[Error] " + info + "\n"; }
	};

	//========================================
	// Warning: 警告类，用于程序中的类型不匹配等问题
	//========================================
	class Warning {
	private:
		std::vector<std::string> warnings;
		Warning() {}

	public:
		static Warning* getInstance() {
			static Warning* warning = nullptr;
			if (!warning) {
				warning = new Warning();
			}
			return warning;
		}
		void clear() { warnings.clear(); }
		void add(int line, std::string info) {
			std::string str = "[Warning] Line " + std::to_string(line) + ": " + info;
			warnings.push_back(std::move(str));
		}

		void output()const {
			if (warnings.empty()) return;  //避免多余的std::endl
			std::cout << std::endl;
			for (auto str : warnings) {
				std::cout << str << std::endl;
			}
		}
	};

	//========================================
	// Debug: 调试类，主要用于输出调试信息
	//========================================
	class Debug {
	private:
		static void outputToFile(const std::string& desc, std::string format) {
			static std::ofstream outFile("debug.txt", std::ios::app);
			outFile << format << desc << std::endl;
		}
	public:
		static bool OUTPUT_TO_CONSOLE;  //是否输出到控制台
		static void output(const std::string& desc, std::string format) {
			if (OUTPUT_TO_CONSOLE) {
				std::cout << format << desc << std::endl;
			}
			else {
				outputToFile(desc, format);
			}
		}
		
		static void clear() {
			static std::ofstream outFile("debug.txt", std::ios::ate);
			outFile << std::endl;
		}
	};
}

#endif
