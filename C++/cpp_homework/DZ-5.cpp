//
// Created by Иван Черемисенов on 22.03.2023.
//

#include <iostream>
#include <array>
#include <sstream>

// =====================================================================================================================
// Figures
// =====================================================================================================================

class Figure {
 public:
	Figure(int x, int y);
	virtual ~Figure() = default;
	virtual bool move(int x, int y) const = 0;

	virtual int get_x() const;
	virtual int get_y() const;

 private:
	int m_x, m_y;
};

Figure::Figure(int x, int y) :
	m_x(x),
	m_y(y)
{

}

int Figure::get_x() const {
	return m_x;
}

int Figure::get_y() const
{
	return m_y;
}

// BISHOP ==============================================================================================================

class Bishop: public Figure {
 public:
	Bishop(int x, int y);
	virtual bool move(int x, int y) const override;
};

bool Bishop::move(int x, int y) const {
	return !(x == get_x() && y == get_y()) && abs(x - get_x()) == abs(y - get_y());
}

Bishop::Bishop(int x, int y) : Figure(x, y) {

}

// ROOK ================================================================================================================

class Rook: public Figure {
 public:
	Rook(int x, int y);
	virtual bool move(int x, int y) const override;
};

bool Rook::move(int x, int y) const {
	return !(x == get_x() && y == get_y()) && (x == get_x() || y == get_y());
}

Rook::Rook(int x, int y) : Figure(x, y) {

}

// QUEEN ===============================================================================================================

class Queen: public Rook, public Bishop {
 public:
	Queen(int x, int y);
	virtual bool move(int x, int y) const override;

	int get_x() const override;
	int get_y() const override;
};

Queen::Queen(int x, int y) : Rook(x, y), Bishop(x, y) {

}

bool Queen::move(int x, int y) const {
	return Rook::move(x, y) || Bishop::move(x, y);
}

int Queen::get_x() const
{
	return Rook::get_x();
}

int Queen::get_y() const
{
	return Rook::get_y();
}


// KING ================================================================================================================

class King: public Queen {
 public:
	King(int x, int y);
	virtual bool move(int x, int y) const override;
};

King::King(int x, int y) : Queen(x, y) {

}

bool King::move(int x, int y) const {
	return Queen::move(x, y) && abs(x - get_x()) <= 1 && abs(y - get_y()) <= 1;
}

// KNIGHT ==============================================================================================================

class Knight: public Queen {
 public:
	Knight(int x, int y);
	virtual bool move(int x, int y) const override;
};

Knight::Knight(int x, int y) : Queen(x, y) {

}

bool Knight::move(int x, int y) const {
	return !(x == get_x() && y == get_y()) && !Queen::move(x, y) && abs(x - get_x()) <= 2 && abs(y - get_y()) <= 2;
}

// =====================================================================================================================
// Parser
// =====================================================================================================================

class Parser {
 public:
	explicit Parser(const std::string & input);

	int get_start_x() const;
	int get_start_y() const;
	int get_end_x() const;
	int get_end_y() const;
	char get_type() const;

 private:
	static void check_bounds(int coord);
	
	int m_start_x, m_start_y;
	int m_end_x, m_end_y;
	char m_type;
};

Parser::Parser(const std::string& input) :
	m_start_x(-1),
	m_start_y(-1),
	m_end_x(-1),
	m_end_y(-1),
	m_type(0)
{
	std::stringstream ss (input);
	ss >> m_type;

	std::string start, end;
	ss >> start;
	ss >> end;

	if (start.length() != 2 || end.length() != 2)
		throw std::invalid_argument("Invalid input");

	m_start_x = start[0] - 'a';
	m_start_y = start[1] - '1';

	m_end_x = end[0] - 'a';
	m_end_y = end[1] - '1';

	check_bounds(m_start_x);
	check_bounds(m_start_y);
	check_bounds(m_end_x);
	check_bounds(m_end_y);
}

void Parser::check_bounds(int coord) {
	if (coord < 0 || coord > 7)
		throw std::out_of_range("Figure coordinate out of bounds");
}

int Parser::get_start_x() const {
	return m_start_x;
}

int Parser::get_start_y() const {
	return m_start_y;
}

int Parser::get_end_x() const {
	return m_end_x;
}

int Parser::get_end_y() const {
	return m_end_y;
}

char Parser::get_type() const {
	return m_type;
}

// =====================================================================================================================
// ChessChecker
// =====================================================================================================================

class ChessChecker {
 public:
	ChessChecker();
	~ChessChecker();
	bool check();
	
 private:
	void assign_figure(char type, int x, int y);
	void print_field() const;
	
	Figure * m_figure;
};

bool ChessChecker::check() {
	std::string input;
	std::getline(std::cin, input);
	
	try {
		Parser parser(input);
		assign_figure(parser.get_type(), parser.get_start_x(), parser.get_start_y());
		return m_figure->move(parser.get_end_x(), parser.get_end_y());
		
	} catch (std::logic_error & e) {
		return false;
	}
}

ChessChecker::ChessChecker() :
	m_figure(nullptr) {
}

ChessChecker::~ChessChecker() {
	delete m_figure;
}

void ChessChecker::assign_figure(char type, int x, int y) {
	switch (type) {
	case 'K':
		m_figure = static_cast<Rook*>(new King(x, y));
		break;

	case 'Q':
		m_figure = static_cast<Rook*>(new Queen(x, y));
		break;

	case 'R':
		m_figure = new Rook(x, y);
		break;

	case 'B':
		m_figure = new Bishop(x, y);
		break;

	case 'N':
		m_figure = static_cast<Rook*>(new Knight(x, y));
		break;

	default:
		throw std::invalid_argument("Invalid figure code");
	}
}

void ChessChecker::print_field() const {
	std::cout << " abcdefgh" << std::endl;
	for (int x = 0; x < 8; x++) {
		std::cout << x + 1;
		for (int y = 0; y < 8; y++) {
			if (m_figure->move(x, y))
				std::cout << '#';
			else
				std::cout << ' ';
		}
		std::cout << x + 1;
		std::cout << std::endl;
	}
	std::cout << " abcdefgh" << std::endl;
}

// =====================================================================================================================
// Main
// =====================================================================================================================

int main() {
	ChessChecker cc;
	if (cc.check())
		std::cout << "YES" << std::endl;
	else
		std::cout << "NO" << std::endl;

	return 0;
}