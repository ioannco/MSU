//
// Created by ivan on 2/25/2022.
//
#include <iostream>

class Vector_2d
{
    int x, y;

public:
    Vector_2d (int arg_x = 0, int arg_y = 0) :
        x(arg_x), y(arg_y)
    {
    }

    Vector_2d operator + (const Vector_2d & other) const
    {
        Vector_2d temp;         // локальный объект
        temp.x = x + other.x;
        temp.y = y + other.y;
        return temp;            // возвращение через временный объект
    }

    Vector_2d operator - (const Vector_2d & other) const
    {
        return {x - other.x, y - other.y}; // анонимный объект
    }

    Vector_2d & operator = (const Vector_2d & other)
    {
        x = other.x;
        y = other.y;

        return *this;
    }

    void print (const char * str = nullptr) const
    {
        if (str)
            std::cout << str << " = ";
        std::cout << "(" << x << "," << y << ")" << std::endl;
    }
};

int main ()
{

}



