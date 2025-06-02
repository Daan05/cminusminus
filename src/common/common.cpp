#include "common.hpp"

#include <fstream>

#include "error.hpp"

namespace common
{
std::string read_file(const std::string &path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        error::fatal("Failed to open: " + path);
    }

    auto const size = file.tellg();
    if (size == -1)
    {
        error::fatal("Failed to determine file size: " + path);
    }

    std::string content(size, '\0');
    file.seekg(0);
    file.read(content.data(), size);

    if (!file)
    {
        error::fatal("Failed to read entire file: " + path);
    }

    return content;
}

void write_file(std::string const &path, std::string const &code)
{
    std::ofstream file(path);

    if (!file)
    {
        error::fatal("Failed to open for writing: " + path);
    }

    file << code;
}
}  // namespace common
