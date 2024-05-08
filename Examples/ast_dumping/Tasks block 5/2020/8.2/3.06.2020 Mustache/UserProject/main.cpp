#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include "Mustache.h"

int main() {
    std::string text;
    std::string myString;
    std::map<std::string, std::string> context;
    size_t pos = 0;
    std::cout << "Input your context: (after double enter input will be ended)" << std::endl;
    while (getline(std::cin, myString)) {
        pos = 0;
        if (myString.empty()) break;
        else {
            pos = myString.find_first_of(' ', pos);
            if (pos == std::string::npos) break;
            context.insert({myString.substr(0, pos), myString.substr(pos + 1, myString.size())});
        }
    }
    std::cout << "Input your text: (after double enter input will be ended)" << std::endl;
    while (getline(std::cin, text)) {
        if (text.empty()) break;
        mstch::Mustache a(text);
        std::cout << a.generateString(context) << std::endl;
    }

    return 0;
}