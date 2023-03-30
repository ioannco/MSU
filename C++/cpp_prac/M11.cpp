//
// Created by Иван Черемисенов on 28.03.2023.
//

#include <string>
#include <iostream>
#include <vector>

class Flower {
 public:
	explicit Flower(const std::string & color = "", int n = 10);
	~Flower();

	static void Print_flowers();


 private:
	static int pink;
	static int white;
	bool is_white;
};

Flower::Flower(const std::string& color, int n) :
 	is_white(false)
{
	if (color == "pink")
		is_white = false;
	else if (color == "white")
		is_white = true;
	else if (white == pink)
		is_white = n % 2;
	else
		is_white = white < pink;

	if (is_white)
		white++;
	else
		pink++;
}

int Flower::pink = 0;
int Flower::white = 0;


void Flower::Print_flowers()
{
	std::cout << "White: " << white << " Pink: " << pink << std::endl;
}


Flower::~Flower()
{
	if (is_white)
		white--;
	else
		pink--;
}

