//
// Created by Иван Черемисенов on 14.02.2023.
//

#include <cstring>
#include <string>
#include <iostream>
#include <cmath>

const char * right(const char* str, int N) {
	if (N < 0 || !str)
		return nullptr;
	if (N >= strlen(str))
		return str;
	return str + (strlen(str) - N);
}

int digit_count(unsigned long arg) {
	if (!arg)
		return 1;

	int i = 0;

	for (i; arg; i++)
		arg /= 10;

	return i;
}

unsigned long pow10(int N) {
	unsigned long pow = 1;
	for (int i = 0; i < N; i++)
		pow = pow * 10;
	return pow;
}

unsigned long right(unsigned long num, int N) {
	if (N < 0)
		return 0;

	int num_size = digit_count(num);
	if (N >= num_size)
		return num;

	unsigned long power = pow10(N);

	return num - (num / power) * power;
}

int main() {
	char buff[128];
	std::cin.getline(buff, 128);

	if (!strcmp(buff, "string")) {
		int N = 0;

		std::cin.getline(buff, 128);
		std::cin >> N;
		std::cout << right(buff, N) << std::endl;
	} else if (!strcmp(buff, "number")) {
		unsigned long num = 0;
		int N = 0;

		std::cin >> num >> N;

		std::cout << right(num, N) << std::endl;
	}

	return 0;
}