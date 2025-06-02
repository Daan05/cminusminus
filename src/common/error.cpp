#include "error.hpp"

#include <iostream>
#include <stdexcept>
#include <boost/stacktrace.hpp>

void error::unreachable() {
    std::cout << boost::stacktrace::stacktrace();
}

void error::todo(std::string message) {
    std::cout << "TODO: " << message << '\n';
}

void error::fatal(std::string message) {
    throw std::runtime_error(message);
}

void error::synchronize(std::string message) {
    throw std::runtime_error(message);
}

void error::report(size_t line, std::string message) {
    std::cout << "line " << line << ": " << message << '\n';
}

