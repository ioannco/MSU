//
// Created by Ivan on 2/22/2022.
//

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <sstream>

// EXCEPTIONS ==========================================================================================================

class mstring_param_error : public std::logic_error {
 public:
	mstring_param_error(const std::string & method_name, int param1, int param2);
	mstring_param_error(const std::string & method_name, int param);
	explicit mstring_param_error(const std::string & what);
};

mstring_param_error::mstring_param_error(const std::string& method_name, int param1, int param2) :
	std::logic_error(std::string("Exception: problem in ") + method_name + " incorrect parameter:" +
					 std::to_string(param1) + " " + std::to_string(param2))
{

}

mstring_param_error::mstring_param_error(const std::string& method_name, int param) :
	std::logic_error(std::string("Exception: problem in ") + method_name + " incorrect parameter:" +
					 std::to_string(param))
{
}

mstring_param_error::mstring_param_error(const std::string& what) : logic_error(what)
{
}

// MSTRING =============================================================================================================

class mstring
{
public:
    mstring ();
    mstring (const mstring & other);
    mstring (const char * c_str);
    ~mstring ();

    size_t length () const;
    bool isempty () const;
    void add (char c);
    void add (const char * c);
    void insert (char c, int pos);
    void insert (const char * c_str, int pos);

    void del (int i);
    void del (int begin, int end);
    int search (const char * str) const;
    void replace (const char * sub_str, const char * new_str);
    void print () const;

	mstring & operator = (const mstring & other);

	mstring operator + (const mstring & other) const;
	mstring operator + (const char * other) const;
	friend mstring operator + (const char * left, const mstring & other);

	mstring operator * (int multiplier) const;
	friend mstring operator*(int multiplier, const mstring & other);

	bool operator > (const mstring & other) const;
	bool operator < (const mstring & other) const;
	bool operator >= (const mstring & other) const;
	bool operator <= (const mstring & other) const;
	bool operator == (const mstring & other) const;

	char & operator [] (int index);

	friend std::ostream & operator << (std::ostream & left, const mstring & right);
	friend std::istream & operator >> (std::istream & left, mstring & right);


private:
    void realloc (size_t bytes);
    void reserve (size_t bytes);

    char * m_str;      // actual string with \0
    size_t m_reserved, // counts bytes reserved for string
    m_length;   // actual length, not strlen

};

mstring::mstring () :
        m_str (new char [1]),
        m_reserved (1),
        m_length (1)
{
    m_str[0] = '\0';
}

mstring::mstring (const mstring & other) :
        m_str (new char [other.m_reserved]),
        m_reserved (other.m_reserved),
        m_length (other.m_length)
{
    if (m_reserved)
        std::memcpy (m_str, other.m_str, other.m_length);
}

mstring::mstring (const char * c_str) :
        m_str (new char [strlen (c_str) + 1]),
        m_reserved (strlen (c_str) + 1),
        m_length (m_reserved)
{
    std::memcpy (m_str, c_str, m_length);
}

void mstring::insert (char c, int pos)
{
    if (!m_str)
        return;

	if (pos < 0 || pos > length())
		throw mstring_param_error("insert", pos);

    if (m_reserved <= m_length + 1)
        mstring::realloc (static_cast<size_t> (static_cast<double> (m_length + 1) * 1.5 + 1));

    std::memcpy (m_str + pos + 1, m_str + pos, m_length - pos);

    m_str[pos] = c;

    m_length++;
}

void mstring::insert (const char * c_str, int pos)
{
    if (!m_str)
        return;

	if (pos < 0 || pos > length())
		throw mstring_param_error("insert", pos);

    size_t c_str_len = strlen (c_str);

    if (m_reserved <= m_length + c_str_len)
        mstring::realloc (static_cast<size_t> (static_cast<double> (m_length + c_str_len) * 1.5 + 1));

    std::memcpy (m_str + pos + c_str_len, m_str + pos, m_length - pos);
    std::memcpy (m_str + pos, c_str, c_str_len);

    m_length += c_str_len;
}


std::ostream & operator<< (std::ostream &left, const mstring & right)
{
    if (!right.m_str)
        return left;

    left << right.m_str;
    return left;
}

std::istream & operator>> (std::istream & left, mstring & right) {
	right = mstring();

	int input = 0;
	while ((input = std::cin.get()) != '\n' && input != EOF) {
		right.insert(static_cast<char>(input), right.length());
	}
	return left;
}

