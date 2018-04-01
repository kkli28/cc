#ifndef __UTILS_H__
#define __UTILS_H__

/*
文件说明：提供一些通用的工具（函数和类）
*/

#include "stdafx.h"

namespace kkli {

	//========================================
	// Error: 错误类，用于生成程序中异常信息
	//========================================
	class Error {
	private:
		std::string info;
	public:
		Error(std::string i) : info(i) {}
		Error(int line, std::string i) {
			info = "Line: " + std::to_string(line) + ". " + i;
		}
		std::string what()const { return "[Error] " + info; }
	};

	//========================================
	// NewError: 新的错误类，支持延后报错，将取代旧的错误类。
	//========================================
	class NewError {
	private:
		std::vector<std::string> errors;
		int errorCount;
		int maxErrorCount;
		NewError(int maxErr) : maxErrorCount(maxErr) {};
	public:
		static NewError* getInstance() {
			static NewError* error = nullptr;
			if (!error) {
				error = new NewError(3);
			}
			return error;
		}
		void add(int line, std::string info) {
			errors.push_back("[Error] Line " + std::to_string(line) + ": " + info);
			++errorCount;
			if (errorCount >= maxErrorCount) {
				output();
				clear();
				throw 1;
			}
		}
		void output() const {
			for (auto err : errors) {
				std::cout << err << std::endl;
			}
		}
		void clear() { errors.clear();  }
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
	public:
		static void output(const std::string& desc) {
			static std::ofstream outFile("file/debug.txt", std::ios::app);
			outFile << desc << std::endl;
		}
		static void output(const std::string& desc, int data) {
			static std::ofstream outFile("file/debug.txt", std::ios::app);
			outFile << desc << data << std::endl;
		}
		static void clear() {
			static std::ofstream outFile("file/debug.txt", std::ios::ate);
			outFile << std::endl;
		}
	};
}

#endif
