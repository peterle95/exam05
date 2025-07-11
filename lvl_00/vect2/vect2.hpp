#ifndef VECT2_HPP
#define VECT2_HPP

#include <cmath>
#include <iostream>

class vect2 
{
    private:
        int x;
        int y;
    public:
        // Default constructor, initializes vector to {0, 0}
        vect2();
        // Parameterized constructor, initializes vector to {x, y}
        vect2(int x, int y);
        // Copy constructor
        vect2(const vect2& v);
        // Assignment operator
        vect2& operator=(const vect2& v);
        // Destructor
        ~vect2();

        // Operator overloads
        // Overloads the + operator to perform vector addition
        vect2 operator+(const vect2& other) const;
        // Overloads the - operator to perform vector subtraction
        vect2 operator-(const vect2& other) const;
        // Overloads the * operator for scalar multiplication
        vect2 operator*(int scalar) const;
        // Overloads the [] operator for component access
        int& operator[](int index);
        // Overloads the [] operator for const component access
        const int& operator[](int index) const;
        // Overloads the == operator for vector comparison
        bool operator==(const vect2& other) const;
        // Overloads the != operator for vector comparison
        bool operator!=(const vect2& other) const;
        // Overloads the += operator for vector addition
        vect2& operator+=(const vect2& other);
        // Overloads the -= operator for vector subtraction
        vect2& operator-=(const vect2& other);
        // Overloads the *= operator for scalar multiplication
        vect2& operator*=(int scalar);
        // Overloads the prefix ++ operator
        vect2& operator++();
        // Overloads the postfix ++ operator
        vect2 operator++(int);
        // Overloads the prefix -- operator
        vect2& operator--();
        // Overloads the postfix -- operator
        vect2 operator--(int);
        // Overloads the unary - operator
        vect2 operator-() const;

        // Friend function for the << operator overload
        friend std::ostream& operator<<(std::ostream& os, const vect2& v);
};

// Overloads the * operator for scalar multiplication (scalar * vector)
vect2 operator*(int scalar, const vect2& v);

#endif
