#ifndef COMMON_COMMON_HPP
#define COMMON_COMMON_HPP

#include <string>

namespace common
{
std::string read_file(std::string const &path);
void write_file(std::string const &path, std::string const &code);
}  // namespace common

#endif
