#ifndef ERROR_HPP
#define ERROR_HPP

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
    Synchronize(int line, std::string message);
    ~Synchronize() = default;
};

void unreachable();
void todo(std::string message);
void fatal(std::string message);
void synchronize(int line, std::string message);
void report(int line, std::string message);
}  // namespace error

#endif
