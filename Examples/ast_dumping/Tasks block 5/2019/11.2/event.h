#ifndef LUNCH_CALCULATOR_EVENT_H
#define LUNCH_CALCULATOR_EVENT_H

#include <vector>
#include <string>
#include <unordered_map>

class event {
public:
    inline explicit event(time_t _ts) : _ts{_ts} {
        _is_the_amount_correct = false;
    }

    ~event() = default;

    void add_info(int money, std::string &user_id);

    inline bool is_availble() const {
        return _is_the_amount_correct;
    }

    inline const time_t time() const {
        return _ts;
    }

    const std::unordered_map<std::string,int> get_map_userID_money() const {
        return _map_userID_money;

    }
    bool availability_check();
private:
    std::unordered_map<std::string,int> _map_userID_money;
    time_t _ts;
    bool _is_the_amount_correct;

};


#endif //LUNCH_CALCULATOR_EVENT_H
