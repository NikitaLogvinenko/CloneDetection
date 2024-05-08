#include "UserAction.hpp"

std::unordered_map<int, User>::iterator ContainerUsers::begin(){
    return users.begin();
}
std::unordered_map<int, User>::iterator ContainerUsers::end(){
    return users.end();
}


const User* ContainerUsers::findUser(int user_id) const{
    const User* ptr_user = nullptr;
    try {
        ptr_user = &users.at(user_id);
    } catch (std::out_of_range& ex) {
        ptr_user = nullptr;
    }
    return ptr_user;
}

void ContainerUsers::addUsers (int first_id, int second_id){
    if (findUser(first_id) == nullptr){
        User first_user(first_id);
        users.insert({first_id, first_user});
    }
    users.at(first_id).addFriend(second_id);
    if (findUser(second_id) == nullptr){
        User second_user(second_id);
        users.insert({second_id, second_user});
    }
    users.at(second_id).addFriend(first_id);
}



std::set<int> recommendFriend(ContainerUsers& users, int user_id, int in_koef){
    const auto& user = *users.findUser(user_id);
    int coeff = std::clamp(in_koef, 0, 100);
    int count_communication = user.getFriendsId().size()*coeff/100;
    
    std::set <int> rec_friend;
    std::map <int, int> friend_friend;

    for (auto friend_id : user.getFriendsId()){
        auto friend_ = users.findUser(friend_id);
        for (auto friend_friend_id : friend_->getFriendsId()){
            ++friend_friend[friend_friend_id];
        }
    }
    
    std::transform(friend_friend.begin(), friend_friend.end(), std::inserter(rec_friend, rec_friend.end()),
        [&count_communication](const std::pair<int,int> pair){
            if(pair.second >= count_communication)
                return pair.first;
            else return 0;
    });
    for (auto friend_id : user.getFriendsId()){
        rec_friend.erase(friend_id);
    }
    rec_friend.erase(0);
    rec_friend.erase(user.getId());
    return rec_friend;
}
