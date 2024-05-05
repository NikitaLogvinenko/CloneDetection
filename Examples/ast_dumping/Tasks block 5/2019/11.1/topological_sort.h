#ifndef ADDICTION_SORT_TOPOLOGICAL_SORT_H
#define ADDICTION_SORT_TOPOLOGICAL_SORT_H

#include <vector>
#include <stack>
#include <unordered_set>
#include <string>
#include<list>

class dependence_graph {
public:
    dependence_graph() = default;

    ~dependence_graph() = default;

    bool add_knot(const std::string &_knot, const std::vector<std::string> &dependence_list);

    std::vector<std::string> topological_sort_by_Tarjan();

private:
    std::vector<std::vector<bool>> adjacency_matrix;
    std::vector<std::string> knots;

    void vector_resize();

    size_t check_for_availability_of_knot(const std::string &_knot);

    size_t find_next(size_t i, size_t j);

    bool is_grey_or_black(std::list<size_t> &list, size_t knot_pos);
};


#endif //ADDICTION_SORT_TOPOLOGICAL_SORT_H