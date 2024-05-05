#include "Exceptions.hpp"

const char* OperException::what() const noexcept{
    return "Main parameters not equal";
}
