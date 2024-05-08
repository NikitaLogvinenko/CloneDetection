#ifndef LUNCH_CALCULATOR_LUNCH_CALCULATOR_H
#define LUNCH_CALCULATOR_LUNCH_CALCULATOR_H

#include "event.h"
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <istream>
#include <list>
#include <locale>
#include <iomanip>

class lunch_calculator {
public:
    inline explicit lunch_calculator(const std::string &file_name) {
        if(file_name.empty())
            return ;
        std::fstream file_stream;
        file_stream.open(file_name);
        if (file_stream) parcing(file_stream);
    }

    inline explicit lunch_calculator(std::istream &istream) {
        if (istream) parcing(istream);
    };
    void add_events(std::istream &istream)
    {
        if (istream) parcing(istream);
    }
    std::vector<std::string> to_offer_advice(const std::string &ts_str_begin, const std::string ts_str_end);
    static time_t parse_time(const char * time);

private:

    std::unordered_map<std::string, std::unordered_map<std::string, size_t>> _estimation;

    void parcing(std::istream &istream);

    std::vector<event> _vec_events;

    static bool cmp(const event &a, const event &b);

    std::string find_with_positive(std::unordered_map<std::string, int> &map);

    void update_links(const std::string &rich_id, const std::string &debt_id, int debt_to_rich,
                      std::unordered_map<std::string, size_t> &auxiliary_map);


    std::vector<std::string> make_offers();
};

#endif //LUNCH_CALCULATOR_LUNCH_CALCULATOR_H
