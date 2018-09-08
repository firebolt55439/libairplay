#pragma once

#include <string>
#include <iostream>
#include <exception>

class project_exception : std::exception {
public:
    explicit project_exception(const std::string& message) :
        _message(message)
    {};
    virtual ~project_exception() = default;

    std::string get_message() const {
        return _message;
    }

private:
    const std::string _message;
};

#define CHECK_AND_THROW(condition, message)     \
    if (!(condition)) {                         \
        throw project_exception(message);       \
    }

#define PRINT_ANY_EXCEPTION()                   \
    std::cerr << "Caught general exception (errno: " << errno << ")" << std::endl;
