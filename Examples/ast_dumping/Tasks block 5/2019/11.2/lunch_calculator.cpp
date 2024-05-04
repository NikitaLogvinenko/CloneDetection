#include "lunch_calculator.h"
static const std::string dateTimeFormat{ "%Y-%m-%dT%H:%M:%S" };
using namespace std;

void lunch_calculator::parcing(std::istream &istream) {
    string str;
    int money, i = 0;
    vector<int> vec_moneys;
    string user_id;
    vector<string> _user_id;
    time_t ts;
    std::tm dt;
    while (!istream.eof()) {
        getline(istream, str);
        size_t str_time_pos=str.find_first_of(' ',0);
        if(str_time_pos==string::npos) continue;
        string str_time=str.substr(0,str_time_pos);
        str=str.substr(str_time_pos+1);
        if (str.empty()) continue;
        stringstream str_stream = stringstream(str);
        istringstream str_stream_time {str_time};
        if (!(str_stream_time >> std::get_time(&dt, dateTimeFormat.c_str()))) continue;
        ts=std::mktime(&dt);
        if (!(str_stream >> user_id)) continue;
        if (!(str_stream >> money)) continue;
        auto ev_it = find_if(_vec_events.begin(), _vec_events.end(), [ts](event &a) { return a.time() == ts; });
        if (ev_it == _vec_events.end()) {
            event _new(ts);
            _new.add_info(money, user_id);
            _vec_events.push_back(_new);
        } else {
            ev_it->add_info(money, user_id);
        }
    }
    for(auto &it:_vec_events)
    {
        it.availability_check();
    }

}
time_t lunch_calculator::parse_time(const char *time) {
    // Converts UTC time string to a time_t value.
    std::istringstream ss{ time };
    std::tm dt;
    ss >> std::get_time(&dt, dateTimeFormat.c_str());
    return std::mktime(&dt);
}
std::vector<std::string> lunch_calculator::to_offer_advice(const std::string &ts_str_begin,const std::string ts_str_end) {
    if(_vec_events.empty())
        return std::vector<std::string>();
    _estimation.clear();
    time_t ts_begin=parse_time(ts_str_begin.c_str());
    time_t ts_end=parse_time(ts_str_end.c_str());
    sort(_vec_events.begin(), _vec_events.end(), cmp);
    std::vector<event> auxiliary_vec_events;
    if(ts_end<_vec_events[0].time()||ts_begin>_vec_events[_vec_events.size()-1].time()) return vector<string>();
    if(ts_begin<_vec_events[0].time()) ts_begin=_vec_events[0].time();
    if(ts_end>_vec_events[_vec_events.size()-1].time()) ts_end=_vec_events[_vec_events.size()-1].time();
    for(auto &ev_it:_vec_events)
    {
        if(ev_it.time()>=ts_begin&&ev_it.time()<=ts_end)
        {
            if(ev_it.is_availble()) auxiliary_vec_events.push_back(ev_it);
        }
    }
    string rich_name;
    std::unordered_map<std::string, size_t> auxiliary_map;
    for (const auto &it:auxiliary_vec_events) {
        unordered_map<string, int> map_userID_money = it.get_map_userID_money();
        rich_name = find_with_positive(map_userID_money);
        auto row_it = _estimation.find(rich_name);
        auto rich_it = map_userID_money.find(rich_name);
        if (row_it == _estimation.end()) {
            for (const auto &debt_it:map_userID_money) {
                if (debt_it.first == rich_it->first) continue;
                auxiliary_map.emplace(debt_it.first, abs(debt_it.second));
                update_links(rich_it->first, debt_it.first, debt_it.second, auxiliary_map);
            }
            _estimation.emplace(rich_it->first, auxiliary_map);
            auxiliary_map.clear();
        } else {
            for (const auto &debt_it:map_userID_money) {
                if (debt_it.first == rich_it->first) continue;
                auto debt_est_it = row_it->second.find(debt_it.first);
                if (debt_est_it == row_it->second.end()) {
                    auxiliary_map.emplace(debt_it.first, abs(debt_it.second));
                    update_links(rich_it->first, debt_it.first, debt_it.second, auxiliary_map);
                } else {
                    debt_est_it->second = debt_est_it->second + abs(debt_it.second);
                }
            }
            for (const auto &aux_it:auxiliary_map) {
                row_it->second.emplace(aux_it.first, aux_it.second);
            }
            auxiliary_map.clear();
        }
    }
    return make_offers();
}

std::vector<std::string> lunch_calculator::make_offers() {
    vector<string> str;
    for (const auto &it:_estimation) {

        for (const auto &colum:it.second) {
            stringstream ss;
            ss << colum.first << " -> " << it.first << " - " << colum.second/100 << " rub. "<<colum.second%100<<" pen.\n";
            str.push_back(ss.str());

        }
    }
    return str;
}

bool lunch_calculator::cmp(const event &a, const event &b) {
    return a.time() < b.time();
}

string lunch_calculator::find_with_positive(unordered_map<string, int> &map) {
    for (const auto &it:map) {
        if (it.second > 0) return it.first;
    }
    return string();
}

void lunch_calculator::update_links(const std::string &rich_id, const std::string &debt_id, int debt_to_rich,
                                    std::unordered_map<std::string, size_t> &auxiliary_map) {
    auto aux_it = auxiliary_map.find(debt_id);
    auto row = _estimation.find(debt_id);
    if (row == _estimation.end()) return;
    auto colum = row->second.find(rich_id);
    if (colum == row->second.end()) return;
    int summ = 0;
    summ = colum->second + debt_to_rich;
    if (summ == 0) {
        row->second.erase(colum);
        auxiliary_map.erase(aux_it);
    } else if (summ < 0) {
        row->second.erase(colum);
        aux_it->second = (abs(summ));

    } else {
        colum->second = summ;
        auxiliary_map.erase(aux_it);
    }

}
