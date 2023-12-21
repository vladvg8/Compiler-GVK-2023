#include <iostream>

extern "C" {
	int __stdcall Sum(int firstOperand, int secondOperand) {
		return firstOperand + secondOperand;
	}
	int __stdcall Pow(int operand, int power) {
		if (power == 0) {
			return 1;
		}
		if (power < 0) {
			return 0;
		}
		int result = 1;
		for (int i = 0; i < power; i++) {
			result *= operand;
		}
		return result;
	}

	void __stdcall writestr(char* str) {
		std::cout << str << std::endl;
	}

	void __stdcall writechar(char ch) {
		std::cout << ch << std::endl;
	}

	void __stdcall writeint(int n) {
		std::cout << n << std::endl;
	}

	void __stdcall writebool(bool b) {
		if (b == false) {
			std::cout << "false" << std::endl;
		}
		else {
			std::cout << "true" << std::endl;
		}
	}
}