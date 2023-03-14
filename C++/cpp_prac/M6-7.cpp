//
// Created by Ivan on 2/22/2022.
//

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <sstream>

class vec_length_error: public std::length_error {
 public:
	explicit vec_length_error(const std::string & error);
};


class vec_index_error: public std::out_of_range {
 public:
	explicit vec_index_error(const std::string & error);
};

class Vec
{
public:
    explicit  Vec (int len, double * initializer_list = nullptr);
    Vec (const Vec & other);
    ~Vec ();

    void set (double arg, int coord);
    double get (int coord) const;
    double euc_norm () const;
    double max_norm () const;
    void print () const;

    Vec & operator = (const Vec & other);
    const Vec operator + (const Vec & other) const;
    const Vec operator - (const Vec & other) const;
    bool operator == (const Vec & other) const;
    double & operator [] (int index);
    double operator [] (int index) const;
    const Vec operator * (double right) const;

    friend std::ostream & operator << (std::ostream & left, const Vec & right);
    friend const Vec operator * (double left, const Vec & right);

private:
    void resize (size_t size);

    double * m_data;
    size_t m_size;
};

Vec::Vec (int len, double * initializer_list)
{
	if (len < 0)
		throw vec_length_error("Exception: length error");

	m_size = len;
	m_data = new double[len];

    if (initializer_list)
        std::memcpy (m_data, initializer_list, sizeof (double) * m_size);
    else
        for (size_t i = 0; i < m_size; i++)
            m_data[i] = 0.;
}

Vec::Vec (const Vec & other) :
        m_data (new double [other.m_size]),
        m_size (other.m_size)
{
    memcpy (m_data, other.m_data, sizeof (double) * m_size);
}

Vec::~Vec ()
{
    delete[] m_data;
    m_data = nullptr;
    m_size = 0;
}

void Vec::set (double arg, int coord)
{
    if (coord < 0 || coord >= m_size)
        throw vec_index_error("Exception: coordinate error in set()");

    m_data[coord] = arg;
}

double Vec::get (int coord) const
{
    if (coord < 0 || coord >= m_size)
		throw vec_index_error("Exception: coordinate error in get()");

    return m_data[coord];
}

double Vec::euc_norm () const
{
    double norm = 0.;

    for (size_t i = 0; i < m_size; i++)
        norm += m_data[i] * m_data[i];

    return std::sqrt (norm);
}

double Vec::max_norm () const
{
    double max = 0.;

    for (size_t i = 0; i < m_size; i++)
        if (max < std::abs(m_data[i]))
            max = std::abs(m_data[i]);

    return max;
}

void Vec::print () const
{
    std::cout << *this << std::endl;
}

Vec & Vec::operator= (const Vec &other)
{
    if (&other == this)
        return *this;

    if (m_size != other.m_size)
        resize (other.m_size);

    for (unsigned int i = 0; i < m_size; i++)
        m_data[i] = other.m_data[i];

    return *this;

}

const Vec Vec::operator+ (const Vec &other) const
{
    if (other.m_size != m_size) {
		std::stringstream s;
		s << "Exception: addition of vectors of different lengths: " << m_size << " " << other.m_size;
		throw vec_length_error(s.str());
	}

    Vec temp (*this);
    for (size_t i = 0; i < temp.m_size; ++i)
        temp.m_data[i] += other.m_data[i];

    return temp;
}

const Vec Vec::operator- (const Vec &other) const
{
	if (other.m_size != m_size) {
		std::stringstream s;
		s << "Exception: addition of vectors of different lengths: " << m_size << " " << other.m_size;
		throw vec_length_error (s.str());
	}

	Vec temp (*this);
    for (size_t i = 0; i < temp.m_size; ++i)
        temp.m_data[i] -= other.m_data[i];

    return temp;
}

bool Vec::operator== (const Vec &other) const
{
    return (m_size == other.m_size) && (!std::memcmp (m_data, other.m_data, m_size * sizeof (double)));
}

double &Vec::operator[] (int index)
{
    if (index < 0 || index >= m_size)
	{
		std::stringstream s;
		s << "Exception: incorrect indexing: " << index;
		throw vec_index_error (s.str());
	}

    return m_data[index];
}

double Vec::operator[] (int index) const
{
	if (index < 0 || index >= m_size)
	{
		std::stringstream s;
		s << "Exception: incorrect indexing: " << index;
		throw vec_index_error(s.str());
	}

    return m_data[index];
}

const Vec Vec::operator* (double right) const
{
    Vec temp (*this);
    for (size_t i = 0; i < temp.m_size; ++i)
        temp.m_data[i] *= right;

    return temp;
}

std::ostream &operator<< (std::ostream &left, const Vec &right)
{
    left << '(';
    for (size_t i = 0; i < right.m_size; i++)
    {
        left<< right.m_data[i];
        if (i < right.m_size - 1)
            left << ',';
    }

    left << ')';
	return left;
}

const Vec operator* (double left, const Vec &right)
{
    return right * left;
}

void Vec::resize (size_t size)
{
    double * nw = new double [size];
	std::memset(nw, 0, size);

    for (size_t i = 0; i < (m_size < size ? m_size : size); i++)
        nw[i] = m_data[i];

    delete[] m_data;
    m_data = nw;

    m_size = size;
}

vec_length_error::vec_length_error(const std::string& error) : length_error(error)
{

}

vec_index_error::vec_index_error(const std::string& error) : out_of_range(error)
{

}

int main() {
	try {
		error();
	} catch (std::logic_error & e) {
		std::cerr << e.what() << std::endl;
	} catch (...) {
		std::cerr << "Exception: unknown error" << std::endl;
	}
}


