//
// Created by Иван Черемисенов on 21.03.2023.
//

#include <iostream>
#include <cstring>

template <typename T>
bool compare (T first, T second) {

}

template <typename T>
T maxn (T * arr, int size) {
	if (size <= 0)
		return T();

	T max = arr[0];

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
		if (strcmp(arr[i], max) < 0)
			max = arr[i];
	}

	return max;
}

int main()
{
	using namespace std;
	int arri[6] = {1, 2, 5, 3, 1, -1};
	double arrd[4] = {2.3, -1.3, 8.7, 4.5};
	const char * arrc[5] = {"first", "second", "third", "long string", "another string"};

	cout << "max of integer = " << maxn(arri, 6) << endl;
	cout << "max of double = " << maxn(arrd, 4) << endl;
	cout << "max string is " << maxn(arrc, 5) << endl;

	return 0;
}