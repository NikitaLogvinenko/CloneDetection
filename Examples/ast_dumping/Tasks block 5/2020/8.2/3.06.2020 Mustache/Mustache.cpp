#include <iostream>
#include <sstream>
#include <utility>
#include "Mustache.h"
namespace constant {
    const int START_BAD_CHARACTER = 58;
    const int END_BAD_CHARACTER = 64;
}

mstch::Mustache::Mustache(std::string text) : _text(text) {}


std::string mstch::Mustache::generateString(const std::map<std::string, std::string> &context) {
    return processText(context);
}

std::string mstch::Mustache::processText(const std::map<std::string, std::string> &context) {
    std::istringstream iss(_text);
    std::string result, line;
    bool wasOpened = false, enter = false, wasSpecial = false;
    if(!_text.empty() && _text.substr(_text.length()-1,_text.length())=="\n") {
        enter = true;
    }
    Position position;
    while (std::getline(iss, line)) {
        while (parseText(line, position)) {
            if (!(inclusionConditional(line, position, wasOpened, context,wasSpecial)
                  || inclusionReverseConditional(line, position, wasOpened, context,wasSpecial)
                  || removeComments(line, position))) {
                if ((int) line[position.start + 2] <= constant::END_BAD_CHARACTER && (int) line[position.start + 2] >= constant::START_BAD_CHARACTER) throw badTextInput();
                insertVariables(line, position, context);
            }
        }
        formResult(result, line, wasOpened);
    }
    if(!result.empty() && !enter) {result.erase(result.length() - 1, result.length());}
    return result;
}

void mstch::Mustache::insertVariables(std::string &line, const Position &pos,
                                      const std::map<std::string, std::string> &context) {
    const std::string keyValue = line.substr(pos.start + 2, pos.end - pos.start - 2);
    if (context.find(keyValue) != context.end()) {
        line.replace(pos.start, pos.end - pos.start + 2, context.at(keyValue));
    } else {
        throw badContextInput();
    }
}

bool mstch::Mustache::inclusionConditional(std::string &line, const Position &pos,
                                           bool& wasOpened, const std::map<std::string, std::string> &context,bool& wasSpecial) {
    if (line[pos.start + 2] == '#' || line[pos.start + 2] == '/') {
        std::string keyValue = line.substr(pos.start + 3, pos.end - pos.start - 3);
        if (!wasSpecial && (context.count(keyValue) == 0 || context.at(keyValue).empty())) {
            line = "";
            wasOpened = line[pos.start + 2] == '#';
        } else {
            line.replace(pos.start, pos.end - pos.start + 2, "");
            wasOpened = false;
            if(wasSpecial && line[pos.start + 2] == '/')  wasSpecial= false;
        }
        return true;
    }
    return false;
}

bool mstch::Mustache::inclusionReverseConditional(std::string &line, const Position &pos,
                                                  bool& wasOpened, const std::map<std::string, std::string> &context,bool& wasSpecial) {
    if (line[pos.start + 2] == '^') {
        std::string keyValue = line.substr(pos.start + 3, pos.end - pos.start - 3);
        if (context.count(keyValue) == 0 || context.at(keyValue).empty()) {
            line.replace(pos.start, pos.end - pos.start + 2, "");
            wasSpecial = true;
        } else {
            line = "";
            wasOpened = true;
        }
        return true;
    }
    return false;
}

bool mstch::Mustache::removeComments(std::string &line, const Position &pos) {
    if (line[pos.start + 2] == '!') {
        line.replace(pos.start, pos.end - pos.start + 2, "");
        return true;
    }
    return false;
}

void mstch::Mustache::formResult(std::string &result, const std::string &line, bool&wasOpened) {
    if (line.empty()) {
        return;
    }
    if (!wasOpened) result += line + "\n";
}

bool mstch::Mustache::parseText(const std::string &line, Position &pos) {
    pos.start = line.find_first_of("{{", pos.start);
    pos.end = line.find_first_of("}}", pos.start + 2);
    if ((pos.start != std::string::npos && pos.end == std::string::npos) ||
        (pos.start == std::string::npos && pos.end != std::string::npos)) {
        throw badTextInput();
    }
    if (pos.start == std::string::npos && pos.end == std::string::npos) {
        pos.start = 0;
        pos.end = 0;
        return false;
    }
    return true;
}
