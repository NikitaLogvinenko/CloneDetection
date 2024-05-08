#ifndef MARKUP_HTML_HTML_HPP
#define MARKUP_HTML_HTML_HPP

#include <string>
#include <map>

class html_markup {
    std::map<std::string, std::string> vocab;

public:
    html_markup(std::istream &voc_stream);

    void mark_up(std::istream &inp_stream, std::ofstream &of);
};

#endif //MARKUP_HTML_HTML_HPP
