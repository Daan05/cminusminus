#ifndef ERROR_HPP
#define ERROR_HPP

#include <cstddef>
#include <string>

namespace error
{
void unreachable();
void todo(std::string message);
void fatal(std::string message);
void synchronize(std::string message);
void report(size_t line, std::string message);
}  // namespace error

#endif
