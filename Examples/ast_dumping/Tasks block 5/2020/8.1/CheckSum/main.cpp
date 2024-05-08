#include "CheckSum.h"
#include "exception.h"
#include "Parser.h"
#include "ParserForCalculate.h"
#include "ParserForCheck.h"

using namespace std;

int main(int argc, const char **argv) {
    if (argc < 4) throw ArgCountInvalid();
    if ((strcmp(argv[2], "--calc") != 0) && (strcmp(argv[2], "--check") != 0)) throw ArgCountInvalid();
    const Parser::Parser &checkParser = CheckParser::ParserForCheck();
    const Parser::Parser &manifestParser = CalcParser::ParserForCalculate();
    const CheckSum &data = (strcmp(argv[2], "--calc") == 0) ? CheckSum(manifestParser) : CheckSum(checkParser);
    map<string, string> parsedData = data.getParsedData(argc, argv);
    CheckSum::printResultOfChecking(parsedData);

    map<string, string> result = data.calculateCheckSum(parsedData, "apple.txt");
    CheckSum::writeToManifest(result, "apple.txt");
    CheckSum::printResultOfChecking(result);
}