#ifndef MUSTACHE_MUSTACHE_H
#define MUSTACHE_MUSTACHE_H
#include <string>
#include <map>
#include "exception.h"
namespace mstch {
    struct Position {
        size_t start = 0, end = 0;
    };
    class Mustache final {
    public:
        explicit Mustache(std::string text);

        std::string generateString(const std::map<std::string, std::string> &context);

    private:
        std::string _text;
        std::string processText(const std::map<std::string, std::string> &context);

        static void insertVariables(std::string &line, const Position &pos,
                             const std::map<std::string, std::string> &context);

        static bool inclusionConditional(std::string &line, const Position &pos, bool& wasOpened,
                                  const std::map<std::string, std::string> &context,bool& wasSpecial);

        static bool inclusionReverseConditional(std::string &line, const Position &pos, bool& wasOpened,
                                         const std::map<std::string, std::string> &context,bool& wasSpecial);

        static bool removeComments(std::string &line, const Position &pos);

        static bool parseText(const std::string &line, Position &pos);

        static void formResult(std::string &result, const std::string &line, bool& wasOpened);
    };
}


#endif //MUSTACHE_MUSTACHE_H
