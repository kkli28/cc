#ifndef Utils_h
#define Utils_h

/*
�ļ�˵�����ṩһЩͨ�õĹ��ߣ��������ࣩ
*/

#include "stdafx.h"

namespace kkli {

	//========================================
	// Error: ������
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
