#include "gtest_pch.h"
#include "count_matrices_similarity_data.h"
#include "count_matrix_tests_miscellanious.h"

using namespace cm;
using namespace cm::internal;

TEST(index_of_count_vectors_pair_test, ctor_default)
{
	constexpr index_of_count_vectors_pair default_index{};
	ASSERT_EQ(default_index.to_size_t(), 0);
}

TEST(index_of_count_vectors_pair_test, ctor_with_index)
{
	constexpr size_t index = 42;
	const index_of_count_vectors_pair index_of_count_vectors_pair(index);
	ASSERT_EQ(index_of_count_vectors_pair.to_size_t(), index);
}



TEST(count_matrices_similarity_data_test, ctor_default)
{
	const count_matrices_similarity_data default_data{};
	ASSERT_EQ(default_data.matrices_relative_similarity(), relative_similarity{});
	ASSERT_EQ(default_data.similar_cv_pairs_count(), 0);
}

TEST(count_matrices_similarity_data_test, ctor_only_similarity)
{
	const relative_similarity similarity{ 0.5 };
	const count_matrices_similarity_data cm_similarity_data{similarity};
	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity(), similarity);
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), 0);
}

TEST(count_matrices_similarity_data_test, ctor_with_empty_vector)
{
	const relative_similarity cm_similarity{ 0.5 };

	const count_matrices_similarity_data cm_similarity_data{ cm_similarity, {} };

	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity(), cm_similarity);
	ASSERT_EQ(cm_similarity_data.begin(), cm_similarity_data.end());
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), 0);
}

namespace
{
	std::vector<cm_similar_vectors_data> create_similar_cv_ascending_order()
	{
		const cm_similar_vectors_data first_pair_data{ index_of_count_vector{0}, index_of_count_vector{1}, relative_similarity{0.1} };
		const cm_similar_vectors_data second_pair_data{ index_of_count_vector{2}, index_of_count_vector{3}, relative_similarity{0.2} };
		const cm_similar_vectors_data third_pair_data{ index_of_count_vector{4}, index_of_count_vector{5}, relative_similarity{0.3} };
		const cm_similar_vectors_data fourth_pair_data{ index_of_count_vector{6}, index_of_count_vector{7}, relative_similarity{0.4} };

		return std::vector{ first_pair_data, second_pair_data, third_pair_data, fourth_pair_data };
	}

	std::vector<cm_similar_vectors_data> create_similar_cv_descending_order()
	{
		const cm_similar_vectors_data first_pair_data{ index_of_count_vector{0}, index_of_count_vector{1}, relative_similarity{0.4} };
		const cm_similar_vectors_data second_pair_data{ index_of_count_vector{2}, index_of_count_vector{3}, relative_similarity{0.3} };
		const cm_similar_vectors_data third_pair_data{ index_of_count_vector{4}, index_of_count_vector{5}, relative_similarity{0.2} };
		const cm_similar_vectors_data fourth_pair_data{ index_of_count_vector{6}, index_of_count_vector{7}, relative_similarity{0.1} };

		return std::vector{ first_pair_data, second_pair_data, third_pair_data, fourth_pair_data };
	}

	void compare_cm_data_from_ascending_vector_dont_check_index(const std::vector<cm_similar_vectors_data>& ascending_cv_vector, 
		const count_matrices_similarity_data& cm_sim_data)
	{
		for (size_t initial_pair_index = 0; initial_pair_index < ascending_cv_vector.size(); ++initial_pair_index)
		{
			const auto& pair_from_vector = ascending_cv_vector[initial_pair_index];
			const auto& pair_from_cm_sim_data = cm_sim_data.get_similar_vectors_data_dont_check_index(
				index_of_count_vectors_pair(cm_sim_data.similar_cv_pairs_count() - initial_pair_index - 1));
			assert_equal_cm_similar_vectors_data(pair_from_vector, pair_from_cm_sim_data);
		}
	}

	void compare_cm_data_from_ascending_vector_check_index(const std::vector<cm_similar_vectors_data>& ascending_cv_vector, 
		const count_matrices_similarity_data& cm_sim_data)
	{
		for (size_t initial_pair_index = 0; initial_pair_index < ascending_cv_vector.size(); ++initial_pair_index)
		{
			const auto& pair_from_vector = ascending_cv_vector[initial_pair_index];
			const auto& pair_from_cm_sim_data = cm_sim_data.get_similar_vectors_data(
				index_of_count_vectors_pair(cm_sim_data.similar_cv_pairs_count() - initial_pair_index - 1));
			assert_equal_cm_similar_vectors_data(pair_from_vector, pair_from_cm_sim_data);
		}
	}

	
}

TEST(count_matrices_similarity_data_test, ctor_initial_descending_order)
{
	const relative_similarity cm_similarity{ 0.5 };
	const std::vector similar_vectors_data_descending_order{ create_similar_cv_descending_order() };
	const count_matrices_similarity_data cm_similarity_data{ cm_similarity, similar_vectors_data_descending_order };

	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity(), cm_similarity);

	for (size_t initial_pair_index = 0; initial_pair_index < similar_vectors_data_descending_order.size(); ++initial_pair_index)
	{
		const auto& pair_from_vector = similar_vectors_data_descending_order[initial_pair_index];
		const auto& pair_from_cm_sim_data = cm_similarity_data.get_similar_vectors_data_dont_check_index(index_of_count_vectors_pair(initial_pair_index));
		assert_equal_cm_similar_vectors_data(pair_from_vector, pair_from_cm_sim_data);
	}
}

