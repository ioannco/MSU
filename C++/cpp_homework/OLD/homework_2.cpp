//
// Created by Ivan on 2/22/2022.
//

#include <cstring>
#include <iostream>
#include <stdexcept>

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
    void insert (char c, size_t pos);
    void insert (const char * c_str, size_t pos);
    void del (size_t i);
    void del (size_t begin, size_t end);
    int search (const char * str) const;
    void replace (const char * sub_str, const char * new_str);
    void print () const;

    friend std::ostream & operator << (std::ostream & left, const mstring & right);

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

void mstring::insert (char c, size_t pos)
{
    if (!m_str)
        return;

    if (m_reserved <= m_length + 1)
        mstring::realloc (static_cast<size_t> (static_cast<double> (m_length + 1) * 1.5 + 1));

    std::memcpy (m_str + pos + 1, m_str + pos, m_length - pos);

    m_str[pos] = c;

    m_length++;
}

void mstring::insert (const char * c_str, size_t pos)
{
    if (!m_str)
        return;

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
    insert (c, m_length - 1);
}

void mstring::add (const char * c_str)
{
    insert (c_str, m_length - 1);
}

void mstring::del (size_t i)
{
    if (!m_str)
        return;

    std::memcpy (m_str + i, m_str + i + 1, m_length - i - 1);
    m_length--;

    if (m_reserved > 2 * (m_length))
        mstring::realloc (static_cast<size_t> (static_cast<double> (m_length) * 1.5 + 1));

}

void mstring::del (size_t begin, size_t end)
{
    if (!m_str)
        return;

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
