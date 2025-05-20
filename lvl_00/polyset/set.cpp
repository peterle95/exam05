#include "set.hpp"

set::set(searchable_bag &bag) : bag(bag) {}

set::set(const set &other) : bag(other.bag) {}

set &set::operator=(const set &other) {
    (void)other;
    return *this;
}

set::~set() {}

void set::insert(int val) {
    if (!this->bag.has(val))
        this->bag.insert(val);
}

void set::insert(int *array, int size) {
    for (int i = 0; i < size; i++)
        this->insert(array[i]);
}

void set::print() const {
    this->bag.print();
}

void set::clear() {
    this->bag.clear();
}

bool set::has(int val) const {
    return this->bag.has(val);
}

searchable_bag &set::get_bag() const {
    return this->bag;
}