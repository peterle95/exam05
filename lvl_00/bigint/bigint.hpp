// bigint.hpp
#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <string>
#include <iostream>
#include <vector>

class bigint {
private:
    std::vector<int> digits; // Store digits in reverse order for easier operations

public:
    // Constructors
    bigint();
    bigint(unsigned long long n);
    bigint(const bigint& other);

    // Assignment operator
    bigint& operator=(const bigint& other);

    // Addition operators
    bigint operator+(const bigint& other) const;
    bigint& operator+=(const bigint& other);

    // Increment operators
    bigint& operator++(); // Prefix
    bigint operator++(int); // Postfix

    // Digit shift operators
    bigint operator<<(unsigned int shift) const;
    bigint operator>>(unsigned int shift) const;
    bigint& operator<<=(unsigned int shift);
    bigint& operator>>=(const bigint& shift);

    // Comparison operators
    bool operator<(const bigint& other) const;
    bool operator>(const bigint& other) const;
    bool operator==(const bigint& other) const;
    bool operator!=(const bigint& other) const;
    bool operator<=(const bigint& other) const;
    bool operator>=(const bigint& other) const;

    // Output operator
    friend std::ostream& operator<<(std::ostream& os, const bigint& bi);
};

#endif // BIGINT_HPP