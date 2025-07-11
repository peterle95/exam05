#include "vect2.hpp"

// Default constructor, initializes vector to {0, 0}
vect2::vect2() : x(0), y(0) {}

// Parameterized constructor, initializes vector to {x, y}
vect2::vect2(int x, int y) : x(x), y(y) {}

// Copy constructor
vect2::vect2(const vect2& v) : x(v.x), y(v.y) {}

// Assignment operator
vect2& vect2::operator=(const vect2& v) {
    // Self-assignment check
    if (this != &v) {
        x = v.x;
        y = v.y;
    }
    return *this;
}

// Destructor
vect2::~vect2() {
    // No dynamic memory to free, but destructor is needed for completeness
}

// Overloads the + operator to perform vector addition
vect2 vect2::operator+(const vect2& other) const {
    return vect2(x + other.x, y + other.y);
}

// Overloads the - operator to perform vector subtraction
vect2 vect2::operator-(const vect2& other) const {
    return vect2(x - other.x, y - other.y);
}

// Overloads the * operator for scalar multiplication
vect2 vect2::operator*(int scalar) const {
    return vect2(x * scalar, y * scalar);
}

// Overloads the [] operator for component access
int& vect2::operator[](int index) {
    if (index == 0)
        return x;
    return y;
}

// Overloads the [] operator for const component access
const int& vect2::operator[](int index) const {
    if (index == 0)
        return x;
    return y;
}

// Overloads the == operator for vector comparison
bool vect2::operator==(const vect2& other) const {
    return (x == other.x && y == other.y);
}

// Overloads the != operator for vector comparison
bool vect2::operator!=(const vect2& other) const {
    return !(*this == other);
}

// Overloads the += operator for vector addition
vect2& vect2::operator+=(const vect2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

// Overloads the -= operator for vector subtraction
vect2& vect2::operator-=(const vect2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

// Overloads the *= operator for scalar multiplication
vect2& vect2::operator*=(int scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

// Overloads the prefix ++ operator
vect2& vect2::operator++() {
    ++x;
    ++y;
    return *this;
}

// Overloads the postfix ++ operator
vect2 vect2::operator++(int) {
    vect2 temp(*this);
    ++(*this);
    return temp;
}

// Overloads the prefix -- operator
vect2& vect2::operator--() {
    --x;
    --y;
    return *this;
}

// Overloads the postfix -- operator
vect2 vect2::operator--(int) {
    vect2 temp(*this);
    --(*this);
    return temp;
}

// Overloads the unary - operator
vect2 vect2::operator-() const {
    return vect2(-x, -y);
}

// Friend function for the << operator overload
std::ostream& operator<<(std::ostream& os, const vect2& v) {
    os << "{" << v.x << ", " << v.y << "}";
    return os;
}

// Overloads the * operator for scalar multiplication (scalar * vector)
vect2 operator*(int scalar, const vect2& v) {
    return v * scalar;
}
