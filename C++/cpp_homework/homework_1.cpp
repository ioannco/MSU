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

    protected:

    static int gcd(int a, unsigned b);

    int m_dividend;
    unsigned m_divisor;
};

/*
int main ()
{
    Quotient q1(16, -4), q2(6,12), q3(7), q4;
    q1.print();
    q2.print();
    q3.print();
    q4.print();
    return 0;

}
*/

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

int Quotient::gcd(int a, unsigned b)
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


