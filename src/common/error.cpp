#include "error.hpp"

#include <iostream>
#include <boost/stacktrace.hpp>
#include <string>

error::Fatal::Fatal(std::string message) { throw Fatal(message); }

error::Synchronize::Synchronize(std::string message)
{
    std::cout << message << '\n';
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

void error::synchronize(std::string message) { throw Synchronize(message); }

void error::report(size_t line, std::string message)
{
    std::cout << "[line " << line << "] " << message << '\n';
}
