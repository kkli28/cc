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
		std::vector<std::pair<std::string, std::string>> testFiles;

		Test() {
			testFiles.push_back({ "test_case/array.c", "array" });
			testFiles.push_back({ "test_case/enum.c", "enum" });
			testFiles.push_back({ "test_case/func_param.c", "func_param" });
			testFiles.push_back({ "test_case/global_variable.c", "global_variable" });
			testFiles.push_back({ "test_case/hanoi.c", "hanoi" });
			testFiles.push_back({ "test_case/local_variable.c", "local_variable" });
			testFiles.push_back({ "test_case/sort.c", "sort" });
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
				std::cout << "\n======== test: " << file.second << " ========\n" << std::endl;
				Compiler compiler(file.first);
				compiler.run();
			}
		}
	};
}

#endif
