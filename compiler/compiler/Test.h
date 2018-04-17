#ifndef __TEST_H__
#define __TEST_H__

#include "stdafx.h"
#include "Compiler.h"

namespace kkli {

	//========================================
	// ≤‚ ‘¿‡
	//========================================
	class Test {
	private:
		std::vector<std::string> testFiles;

		Test() {
			testFiles.push_back("test_case/array.c");
			testFiles.push_back("test_case/enum.c");
			testFiles.push_back("test_case/func_param.c");
			testFiles.push_back("test_case/global_variable.c");
			testFiles.push_back("test_case/hanoi.c");
			testFiles.push_back("test_case/local_variable.c");
			testFiles.push_back("test_case/sort.c");
		}

	public:
		static Test* getInstance() {
			static Test* test = nullptr;
			if (!test) {
				test = new Test();
			}
			return test;
		}

		void run() const {
			for (auto file : testFiles) {
				Compiler compiler(file);
				compiler.run();
			}
		}
	};
}

#endif
