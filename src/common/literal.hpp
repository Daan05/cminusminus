#ifndef LITERAL_HPP
#define LITERAL_HPP

#include <string>
#include <variant>

using LiteralValue = std::variant<std::monostate, std::string, double>;

struct Literal
{
    Literal() = default;
    Literal(std::string const &s);
    Literal(std::string &&s);
    Literal(double);
    Literal(Literal const &Literal);
    ~Literal();

    LiteralValue value;

    void print() const;
    std::string to_string() const;
};

#endif
