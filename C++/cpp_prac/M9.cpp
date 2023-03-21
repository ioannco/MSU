//
// Created by Иван Черемисенов on 21.03.2023.
//

#include <iostream>
#include <cstring>
#include <stdexcept>

template <typename T>
T maxn (T * arr, int size) {
	if (size <= 0)
		throw std::length_error("Length error");

	T & max = arr[0];

	for (int i = 1; i < size; i++) {
		if (arr[i] > max)
			max = arr[i];
	}

	return max;
}

const char * maxn(const char ** arr, int size) {
	if (size <= 0)
		return nullptr;

	const char * max = arr[0];

	for (int i = 1; i < size; i++) {
		if (strlen(max) < strlen(arr[i]))
			max = arr[i];
	}

	return max;
}
