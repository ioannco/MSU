//
// Created by Ivan on 2/22/2022.
//

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

class Vec
{
public:
    explicit  Vec (size_t len, double * initializer_list = nullptr);
    Vec (const Vec & other);
    ~Vec ();

    void set (double arg, int coord);
    double get (int coord) const;
    double euc_norm () const;
    double max_norm () const;
    void print () const;

private:
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

void Vec::set (double arg, int coord)
{
    if (coord >= m_size)
        return; // error

    m_data[coord] = arg;
}

double Vec::get (int coord) const
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
    std::cout << '(';
    for (size_t i = 0; i < m_size; i++)
    {
        std::cout << m_data[i];
        if (i < m_size - 1)
            std::cout << ',';
    }

    std::cout << ')' << std::endl;
}



