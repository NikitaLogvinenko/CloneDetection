//
// Created by Sergey Vandanov on 03.06.2020.
//

#ifndef CHECKSUM_PARSERFORCHECK_H
#define CHECKSUM_PARSERFORCHECK_H

#include "Parser.h"

namespace CheckParser {
    class ParserForCheck final : public Parser::Parser {
    public:
        std::map<std::string, std::string> getParsedResult(const int &argc, const char **argv) const override;

        virtual ~ParserForCheck() {};
    };
}


#endif //CHECKSUM_PARSERFORCHECK_H
