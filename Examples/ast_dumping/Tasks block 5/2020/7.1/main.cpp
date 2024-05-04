#include <iostream>
#include <fstream>
#include "Parser.hpp"
#include "User.hpp"
#include "UserAction.hpp"
#include <cstdlib>

int main(int argc, const char * argv[]) {
    
    std::ifstream file;
    file.open("Users.txt");
    
    if(!file.is_open()){
        std::cout << "File not open!" << std::endl;
    }
    
    ContainerUsers users = parser(file);
    
    
    for(auto user = users.begin(); user != users.end(); ++user){
        std::cout << "user - " << user->first << " friends: ";
        for(auto friend_id: user->second.getFriendsId()){
            std::cout << friend_id << " ";
        }
        std::cout << std::endl << "Recomend friends: ";
        for(auto recomend_id : recommendFriend(users, user->first, 100)){
            std::cout << recomend_id << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}