void mstring::print () const
{
    std::cout << *this << std::endl;
}

mstring::~mstring ()
{
    delete[] m_str;
}

void mstring::reserve (size_t bytes)
{
    if (bytes <= m_reserved)
        return;

    mstring::realloc (bytes);
}

size_t mstring::length () const
{
    return m_length - 1;
}

bool mstring::isempty () const
{
    return m_length == 1;
}

void mstring::realloc (size_t bytes)
{
    if (!m_str)
        return;

    char * new_str = new char [bytes];
    std::memcpy (new_str, m_str, bytes < m_reserved ? bytes : m_reserved);

    if (bytes < m_reserved)
        new_str[bytes - 1] = '\0';

    delete[] m_str;
    m_str = new_str;
    m_reserved = bytes;
}

void mstring::add (char c)
{
    insert (c, static_cast<int>(m_length) - 1);
}

void mstring::add (const char * c_str)
{
    insert (c_str, static_cast<int>(m_length) - 1);
}

void mstring::del (int i)
{
    if (!m_str)
        return;

	if (i < 0 || i >= length())
		throw mstring_param_error("del", i);

    std::memcpy (m_str + i, m_str + i + 1, m_length - i - 1);
    m_length--;

    if (m_reserved > 2 * (m_length))
        mstring::realloc (static_cast<size_t> (static_cast<double> (m_length) * 1.5 + 1));
}

void mstring::del (int begin, int end)
{
    if (!m_str)
        return;

	if (begin < 0 || begin > end || end >= length())
		throw mstring_param_error("del", begin, end);

    std::memcpy (m_str + begin, m_str + end + 1, m_length - (end - begin + 1));
    m_length -= end - begin + 1;

    if (m_reserved > 2 * (m_length))
        mstring::realloc (static_cast<size_t> (static_cast<double> (m_length) * 1.5 + 1));
}

int mstring::search (const char *str) const
{
    const char * substr = std::strstr (m_str, str);

    if (substr)
        return static_cast<int> (substr - m_str);

    return -1;
}

void mstring::replace (const char *sub_str, const char *new_str)
{
    int begin = search (sub_str);

    if (begin < 0)
        return;

    size_t end = begin + strlen (sub_str) - 1;

    del (begin, end);
    insert (new_str, begin);
}

mstring mstring::operator+(const mstring& other) const
{
	mstring temp(*this);
	temp.insert(other.m_str, temp.m_length - 1);
	return temp;
}

mstring mstring::operator*(int multiplier) const
{
	if (multiplier < 0)
		throw mstring_param_error("operator*", multiplier);

	if (multiplier == 0)
		return mstring();

	mstring temp(*this);
	temp.reserve(temp.length() * multiplier + 1);
	for (int i = 1; i < multiplier; i++) {
		temp = temp + *this;
	}
	return temp;
}
mstring & mstring::operator=(const mstring& other)
{
	if (&other == this)
		return *this;

	if (m_reserved < other.m_length)
		reserve(other.m_length);

	std::memcpy(m_str, other.m_str, other.m_length);
	m_length = other.m_length;
	return *this;
}

bool mstring::operator==(const mstring& other) const
{
	if (&other == this)
		return true;

	return m_length == other.m_length && strcmp(m_str, other.m_str) == 0;
}

mstring operator * (int multiplier, const mstring & other){
	return other * multiplier;
}

bool mstring::operator>(const mstring& other) const
{
	return strcmp(this->m_str, other.m_str) > 0;
}

bool mstring::operator<(const mstring& other) const
{
	return strcmp(this->m_str, other.m_str) < 0;
}

bool mstring::operator>=(const mstring& other) const
{
	return !(*this < other);
}

bool mstring::operator<=(const mstring& other) const
{
	return !(*this > other);
}

char& mstring::operator[](int index)
{
	if (index < 0 || index >= length())
		throw mstring_param_error("operator[]", index);

	return m_str[index];
}

mstring mstring::operator+(const char* other) const
{
	return *this + mstring(other);
}

mstring operator+(const char* left, const mstring& other)
{
	return mstring(left) + other;
}


int main() {
	try {
		error();
	} catch (std::exception & err) {
		std::cerr << err.what() << std::endl;
	}
}
