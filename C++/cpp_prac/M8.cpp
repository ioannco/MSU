//
// Created by Иван Черемисенов on 14.03.2023.
//

#include <iostream>
#include <string>

class event {
 public:
	event();
	event(const std::string & name, int year, int month, int day);
	virtual void print_res() const;
	virtual void change_grade(int grade) = 0;
	void change_date(int year, int month, int day);

 protected:
	std::string name_;
	int year_, month_, day_;
};

event::event() :
	year_(0),
	month_(0),
	day_(0)
{

}

event::event(const std::string & name, int year, int month, int day) :
	name_(name),
	year_(year),
	month_(month),
	day_(day)
{

}

void event::change_date(int year, int month, int day)
{
	year_ = year;
	month_ = month;
	day_ = day;
}

void event::print_res() const
{
	std::cout << name_ << " " << year_ << " " << month_ << " " << day_;
}

class test : public event {
 public:
	test();
	test(const std::string & name, int year, int month, int day, bool grade);

	virtual void change_grade(int grade);
	virtual void print_res() const override;

  private:
	 bool grade_;
 };

test::test() :
	event(),
	grade_(false)
{

}

test::test(const std::string & name, int year, int month, int day, bool grade) :
	event(name, year, month, day),
	grade_(grade)
{

}

void test::change_grade(int grade)
{
	grade_ = grade;
}

void test::print_res() const
{
	event::print_res();
	std::cout << " " << (grade_ ? "true" : "false") << std::endl;
}

class exam : public event {
 public:
	exam();
	exam(const std::string & name, int year, int month, int day, int grade);
	virtual void change_grade(int grade) override;
	virtual void print_res() const override;

 private:
	int grade_;
};

exam::exam() :
	event(),
	grade_()
{

}

exam::exam(const std::string& name, int year, int month, int day, int grade) :
	event(name, year, month, day),
	grade_(grade)
{

}

void exam::change_grade(int grade)
{
	grade_ = grade;
}

void exam::print_res() const
{
	event::print_res();
	std::cout << " Grade " << grade_ << std::endl;
}