TEST(count_matrices_similarity_data_test, ctor_initial_ascending_order)
{
	const relative_similarity cm_similarity{ 0.5 };
	const std::vector similar_vectors_data_ascending_order{ create_similar_cv_ascending_order() };
	const count_matrices_similarity_data cm_similarity_data{ cm_similarity, similar_vectors_data_ascending_order };
	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity(), cm_similarity);
	compare_cm_data_from_ascending_vector_dont_check_index(similar_vectors_data_ascending_order, cm_similarity_data);
}

TEST(count_matrices_similarity_data_test, matrices_relative_similarity)
{
	ASSERT_EQ(count_matrices_similarity_data{}.matrices_relative_similarity(), relative_similarity{});
	const relative_similarity cm_similarity{ 0.5 };
	const count_matrices_similarity_data cm_similarity_data_without_cv{ cm_similarity };
	ASSERT_EQ(cm_similarity_data_without_cv.matrices_relative_similarity(), cm_similarity);
	const count_matrices_similarity_data cm_similarity_data{ cm_similarity, {} };
	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity(), cm_similarity);
}

TEST(count_matrices_similarity_data_test, similar_cv_pairs_count)
{
	ASSERT_EQ(count_matrices_similarity_data{}.similar_cv_pairs_count(), 0);

	const relative_similarity cm_similarity{ 0.5 };
	const count_matrices_similarity_data cm_similarity_data_without_cv{ cm_similarity };
	ASSERT_EQ(cm_similarity_data_without_cv.similar_cv_pairs_count(), 0);

	const std::vector cv_similarities_data{ cm_similar_vectors_data{}, cm_similar_vectors_data{}, cm_similar_vectors_data{} };
	const count_matrices_similarity_data cm_similarity_data{ cm_similarity, cv_similarities_data };
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cv_similarities_data.size());
}

TEST(count_matrices_similarity_data_test, get_similar_vectors_data_dont_check_index)
{
	const relative_similarity cm_similarity{ 0.5 };
	const std::vector similar_vectors_data_ascending_order{ create_similar_cv_ascending_order() };
	const count_matrices_similarity_data cm_similarity_data{ cm_similarity, similar_vectors_data_ascending_order };
	compare_cm_data_from_ascending_vector_dont_check_index(similar_vectors_data_ascending_order, cm_similarity_data);
}

TEST(count_matrices_similarity_data_test, get_similar_vectors_data)
{
	const relative_similarity cm_similarity{ 0.5 };
	const std::vector similar_vectors_data_ascending_order{ create_similar_cv_ascending_order() };
	const count_matrices_similarity_data cm_similarity_data{ cm_similarity, similar_vectors_data_ascending_order };
	compare_cm_data_from_ascending_vector_check_index(similar_vectors_data_ascending_order, cm_similarity_data);
}

TEST(count_matrices_similarity_data_test, get_similar_vectors_data_invalid_index)
{
	const relative_similarity cm_similarity{ 0.5 };
	const std::vector similar_vectors_data_ascending_order{ create_similar_cv_ascending_order() };
	const count_matrices_similarity_data cm_similarity_data{ cm_similarity, similar_vectors_data_ascending_order };
	ASSERT_THROW(const auto & _ = cm_similarity_data.get_similar_vectors_data(
		index_of_count_vectors_pair(cm_similarity_data.similar_cv_pairs_count())), std::out_of_range);
}

TEST(count_matrices_similarity_data_test, iterators)
{
	const count_matrices_similarity_data default_cm_sim_data{};
	ASSERT_EQ(default_cm_sim_data.begin(), default_cm_sim_data.end());

	const count_matrices_similarity_data cm_similarity_data{ {}, create_similar_cv_descending_order() };

	size_t forward_index = 0;
	for (auto iterator = cm_similarity_data.begin(); iterator != cm_similarity_data.end(); ++iterator)
	{
		const auto& pair_by_index = cm_similarity_data.get_similar_vectors_data_dont_check_index(index_of_count_vectors_pair(forward_index));
		assert_equal_cm_similar_vectors_data(*iterator, pair_by_index);
		ASSERT_EQ(cm_similarity_data.begin() + forward_index, iterator);
		ASSERT_EQ(cm_similarity_data.end() - (cm_similarity_data.similar_cv_pairs_count() - forward_index), iterator);
		++forward_index;
	}

	size_t backward_index = cm_similarity_data.similar_cv_pairs_count() - 1;
	for (auto iterator = cm_similarity_data.end(); iterator > cm_similarity_data.begin();)
	{
		--iterator;
		const auto& pair_by_index = cm_similarity_data.get_similar_vectors_data_dont_check_index(index_of_count_vectors_pair(backward_index));
		assert_equal_cm_similar_vectors_data(*iterator, pair_by_index);
		--backward_index;
	}
	ASSERT_EQ(cm_similarity_data.begin() + cm_similarity_data.similar_cv_pairs_count(), cm_similarity_data.end());
	ASSERT_EQ(cm_similarity_data.begin(), cm_similarity_data.end() - cm_similarity_data.similar_cv_pairs_count());

}
