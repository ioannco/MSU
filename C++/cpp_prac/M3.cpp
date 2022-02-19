
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
    bool contains (int x, int y) const;

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

bool Rect::contains (int x, int y) const
{
    return (x >= m_x && x <= m_x + m_width) && (y >= m_y && y <= m_y + m_height);
}

bool Rect::intersect (const Rect& r) const
{
    return contains (r.m_x, r.m_y) ||
           contains (r.m_x + r.m_width, r.m_y + r.m_height) ||
           contains (r.m_x, r.m_y + r.m_height) ||
           contains (r.m_x + r.m_width, r.m_y);
}
