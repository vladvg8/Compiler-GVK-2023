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
		return operand;
	}
}