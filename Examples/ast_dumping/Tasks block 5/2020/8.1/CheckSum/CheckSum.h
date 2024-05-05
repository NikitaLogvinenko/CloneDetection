//
// Created by Sergey Vandanov on 03.06.2020.
//

#ifndef CHECKSUM_CHECKSUM_H
#define CHECKSUM_CHECKSUM_H

#include <istream>
#include "Parser.h"

class CheckSum {
private:
    const Parser::Parser &_typosParse;

    static unsigned short calcCRC16(const std::string &filename);

public:
    explicit CheckSum(const Parser::Parser &typosParse) : _typosParse(typosParse) {};

    std::map<std::string, std::string> getParsedData(const int &argc, const char **argv) const;

    std::map<std::string, std::string>
    calculateCheckSum(const std::map<std::string, std::string> &parsedData, const std::string &nameManifestFile) const;

    static void writeToManifest(const std::map<std::string, std::string> &result, const std::string &nameManifestFile);

    static void printResultOfChecking(const std::map<std::string, std::string> &parsedData);
};


#endif //CHECKSUM_CHECKSUM_H
