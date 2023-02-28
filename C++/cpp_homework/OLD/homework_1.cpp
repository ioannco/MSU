//
// Created by Ivan on 2/13/2022.
//
#include <iostream>
#include <cstdio>

class Quotient
{
    public:

    explicit Quotient (int dividend = 0, int divisor = 1);
    void print () const;

    private:

    static int gcd(int a, int b);

    int m_dividend;
    unsigned m_divisor;
};


Quotient::Quotient (int dividend, int divisor)
{
    if (!divisor )
        m_divisor = 1;
    else
        m_divisor = abs (divisor);

    if (!dividend)
    {
        m_dividend = 0;
        return;
    }

    int sign;
    if (divisor == 0)
        sign = 1;
    else
        sign = divisor / static_cast<int> (m_divisor);

    m_dividend = dividend * sign / gcd (abs (dividend), abs (divisor));
    m_divisor /= gcd (abs (dividend), abs (divisor));
}

int Quotient::gcd(int a, int b)
{
    if (!a || !b)
        return 1;

    while (a != b)
    {
        if (a > b)
        {
            int tmp = a;
            a = b;
            b = tmp;
        }
        b = b - a;
    }

    return a;
}

void Quotient::print() const
{

    std::cout << m_dividend;
    if (m_divisor != 1)
        std::cout << "/" << m_divisor;
    std::cout << std::endl;
}


