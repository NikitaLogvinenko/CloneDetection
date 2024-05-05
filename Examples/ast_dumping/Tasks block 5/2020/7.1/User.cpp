#include "User.hpp"

User::User (int id_){
    id = id_;
}

const std::set<int>& User::getFriendsId() const{
    return friends_id;
}

int User::getId() const{
    return id;
}

void User::addFriend (int friend_id){
    friends_id.insert(friend_id);
}
