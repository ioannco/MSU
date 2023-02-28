//
// Created by Ivan on 2/19/2022.
//

#include <iostream>

class Array {
public:
    Array ()
    {
        m_arr = new int [10];
        m_size = 10;
    }

    ~Array(){
        delete [] m_arr;
    }

    int & elem (int index);

    int size ();

private:
    int *m_arr;
    int m_size;
};

int main ()
{
    Array a;
}

int & Array::elem (int index)
{
    return m_arr [index];
}

int Array::size ()
{
    return m_size;
}

