#include "literal.hpp"

#include <iostream>
#include <string>

Literal::Literal(std::string const &s) : value(s) {}

Literal::Literal(std::string &&s) : value(std::move(s)) {}

Literal::Literal(double num) : value(num) {}

Literal::Literal(Literal const &other) : value(other.value) {}

Literal::~Literal() {}

void Literal::print() const
{
    std::cout << "literal type:";
    switch (value.index())
    {
    case 0:  // std::monostate
        std::cout << " None";
        break;
    case 1:  // std::string
        std::cout << " Str";
        std::cout << ", literal value: " << get<std::string>(value);
        break;
    case 2:  // double
        std::cout << " Num";
        std::cout << ", literal value: " << get<double>(value);
        break;
    }
}

std::string Literal::to_string() const
{
    switch (value.index())
    {
    case 0:  // std::monostate
        return "None";
        break;
    case 1:  // std::string
        return get<std::string>(value);
        break;
    case 2:  // double
        return std::to_string(get<double>(value));
        break;
    default: // unreachable
        return std::string();
        break;
    }
}
