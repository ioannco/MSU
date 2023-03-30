//
// Created by Иван Черемисенов on 28.03.2023.
//

#include <exception>
#include <iostream>
#include <list>
#include <algorithm>

class PlayerException : public std::invalid_argument {
 public:
	PlayerException(const std::string & what, const std::string & team, int number);
	PlayerException(const std::string & what, const std::string & team);
};

PlayerException::PlayerException(const std::string & what, const std::string & team, int number)
	: invalid_argument(std::string("Exception: ") + what + " in team " + team + " Error number: " +
		std::to_string(number))
{

}

PlayerException::PlayerException(const std::string& what, const std::string& team)
	: invalid_argument(std::string("Exception: ") + what + " in team " + team)
{

}

class Player{
 public:
	Player(const std::string & team, int number);
	~Player();

	void Print() const;
	static void Print_teams();

 private:
	static std::list<int> zenit;
	static std::list<int> cska;

	int m_number;
	std::string m_team;
};

std::list<int> Player::zenit = std::list<int>();
std::list<int> Player::cska = std::list<int>();

Player::Player(const std::string& team, int number) :
	m_number(number),
	m_team(team)
{
	if (team == "Zenit") {
		if (number < 1)
			throw PlayerException("Wrong number", team, number);

		if (zenit.size() == 11)
			throw PlayerException("Too many players", team);

		if (std::find(zenit.begin(), zenit.end(), number) != zenit.end())
			throw PlayerException("Double number", team, number);

		zenit.push_back(number);
	}

	else if (team == "CSKA") {
		if (number < 1)
			throw PlayerException("Wrong number", team, number);

		if (cska.size() == 11)
			throw PlayerException("Too many players", team);

		if (std::find(cska.begin(), cska.end(), number) != cska.end())
			throw PlayerException("Double number", team, number);

		cska.push_back(number);
	}

	else
		throw PlayerException("Wrong name", team);
}


Player::~Player()
{
	if (m_team == "Zenit")
		zenit.erase(std::find(zenit.begin(), zenit.end(), m_number));
	else
		cska.erase(std::find(cska.begin(), cska.end(), m_number));
}

void Player::Print() const
{
	std::cout << m_team << " " << m_number << std::endl;
}

void Player::Print_teams()
{
	if (!zenit.empty()) {
		std::cout << "Zenit: ";
		for (auto & el: zenit) {
			std::cout << el << " ";
		}
		std::cout << std::endl;
	}

	if (!cska.empty()) {
		std::cout << "CSKA: ";
		for (auto & el: cska) {
			std::cout << el << " ";
		}
		std::cout << std::endl;
	}
}


int main () {
	try {
		test_players();
	} catch (PlayerException & e) {
		std::cerr << e.what() << std::endl;
	}
}