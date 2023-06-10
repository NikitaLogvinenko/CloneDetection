#pragma once
#include "gtest_pch.h"
#include "count_vector.h"

template <size_t CountVectorDimension>
void assert_equal_count_vectors(
	const count_matrix::count_vector<CountVectorDimension>& first_cv, 
	const count_matrix::count_vector<CountVectorDimension>& second_cv)
{
	for (size_t count_value_index = 0; count_value_index < CountVectorDimension; ++count_value_index)
	{
		ASSERT_EQ(first_cv[count_matrix::index_of_count_value(count_value_index)].value(), second_cv[count_matrix::index_of_count_value(count_value_index)].value());
	}
}