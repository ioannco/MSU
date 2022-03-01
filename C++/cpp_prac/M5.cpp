//
// Created by Ivan on 2/22/2022.
//

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <iostream>

class Vec
{
public:
    explicit  Vec (size_t len, double * initializer_list = nullptr);
    Vec (const Vec & other);
    ~Vec ();

    void set (double arg, size_t coord);
    double get (size_t coord) const;
    double euc_norm () const;
    double max_norm () const;
    void print () const;

    Vec & operator = (const Vec & other);
    Vec operator + (const Vec & other) const;
    Vec operator - (const Vec & other) const;
    bool operator == (const Vec & other) const;
    double & operator [] (size_t index);
    double operator [] (size_t index) const;
    Vec operator * (double right) const;


    friend std::ostream & operator << (std::ostream & left, const Vec & right);
    friend Vec operator * (double left, const Vec & right);

private:
    void resize (size_t size);

    double * m_data;
    size_t m_size;
};

Vec::Vec (size_t len, double * initializer_list) :
        m_data (new double [len]),
        m_size (len)
{
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

void Vec::set (double arg, size_t coord)
{
    if (coord >= m_size)
        return; // error

    m_data[coord] = arg;
}

double Vec::get (size_t coord) const
{
    if (coord >= m_size)
        return 0; // error

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

Vec Vec::operator+ (const Vec &other) const
{
    if (other.m_size != m_size)
        throw std::invalid_argument ("Vec::operator +: vectors' lengths must be the same");

    Vec temp (*this);
    for (size_t i = 0; i < temp.m_size; ++i)
        temp.m_data[i] += other.m_data[i];

    return temp;
}

Vec Vec::operator- (const Vec &other) const
{
    if (other.m_size != m_size)
        throw std::invalid_argument ("Vec::operator +: vectors' lengths must be the same");

    Vec temp (*this);
    for (size_t i = 0; i < temp.m_size; ++i)
        temp.m_data[i] -= other.m_data[i];

    return temp;
}

bool Vec::operator== (const Vec &other) const
{
    return (m_size == other.m_size) && (!std::memcmp (m_data, other.m_data, m_size * sizeof (double)));
}

double &Vec::operator[] (size_t index)
{
    if (index >= m_size)
        throw std::invalid_argument ("Vec::operator[]: vector index out of range");

    return m_data[index];
}

double Vec::operator[] (size_t index) const
{
    if (index >= m_size)
        throw std::invalid_argument ("Vec::operator[]: vector index out of range");

    return m_data[index];
}

Vec Vec::operator* (double right) const
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
}

Vec operator* (double left, const Vec &right)
{
    return right * left;
}

void Vec::resize (size_t size)
{
    double * nw = new double [size];

    for (size_t i = 0; i < m_size < size ? m_size : size; i++)
        nw[i] = m_data[i];

    delete[] m_data;
    m_data = nw;

    m_size = size;
}

int main()
{
        using namespace std;

        double y1[4] = {1,2,3,4};
        double y2[4] = {-2,1,3,-3};

        Vec u(4, y1), v(4, y2);
        Vec w(4);

        cout << "u = " << u << endl;
        cout << "v = " << v << endl;
        w = u = u;
        cout << "w = u = " << w << endl;
        w = 4.*u + v*3;
        cout << "w = 4.*u + v*3 = " << w << endl;
        w[2] = 3;
        w[0] = -1;
        cout << "w = " << w << endl;

        return 0;
}


