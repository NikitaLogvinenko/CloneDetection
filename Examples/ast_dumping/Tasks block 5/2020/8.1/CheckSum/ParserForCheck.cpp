//
// Created by Sergey Vandanov on 03.06.2020.
//

#include <fstream>
#include "ParserForCheck.h"
#include "exception.h"
#include <string>
#include <iostream>
#include <map>

std::map<std::string, std::string> CheckParser::ParserForCheck::getParsedResult(const int &argc, const char **argv) const {
    std::ifstream ifs(argv[3]);
    if (!ifs.is_open()) {
        throw FileDoesntExist();
    }
    std::string line;
    std::map<std::string, std::string> result;
    while (getline(ifs, line)) {
        size_t doubleDot = line.find_last_of(':');
        if (doubleDot == std::string::npos) {
            throw InvalidFileContext();
        }
        std::string filename = line.substr(0, doubleDot);
        std::string checkSum = line.substr(doubleDot + 1);
        result.insert(std::make_pair(filename, checkSum));
    }
    ifs.close();
    return result;
}