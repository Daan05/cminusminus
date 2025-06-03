#include "error.hpp"

#include <iostream>
#include <boost/stacktrace.hpp>
#include <string>

// TODO: is this really the best way?
#define RESET "\033[0m"
#define MAGENTA "\033[35m"
#define RED "\033[31m"

error::Fatal::Fatal(std::string message) { report(-1, message); }

error::Synchronize::Synchronize(int line, std::string message)
{
    report(line, message);
}

void error::unreachable()
{
    auto trace = boost::stacktrace::stacktrace(0, 10);
    throw Fatal("unreachable code was reached");
}

void error::todo(std::string message)
{
    std::cout << "TODO: " << message << '\n';
}

void error::fatal(std::string message) { throw Fatal(message); }

void error::synchronize(int line, std::string message)
{
    throw Synchronize(line, message);
}

void error::report(int line, std::string message)
{
    if (line == -1)
    {
        std::cout << MAGENTA << "[Fatal error] " << RED << message << RESET
                  << '\n';
    }
    else
    {
        std::cout << MAGENTA << "[line " << line << "] " << RED << message
                  << RESET << '\n';
    }
}
