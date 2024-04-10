#pragma once
#include "gtest_pch.h"
#include "count_vector.h"
#include "cv_similarity_with_indices.h"

template <size_t CountVectorDimension>
void assert_equal_count_vectors(
	const cm::count_vector<CountVectorDimension>& first_cv,
	const cm::count_vector<CountVectorDimension>& second_cv)
{
	for (size_t count_value_index = 0; count_value_index < CountVectorDimension; ++count_value_index)
	{
		ASSERT_EQ(first_cv[cm::internal::index_of_counted_value(count_value_index)].value(),
		          second_cv[cm::internal::index_of_counted_value(count_value_index)].value());
	}
}

inline void assert_equal_cm_similar_vectors_data(
	const cm::internal::cv_similarity_with_indices& first_data,
	const cm::internal::cv_similarity_with_indices& second_data)
{
	ASSERT_EQ(first_data.relative_similarity(), second_data.relative_similarity());
	ASSERT_EQ(first_data.index_of_first_vector().to_size_t(), second_data.index_of_first_vector().to_size_t());
	ASSERT_EQ(first_data.index_of_second_vector().to_size_t(), second_data.index_of_second_vector().to_size_t());
}
