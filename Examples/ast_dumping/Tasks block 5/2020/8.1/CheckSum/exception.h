//
// Created by Sergey Vandanov on 03.06.2020.
//

#ifndef CHECKSUM_EXCEPTION_H
#define CHECKSUM_EXCEPTION_H


#include <exception>
#include <string>

class ArgCountInvalid: public std::exception  {
public:
    const char* what() const noexcept override { return "ArgCount should be more than 2";}
};

class FileDoesntExist: public std::exception  {
public:
    const char* what() const noexcept override { return "File doesn't exist";}
};

class InvalidFileContext: public std::exception  {
public:
    const char* what() const noexcept override { return "Invalid string in manifest file";}
};



#endif //CHECKSUM_EXCEPTION_H
