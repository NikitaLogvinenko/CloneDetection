#include "event.h"

void event::add_info(int money, std::string &user_id)  {
    if(user_id.empty()) return;
    _map_userID_money.emplace(user_id,money);
    if (_map_userID_money.empty()) {
        _is_the_amount_correct = false;
    } else {
        availability_check();
    }
}

bool event::availability_check() {
    _is_the_amount_correct = true;
    int64_t summ = 0;
    for (auto  it:_map_userID_money) {
        if (it.second > 0) summ++;
        if (summ > 1) {
            _is_the_amount_correct = false;
            return _is_the_amount_correct;
        }
    }
    if(_map_userID_money.size()==1)
    {
        _is_the_amount_correct = false;
        return _is_the_amount_correct;
    }
    for (auto it:_map_userID_money) {
        summ += it.second;
    }
    if (summ <= 0) _is_the_amount_correct = false;
    return _is_the_amount_correct;
}
