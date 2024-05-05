#ifndef UserAction_hpp
#define UserAction_hpp

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include "User.hpp"
#include <unordered_map>

class ContainerUsers final{
public:
    std::unordered_map<int, User>::iterator begin();
    std::unordered_map<int, User>::iterator end();
    
    void addUsers (int first_id, int second_id);
    
    const User* findUser(int user_id) const;
    
private:
    std::unordered_map<int, User> users;
};

std::set<int> recommendFriend(ContainerUsers& users, int user_id, int in_koef);

#endif /* UserAction_hpp */
