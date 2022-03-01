
//
// Created by Ivan on 2/15/2022.
//

#include <iostream>

class Rect
{
    public:
    Rect (int x, int y, unsigned width, unsigned height);
    Rect ();
    Rect (unsigned size);

    void print_corner () const;
    void print_sz () const;
    int area() const;
    int perimeter() const;
    bool intersect (const Rect& r) const;

    private:

    int m_x, m_y;
    unsigned int m_width, m_height;
};

Rect::Rect (int x, int y, unsigned width, unsigned height)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
}

Rect::Rect ()
{
    m_x = 0;
    m_y = 0;
    m_width = 1;
    m_height = 1;
}

Rect::Rect (unsigned size)
{
    m_x = 0;
    m_y = 0;
    m_width = size;
    m_height = size;
}

void Rect::print_corner () const
{
    std::cout << "(" << m_x << "," << m_y << ")" << std::endl;
}

void Rect::print_sz () const
{
    std::cout << "wd=" << m_width << " hd=" << m_height << std::endl;
}

int Rect::area () const
{
    return static_cast<int>(m_width) * static_cast<int>(m_height);
}

int Rect::perimeter () const
{
    return static_cast<int>(m_width) * 2 + static_cast<int>(m_height) * 2;
}

bool Rect::intersect (const Rect& r) const
{
    bool cond1 = m_x <= r.m_x + static_cast<int> (r.m_width);
    bool cond2 = m_x + static_cast<int> (m_width) >= r.m_x;
    bool cond3 = m_y <= r.m_y + static_cast<int> (r.m_height);
    bool cond4 = m_y + static_cast<int> (m_height) >= r.m_y;

    return cond1 && cond2 && cond3 && cond4;
}

/*
int main()
{
    using namespace std;

    Rect r1(-2, -3, 4, 5);
    Rect r2;
    Rect r3(7);
    r1.print_corner();
    r1.print_sz();

    int a2 = r2.area();
    int p2 = r2.perimeter();
    cout << a2 << " " << p2 << endl;

    if (r1.intersect(r3))
        cout << "YES";
    else
        cout << "NO";

    return 0;
}
*/
