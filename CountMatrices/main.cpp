#include "count_vector.h"
#include <iostream>
#include "dlib/optimization/max_cost_assignment.h"
int main()
{
	std::array<count_vector_value, 5> arr{};
	for (size_t value = 0; value < 5; ++value)
	{
		arr[value] = count_vector_value(value);
	}
	const count_vector vector(arr);
	for (const auto& value : vector)
	{
		std::cout << value.value() << "\n";
	}
	std::cout << vector.begin()->value() << std::endl;

	dlib::matrix<size_t> cost(3, 3);
	cost = 1, 2, 6,
		5, 3, 6,
		4, 5, 0;
	std::vector<long> assignment = max_cost_assignment(cost);
	for (unsigned int i = 0; i < assignment.size(); i++)
		std::cout << assignment[i] << " ";
	std::cout << "\noptimal cost: " << assignment_cost(cost, assignment) << std::endl;

	return 0;
}