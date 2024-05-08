//
// Created by Sergey Vandanov on 03.06.2020.
//

#ifndef CHECKSUM_PARSER_H
#define CHECKSUM_PARSER_H

#include <string>
#include <map>

namespace Parser {
    class Parser {
    public:
        virtual std::map<std::string, std::string> getParsedResult(const int &argc, const char **argv) const = 0;

        virtual ~Parser() {};

    };
}


#endif //CHECKSUM_PARSER_H
