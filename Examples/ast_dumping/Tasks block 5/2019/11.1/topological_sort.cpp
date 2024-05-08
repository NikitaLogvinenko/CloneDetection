
#include "topological_sort.h"

using namespace std;

bool dependence_graph::add_knot(const string &_knot, const vector<string> &dependence_list) {
    if (dependence_list.empty()) return false;
    size_t knot_pos = 0, dependence_pos = 0;
    knot_pos = check_for_availability_of_knot(_knot);
    if (knot_pos == -1) {
        knots.push_back(_knot);
        vector_resize();
        knot_pos = knots.size() - 1;
    }
    for (auto it:dependence_list) {
        dependence_pos = check_for_availability_of_knot(it);
        if (dependence_pos != -1) {
            adjacency_matrix[knot_pos][dependence_pos] = true;
        } else {
            knots.push_back(it);
            vector_resize();
            adjacency_matrix[knot_pos][knots.size() - 1] = true;
        }
    }
    return true;
}

void dependence_graph::vector_resize() {
    for (auto it = adjacency_matrix.begin(); it != adjacency_matrix.end(); it++) {
        it->push_back(false);
    }
    vector<bool> New(knots.size());
    fill(New.begin(), New.end(), false);
    adjacency_matrix.push_back(New);
}

size_t dependence_graph::check_for_availability_of_knot(const string &_knot) {
    for (int i = 0; i < knots.size(); i++) {
        if (knots[i] == _knot) {
            return i;
        }
    }
    return -1;
}

std::vector<std::string> dependence_graph::topological_sort_by_Tarjan() {
    if (adjacency_matrix.empty() || knots.empty()) return vector<string>();
    list<size_t> grey_list;
    list<size_t> black_list;
    stack<size_t> line_list;
    size_t vec_pos = 0, dependence_pos = 0;
    line_list.push(vec_pos);
    grey_list.push_back(vec_pos);
    while (black_list.size() != adjacency_matrix.size()) {
        dependence_pos = find_next(vec_pos, dependence_pos);
        if (is_grey_or_black(black_list, dependence_pos)) continue;
        if (is_grey_or_black(grey_list, dependence_pos)) return vector<string>();
        if (dependence_pos == -1) {
            black_list.push_back(vec_pos);
            if(!grey_list.empty())
            grey_list.pop_back();
            dependence_pos = vec_pos;
            vec_pos = line_list.top();
            line_list.pop();
            continue;
        } else {
            grey_list.push_back(dependence_pos);
            line_list.push(vec_pos);
            vec_pos = dependence_pos;
            dependence_pos = 0;
            continue;
        }
    }
    vector<string> vec_str;
    for (auto it:black_list) {
        vec_str.push_back(knots[it]);
    }
    return vec_str;
}

size_t dependence_graph::find_next(size_t i, size_t j) {
    j++;
    for (j; j < adjacency_matrix[i].size(); j++) {
        if (adjacency_matrix[i][j]) return j;
    }
    return -1;
}

bool dependence_graph::is_grey_or_black(std::list<size_t> &list, size_t knot_pos) {
    for (auto it:list) {
        if (it == knot_pos) return true;
    }
    return false;
}