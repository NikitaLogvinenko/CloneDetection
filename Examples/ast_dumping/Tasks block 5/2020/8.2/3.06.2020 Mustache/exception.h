#ifndef MUSTACHE_EXCEPTION_H
#define MUSTACHE_EXCEPTION_H
#include <exception>
#include <string>

class badTextInput: public std::exception {
public:
    const char* what() const noexcept override { return "Text Invalid";}
};

class badContextInput: public std::exception {
public:
    const char* what() const noexcept override { return "Context Invalid";}
};
#endif //MUSTACHE_EXCEPTION_H
