#ifndef Utils_h
#define Utils_h

/*
�ļ�˵�����ṩһЩͨ�õĹ��ߣ��������ࣩ
*/

#include "stdafx.h"

namespace kkli {

	//========================================
	// Error: �����࣬�������쳣��Ϣ�������
	//========================================
	class Error {
	private:
		std::string info;
	public:
		Error(std::string i) : info(i) {}
		std::string what()const { return info; }
	};

	//========================================
	// Debug: �����࣬��Ҫ�������������Ϣ
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
	};
}

#endif
