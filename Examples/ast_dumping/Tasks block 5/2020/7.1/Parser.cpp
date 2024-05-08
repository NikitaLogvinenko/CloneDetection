#include "Parser.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>



ContainerUsers parser (std::istream &stream){
    ContainerUsers users;
    std::string string;
    
    while (!stream.eof()){
        getline (stream, string);
        int first_user_id = 0, second_user_id = 0;
        int user_number = 1;
        bool set_first_user_id = false;
        bool set_second_user_id = false;
        std::stringstream line;
        line << string;
       
        while(!line.eof()){
            std::string user_ = "user_";
            std::string _id = "_id";
            std::string number_id;
            std::string word;
            line >> word;
            
            auto iter_start_id = std::search(word.begin(), word.end(), user_.begin(), user_.end());
            if (iter_start_id == word.end())
                continue;
            for(int i = 0; i < user_.size(); ++i)
                ++iter_start_id;
            auto iter_end_id = std::search(word.begin(), word.end(), _id.begin(), _id.end());
            if (iter_end_id == word.end())
                continue;
            std::copy(iter_start_id, iter_end_id, std::back_inserter(number_id));
            switch (user_number) {
                case 1:
                    first_user_id = stoi(number_id);
                    set_first_user_id = true;
                    break;
                case 2:
                    second_user_id = stoi(number_id);
                    set_second_user_id = true;
                    break;
            }
            ++user_number;
        }
        
        if(set_first_user_id && set_second_user_id)
            users.addUsers(first_user_id, second_user_id);
        set_first_user_id = false;
        set_second_user_id = false;
        string.clear();
    }
    
    return users;
}


