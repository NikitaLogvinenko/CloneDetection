#ifndef User_hpp
#define User_hpp

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

class User final{
public:
    User (int id_);
    
    const std::set<int>& getFriendsId() const;
    int getId() const;
    void addFriend (int friend_id);
    
    
private:
    int id;
    std::set<int> friends_id;
};

#endif /* User_hpp */
