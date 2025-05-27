#include "literal.hpp"

#include <iostream>

Literal::Literal(std::string const &s) : value(s) {}

Literal::Literal(std::string &&s) : value(std::move(s)) {}

Literal::Literal(double num) : value(num) {}

Literal::Literal(Literal const &other) : value(other.value) {}

Literal::~Literal() {}

void Literal::print() const
{
    std::cout << "literal type: ";
    switch (value.index())
    {
    case 0:  // std::monostate
        std::cout << "None\n";
        break;
    case 1:  // std::string
        std::cout << "Str\n";
        std::cout << "literal value: " << get<std::string>(value) << '\n';
        break;
    case 2:  // double
        std::cout << "Num\n";
        std::cout << "literal value: " << get<double>(value) << '\n';
        break;
    }
}

