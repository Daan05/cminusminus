#ifndef ERROR_HPP
#define ERROR_HPP

#include <cstddef>
#include <string>

namespace error
{
struct Fatal
{
    Fatal(std::string message);
    ~Fatal() = default;
};

struct Synchronize
{
    Synchronize(std::string message);
    ~Synchronize() = default;
};

void unreachable();
void todo(std::string message);
void fatal(std::string message);
void synchronize(std::string message);
void report(size_t line, std::string message);
}  // namespace error

#endif
