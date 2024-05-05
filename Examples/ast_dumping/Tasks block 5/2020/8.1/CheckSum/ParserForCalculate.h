#ifndef CHECKSUM_PARSERFORCALCULATE_H
#define CHECKSUM_PARSERFORCALCULATE_H

#include "Parser.h"
namespace CalcParser {
    class ParserForCalculate final : public Parser::Parser {
    public:
        ParserForCalculate() {};
        std::map<std::string, std::string> getParsedResult(const int& argc, const char** argv) const override;

        ~ParserForCalculate() {};
    };
}


#endif //CHECKSUM_PARSERFORCALCULATE_H
