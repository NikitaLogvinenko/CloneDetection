#include "mark_up.hpp"
#include <iostream>
#include <map>
#include <istream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>


std::string replace(std::string str, const std::string &from, const std::string &to) {
    size_t start_pos = str.find(from);
    while (start_pos != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
        start_pos = str.find(from, start_pos);
    }
    return str;
}

html_markup::html_markup(std::istream &voc_stream) {
    while (!voc_stream.eof()) {
        std::string s;
        getline(voc_stream, s, '\n');
        vocab[s.substr(0, s.find_first_of(':') - 1)] = s.substr(s.find_first_of(':') + 2, s.size() - 1);
    }
}

void html_markup::mark_up(std::istream &inp_stream, std::ofstream &of) {
    std::unordered_map<std::string, std::string> specials{
            {"&", "&amp;"},
            { "<", "&lt;" },
            { ">", "&gt;" },
            { "\"", "&quot;" },
            { "~", "&sim;" }
    };
    std::string word;
    int bias;
    while (!inp_stream.eof()) {
        std::string line;
        std::getline(inp_stream, line);
        std::stringstream ss(line);
        while (ss >> word) {
            bias = 0;
            while ((bias < word.size()) && !isalnum(word[word.size() - bias - 1])) {
                bias++;

            }
            std::string semantic = word.substr(0, word.size() - bias);
            std::string punct = word.substr(word.size() - bias, word.size());

            if (vocab.find(semantic) != vocab.end()) {
                std::string semantic_tmp = semantic;
                for(auto &it: specials){
                    semantic_tmp = replace(semantic_tmp, it.first, it.second);
                }
                std::string tmp =
                        "<span style = \"color: #" + vocab[semantic] + "\">" + semantic_tmp + "</span>" + punct + " ";
                of << tmp;
            } else {

                for(auto &it: specials){
                    word = replace(word, it.first, it.second);
                }
                of << word + " ";

            }
        }
        of << "<br>" << std::endl;
    }
}

