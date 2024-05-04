#ifndef Exceptions_hpp
#define Exceptions_hpp

#include <exception>
#include <string>

class OperException final : public std::exception {
public:
    
    const char* what() const noexcept override;
};

#endif /* Exceptions_hpp */
