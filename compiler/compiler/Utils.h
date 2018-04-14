#ifndef __UTILS_H__
#define __UTILS_H__

/*
�ļ�˵�����ṩһЩͨ�õĹ��ߣ��������ࣩ
*/

#include "stdafx.h"

namespace kkli {

#define FORMAT(f) (f+"    ")
#define WARNING kkli::Warning::getInstance()

	//========================================
	// Error: �����࣬�������ɳ������쳣��Ϣ
	//========================================
	class Error {
	private:
		std::string info;  //������Ϣ

	public:
		Error(std::string i) :info(i) {}
		Error(int line, std::string i) {
			info = "Line: " + std::to_string(line) + ". " + i;
		}
		virtual std::string what() const { return "\n[Error] " + info + "\n"; }
	};

	//========================================
	// Warning: �����࣬���ڳ����е����Ͳ�ƥ�������
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
			std::cout << std::endl;
			for (auto str : warnings) {
				std::cout << str << std::endl;
			}
			std::cout << std::endl;
		}
	};

	//========================================
	// Debug: �����࣬��Ҫ�������������Ϣ
	//========================================
	class Debug {
	public:
		static void output(const std::string& desc, std::string format) {
			std::cout << format << desc << std::endl;
		}
		static void outputToFile(const std::string& desc, std::string format) {
			static std::ofstream outFile("file/debug.txt", std::ios::app);
			outFile << format << desc << std::endl;
		}
		static void clear() {
			static std::ofstream outFile("file/debug.txt", std::ios::ate);
			outFile << std::endl;
		}
	};
}

#endif
