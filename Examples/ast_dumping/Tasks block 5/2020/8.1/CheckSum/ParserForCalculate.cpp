#include "ParserForCalculate.h"


std::map<std::string, std::string> CalcParser::ParserForCalculate::getParsedResult(const int &argc, const char **argv) const {
    std::map<std::string, std::string> result;
    for (int i = 3; i < argc; i++) {
        result.insert(std::make_pair(argv[i], std::to_string(i)));
    }
    return result;
}
