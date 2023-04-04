//
// Created by Иван Черемисенов on 04.04.2023.
//

#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

std::ostream& operator<<(std::ostream& out, std::list<int> list)
{
	for (auto& el : list)
	{
		out << el << " ";
	}
	out << std::endl;

	return out;
}

int main(int argc, char** argv)
{
	std::list<int> list;

	std::string inputline;
	std::getline(std::cin, inputline);
	std::istringstream ss(inputline);

	int n;
	while (ss >> n)
	{
		list.push_back(n);
	}

	if (argc < 2 || argc > 4)
	{
		std::cout << list;
		return 0;
	}

	std::string command(argv[1]);
	int arg1, arg2;

	try {
		if (argc > 2)
			arg1 = std::stoi(argv[2]);
		if (argc > 3)
			arg2 = std::stoi(argv[3]);
	} catch (std::invalid_argument & e) {
		std::cout << list;
		return 0;
	}

	if (command == "-af" && argc == 3)
		list.push_front(arg1);

	else if (command == "-ae" && argc == 3)
		list.push_back(arg1);

	else if (command == "-ib" && argc == 4)
	{
		auto pos = std::find(list.begin(), list.end(), arg2);
		if (pos != list.end())
			list.insert(pos, arg1);
	}

	else if (command == "-ia" && argc == 4)
	{
		auto pos = std::find(list.begin(), list.end(), arg2);
		if (pos != list.end())
			list.insert(++pos, arg1);
	}

	else if (command == "-d" && argc == 3)
	{
		auto pos = std::find(list.begin(), list.end(), arg1);
		if (pos != list.end())
			list.erase(pos);
	}

	else if (command == "-sa" && argc == 2)
	{
		list.sort();
	}

	else if (command == "-sd" && argc == 2)
	{
		list.sort(std::greater<int>());
	}

	else if (command == "-mf" && argc == 2)
	{
		auto max_pos = std::max_element(list.begin(), list.end());
		if (max_pos != list.end()) {
			int max = *max_pos;
			list.erase(max_pos);
			list.push_front(max);
		}
	}

	else if (command == "-me" && argc == 2)
	{
		auto max_pos = std::max_element(list.begin(), list.end());
		if (max_pos != list.end()) {
			int max = *max_pos;
			list.erase(max_pos);
			list.push_back(max);
		}
	}

	std::cout << list;

	return 0;
}