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

template <typename T>
class Vec
{
 public:
	explicit  Vec (int len, T * initializer_list = nullptr);
	Vec (const Vec & other);
	~Vec ();

	void set (T arg, int coord);
	T get (int coord) const;
	T euc_norm () const;
	T max_norm () const;
	void print () const;

	Vec & operator = (const Vec & other);
	const Vec operator + (const Vec & other) const;
	const Vec operator - (const Vec & other) const;
	bool operator == (const Vec & other) const;
	T & operator [] (int index);
	T operator [] (int index) const;
	const Vec operator * (T right) const;
	T operator ^ (const Vec & other) const;
	Vec & operator += (const Vec & other);

	template<typename V>
	friend std::ostream & operator << (std::ostream & left, const Vec<V> & right);

	template<typename V>
	friend const Vec<V> operator * (V left, const Vec<V> & right);

 private:
	void resize (size_t size);

	T * m_data;
	size_t m_size;
};

template<typename T>
Vec<T>::Vec (int len, T * initializer_list)
{
	if (len < 0)
		throw vec_length_error("Exception: length error");

	m_size = len;
	m_data = new T[len];

	if (initializer_list)
		std::memcpy (m_data, initializer_list, sizeof (T) * m_size);
	else
		for (size_t i = 0; i < m_size; i++)
			m_data[i] = 0.;
}

template<typename T>
Vec<T>::Vec (const Vec & other) :
	m_data (new T [other.m_size]),
	m_size (other.m_size)
{
	memcpy (m_data, other.m_data, sizeof (T) * m_size);
}

template<typename T>
Vec<T>::~Vec ()
{
	delete[] m_data;
	m_data = nullptr;
	m_size = 0;
}

template<typename T>
void Vec<T>::set (T arg, int coord)
{
	if (coord < 0 || coord >= m_size)
		throw vec_index_error("Exception: coordinate error in set()");

	m_data[coord] = arg;
}

template<typename T>
T Vec<T>::get (int coord) const
{
	if (coord < 0 || coord >= m_size)
		throw vec_index_error("Exception: coordinate error in get()");

	return m_data[coord];
}

template<typename T>
T Vec<T>::euc_norm () const
{
	T norm = T();

	for (size_t i = 0; i < m_size; i++)
		norm += m_data[i] * m_data[i];

	return std::sqrt (norm);
}

template<typename T>
T Vec<T>::max_norm () const
{
	T max = T();

	for (size_t i = 0; i < m_size; i++)
		if (max < std::abs(m_data[i]))
			max = std::abs(m_data[i]);

	return max;
}

template<typename T>
void Vec<T>::print () const
{
	std::cout << *this << std::endl;
}

template<typename T>
Vec<T> & Vec<T>::operator= (const Vec<T> &other)
{
	if (&other == this)
		return *this;

	if (m_size != other.m_size)
		resize (other.m_size);

	for (unsigned int i = 0; i < m_size; i++)
		m_data[i] = other.m_data[i];

	return *this;
}

template<typename T>
const Vec<T> Vec<T>::operator+ (const Vec<T> &other) const
{
	if (other.m_size != m_size) {
		std::stringstream s;
		s << "Exception: addition of vectors of different lengths: " << m_size << " " << other.m_size;
		throw vec_length_error(s.str());
	}

	Vec<T> temp (*this);
	for (size_t i = 0; i < temp.m_size; ++i)
		temp.m_data[i] += other.m_data[i];

	return temp;
}

template<typename T>
const Vec<T> Vec<T>::operator- (const Vec<T> &other) const
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

template<typename T>
bool Vec<T>::operator== (const Vec<T> &other) const
{
	return (m_size == other.m_size) && (!std::memcmp (m_data, other.m_data, m_size * sizeof (double)));
}

template<typename T>
T &Vec<T>::operator[] (int index)
{
	if (index < 0 || index >= m_size)
	{
		std::stringstream s;
		s << "Exception: incorrect indexing: " << index;
		throw vec_index_error (s.str());
	}

	return m_data[index];
}

template<typename T>
T Vec<T>::operator[] (int index) const
{
	if (index < 0 || index >= m_size)
	{
		std::stringstream s;
		s << "Exception: incorrect indexing: " << index;
		throw vec_index_error(s.str());
	}

	return m_data[index];
}

template<typename T>
const Vec<T> Vec<T>::operator* (T right) const
{
	Vec<T> temp (*this);
	for (size_t i = 0; i < temp.m_size; ++i)
		temp.m_data[i] *= right;

	return temp;
}

template<typename V>
std::ostream &operator<< (std::ostream &left, const Vec<V> &right)
{
	left << '(';
	for (size_t i = 0; i < right.m_size; i++)
	{
		left << right.m_data[i];
		if (i < right.m_size - 1)
			left << ',';
	}

	left << ')';
	return left;
}

template<typename V>
const Vec<V> operator* (V left, const Vec<V> &right)
{
	return right * left;
}

template<typename T>
void Vec<T>::resize (size_t size)
{
	T * nw = new T [size];
	std::memset(nw, 0, size);

	for (size_t i = 0; i < (m_size < size ? m_size : size); i++)
		nw[i] = m_data[i];

	delete[] m_data;
	m_data = nw;

	m_size = size;
}

template <typename T>
T Vec<T>::operator^(const Vec& other) const
{
	if (other.m_size != m_size) {
		std::stringstream s;
		s << "Exception: multiplication of vectors of different lengths: " << m_size << " " << other.m_size;
		throw vec_length_error (s.str());
	}

	T mult = T();
	for (size_t i = 0; i < m_size; i++) {
		mult += m_data[i] * other.m_data[i];
	}

	return mult;
}

template <typename T>
Vec<T> & Vec<T>::operator+=(const Vec& other)
{
	if (other.m_size != m_size) {
		std::stringstream s;
		s << "Exception: addition of vectors of different lengths: " << m_size << " " << other.m_size;
		throw vec_length_error (s.str());
	}

	*this = *this + other;
	return *this;
}

vec_length_error::vec_length_error(const std::string& error) : length_error(error)
{

}

vec_index_error::vec_index_error(const std::string& error) : out_of_range(error)
{

}


int main() {
	try {
		Vector();
	} catch (std::logic_error & e) {
		std::cerr << e.what() << std::endl;
	} catch (...) {
		std::cerr << "Exception: unknown error" << std::endl;
	}
}



