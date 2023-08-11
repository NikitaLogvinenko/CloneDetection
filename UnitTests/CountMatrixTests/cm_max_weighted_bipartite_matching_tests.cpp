#include "gtest_pch.h"
#include "cm_max_weighted_bipartite_matching.h"

using namespace cm;
using namespace cm::internal;

class cm_max_weighted_bipartite_matching_dim3_test_f : public ::testing::Test {
protected:
	std::array<count_vector_value, 3> values_1_1_{};
	std::array<count_vector_value, 3> values_2_1_{};
	std::array<count_vector_value, 3> values_3_1_{};
	std::array<count_vector_value, 3> values_4_1_{};

	std::array<count_vector_value, 3> values_1_2_{};
	std::array<count_vector_value, 3> values_2_2_{};
	std::array<count_vector_value, 3> values_3_2_{};
	std::array<count_vector_value, 3> values_4_2_{};


	count_vector<3> cv_1_1_{};
	count_vector<3> cv_2_1_{};
	count_vector<3> cv_3_1_{};
	count_vector<3> cv_4_1_{};

	count_vector<3> cv_1_2_{};
	count_vector<3> cv_2_2_{};
	count_vector<3> cv_3_2_{};
	count_vector<3> cv_4_2_{};


	std::vector<count_vector<3>> count_vectors_1_{};
	std::vector<count_vector<3>> count_vectors_2_{};
	std::vector<count_vector<3>> count_vectors_2_incomplete_{};
	std::vector<count_vector<3>> count_vectors_1_repeated_cv_{};
	std::vector<count_vector<3>> count_vectors_2_repeated_cv_{};

	::cm::count_matrix<3> cm_1_{};
	::cm::count_matrix<3> cm_2_{};
	::cm::count_matrix<3> cm_2_incomplete_{};
	::cm::count_matrix<3> cm_1_repeated_cv_{};
	::cm::count_matrix<3> cm_2_repeated_cv_{};

	::cm::count_matrix<3> cm_1_copy_{};
	std::vector<count_vector<3>> count_vectors_1_reversed_vectors_order_{};
	::cm::count_matrix<3> cm_1_reversed_vectors_order_{};

	::cm::count_matrix<3> cm_empty_1_{};
	::cm::count_matrix<3> cm_empty_2_{};

	void SetUp() override
	{
		values_1_1_ = std::array{ count_vector_value(0), count_vector_value(1), count_vector_value(2) };
		values_2_1_ = std::array{ count_vector_value(3), count_vector_value(4), count_vector_value(5) };
		values_3_1_ = std::array{ count_vector_value(6), count_vector_value(7), count_vector_value(8) };
		values_4_1_ = std::array{ count_vector_value(9), count_vector_value(10), count_vector_value(11) };

		values_1_2_ = std::array{ count_vector_value(1), count_vector_value(2), count_vector_value(3) };
		values_2_2_ = std::array{ count_vector_value(4), count_vector_value(5), count_vector_value(6) };
		values_3_2_ = std::array{ count_vector_value(7), count_vector_value(8), count_vector_value(9) };
		values_4_2_ = std::array{ count_vector_value(10), count_vector_value(11), count_vector_value(12) };


		cv_1_1_ = count_vector{ values_1_1_ };
		cv_2_1_ = count_vector{ values_2_1_ };
		cv_3_1_ = count_vector{ values_3_1_ };
		cv_4_1_ = count_vector{ values_4_1_ };

		cv_1_2_ = count_vector{ values_1_2_ };
		cv_2_2_ = count_vector{ values_2_2_ };
		cv_3_2_ = count_vector{ values_3_2_ };
		cv_4_2_ = count_vector{ values_4_2_ };


		count_vectors_1_ = std::vector{ cv_1_1_, cv_2_1_, cv_3_1_, cv_4_1_ };
		cm_1_ = ::cm::count_matrix{ count_vectors_1_ };

		count_vectors_2_ = std::vector{ cv_1_2_, cv_2_2_, cv_3_2_, cv_4_2_ };
		cm_2_ = ::cm::count_matrix{ count_vectors_2_ };

		count_vectors_2_incomplete_ = std::vector{ cv_2_2_, cv_3_2_, cv_4_2_ };
		cm_2_incomplete_ = ::cm::count_matrix{ count_vectors_2_incomplete_ };

		count_vectors_1_repeated_cv_ = std::vector{ cv_1_1_, cv_1_1_, cv_2_1_, cv_2_1_, cv_3_1_, cv_3_1_, cv_4_1_, cv_4_1_ };
		cm_1_repeated_cv_ = ::cm::count_matrix{ count_vectors_1_repeated_cv_ };

		count_vectors_2_repeated_cv_ = std::vector{ cv_1_2_, cv_1_2_, cv_2_2_, cv_2_2_, cv_3_2_, cv_3_2_, cv_4_2_, cv_4_2_ };
		cm_2_repeated_cv_ = ::cm::count_matrix{ count_vectors_2_repeated_cv_ };

		cm_1_copy_ = cm_1_;
		std::ranges::copy(count_vectors_1_.rbegin(), count_vectors_1_.rend(), std::back_inserter(count_vectors_1_reversed_vectors_order_));
		cm_1_reversed_vectors_order_ = ::cm::count_matrix{ count_vectors_1_reversed_vectors_order_ };
	}
};

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, default_weights_decimals)
{
	ASSERT_EQ(cm_max_weighted_bipartite_matching<3>::default_weights_decimals(), 3);
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, ctor_too_large_weights_decimals)
{
	ASSERT_THROW(cm_max_weighted_bipartite_matching<3>{1000000}, common_exceptions::overflow_exception);
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, two_empty_matrices)
{
	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const auto cm_similarity_data = cm_similarity_alg.determine_similarity(cm_empty_1_, cm_empty_2_);

	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity().to_double(), relative_similarity::max_similarity_value());
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), 0);
	ASSERT_EQ(cm_similarity_data.begin(), cm_similarity_data.end());
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, one_empty_matrix)
{
	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const auto cm_similarity_data = cm_similarity_alg.determine_similarity(cm_empty_1_, cm_1_);

	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity().to_double(), relative_similarity::min_similarity_value());
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), 0);
	ASSERT_EQ(cm_similarity_data.begin(), cm_similarity_data.end());
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, two_identical_matrices)
{
	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const auto cm_similarity_data = cm_similarity_alg.determine_similarity(cm_1_, cm_1_copy_);

	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity().to_double(), relative_similarity::max_similarity_value());
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_1_.vectors_count());

	for (const auto& similar_cv_data : cm_similarity_data)
	{
		ASSERT_EQ(similar_cv_data.relative_similarity().to_double(), relative_similarity::max_similarity_value());
		ASSERT_EQ(similar_cv_data.index_of_first_vector().to_size_t(), similar_cv_data.index_of_second_vector().to_size_t());
	}
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, equal_vectors_but_different_order)
{
	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const auto cm_similarity_data = cm_similarity_alg.determine_similarity(cm_1_, cm_1_reversed_vectors_order_);

	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity().to_double(), relative_similarity::max_similarity_value());
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_1_.vectors_count());

	for (const auto& similar_cv_data : cm_similarity_data)
	{
		ASSERT_EQ(similar_cv_data.relative_similarity().to_double(), relative_similarity::max_similarity_value());
		ASSERT_EQ(similar_cv_data.index_of_first_vector().to_size_t() + similar_cv_data.index_of_second_vector().to_size_t(), cm_1_.vectors_count() - 1);
	}
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, different_matrices_equal_cv_count)
{
	const double distance_1 = std::sqrt(3.0 / 14);
	const double distance_2 = std::sqrt(3.0 / 77);
	const double distance_3 = std::sqrt(3.0 / 194);
	const double distance_4 = std::sqrt(3.0 / 365);

	const double vectors_similarity_1 = 1 - distance_1;
	const double vectors_similarity_2 = 1 - distance_2;
	const double vectors_similarity_3 = 1 - distance_3;
	const double vectors_similarity_4 = 1 - distance_4;
	const std::vector cv_similarities_descending_order{ vectors_similarity_4, vectors_similarity_3, vectors_similarity_2, vectors_similarity_1 };

	const double cm_similarity = (vectors_similarity_1 + vectors_similarity_2 + vectors_similarity_3 + vectors_similarity_4) / 4;

	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const auto cm_similarity_data = cm_similarity_alg.determine_similarity(cm_1_, cm_2_);

	ASSERT_NEAR(cm_similarity_data.matrices_relative_similarity().to_double(), cm_similarity, 1e-3);

	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_1_.vectors_count());

	for (size_t cv_pair_index = 0; cv_pair_index < cm_similarity_data.similar_cv_pairs_count(); ++cv_pair_index)
	{
		const auto& cv_pair = cm_similarity_data.get_similar_vectors_data_dont_check_index(index_of_count_vectors_pair(cv_pair_index));
		ASSERT_NEAR(cv_pair.relative_similarity().to_double(), cv_similarities_descending_order[cv_pair_index], 1e-3);
		ASSERT_EQ(cv_pair.index_of_first_vector().to_size_t(), cm_1_.vectors_count() - 1 - cv_pair_index);
		ASSERT_EQ(cv_pair.index_of_second_vector().to_size_t(), cm_1_.vectors_count() - 1 - cv_pair_index);
	}
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, different_matrices_equal_cv_count_swap_matrices)
{
	const double distance_1 = std::sqrt(3.0 / 14);
	const double distance_2 = std::sqrt(3.0 / 77);
	const double distance_3 = std::sqrt(3.0 / 194);
	const double distance_4 = std::sqrt(3.0 / 365);

	const double vectors_similarity_1 = 1 - distance_1;
	const double vectors_similarity_2 = 1 - distance_2;
	const double vectors_similarity_3 = 1 - distance_3;
	const double vectors_similarity_4 = 1 - distance_4;
	const std::vector cv_similarities_descending_order{ vectors_similarity_4, vectors_similarity_3, vectors_similarity_2, vectors_similarity_1 };

	const double cm_similarity = (vectors_similarity_1 + vectors_similarity_2 + vectors_similarity_3 + vectors_similarity_4) / 4;

	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const auto cm_similarity_data = cm_similarity_alg.determine_similarity(cm_2_, cm_1_);

	ASSERT_NEAR(cm_similarity_data.matrices_relative_similarity().to_double(), cm_similarity, 1e-3);

	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_1_.vectors_count());

	for (size_t cv_pair_index = 0; cv_pair_index < cm_similarity_data.similar_cv_pairs_count(); ++cv_pair_index)
	{
		const auto& cv_pair = cm_similarity_data.get_similar_vectors_data_dont_check_index(index_of_count_vectors_pair(cv_pair_index));
		ASSERT_NEAR(cv_pair.relative_similarity().to_double(), cv_similarities_descending_order[cv_pair_index], 1e-3);
		ASSERT_EQ(cv_pair.index_of_first_vector().to_size_t(), cm_1_.vectors_count() - 1 - cv_pair_index);
		ASSERT_EQ(cv_pair.index_of_second_vector().to_size_t(), cm_1_.vectors_count() - 1 - cv_pair_index);
	}
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, increased_accuracy)
{
	const double distance_1 = std::sqrt(3.0 / 14);
	const double distance_2 = std::sqrt(3.0 / 77);
	const double distance_3 = std::sqrt(3.0 / 194);
	const double distance_4 = std::sqrt(3.0 / 365);

	const double vectors_similarity_1 = 1 - distance_1;
	const double vectors_similarity_2 = 1 - distance_2;
	const double vectors_similarity_3 = 1 - distance_3;
	const double vectors_similarity_4 = 1 - distance_4;
	const std::vector cv_similarities_descending_order{ vectors_similarity_4, vectors_similarity_3, vectors_similarity_2, vectors_similarity_1 };

	const double cm_similarity = (vectors_similarity_1 + vectors_similarity_2 + vectors_similarity_3 + vectors_similarity_4) / 4;

	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{10};
	const auto cm_similarity_data = cm_similarity_alg.determine_similarity(cm_1_, cm_2_);

	ASSERT_NEAR(cm_similarity_data.matrices_relative_similarity().to_double(), cm_similarity, 1e-10);

	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_1_.vectors_count());

	for (size_t cv_pair_index = 0; cv_pair_index < cm_similarity_data.similar_cv_pairs_count(); ++cv_pair_index)
	{
		const auto& cv_pair = cm_similarity_data.get_similar_vectors_data_dont_check_index(index_of_count_vectors_pair(cv_pair_index));
		ASSERT_NEAR(cv_pair.relative_similarity().to_double(), cv_similarities_descending_order[cv_pair_index], 1e-10);
		ASSERT_EQ(cv_pair.index_of_first_vector().to_size_t(), cm_1_.vectors_count() - 1 - cv_pair_index);
		ASSERT_EQ(cv_pair.index_of_second_vector().to_size_t(), cm_1_.vectors_count() - 1 - cv_pair_index);
	}
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, decreased_accuracy)
{
	const double distance_1 = std::sqrt(3.0 / 14);
	const double distance_2 = std::sqrt(3.0 / 77);
	const double distance_3 = std::sqrt(3.0 / 194);
	const double distance_4 = std::sqrt(3.0 / 365);

	const double vectors_similarity_1 = 1 - distance_1;
	const double vectors_similarity_2 = 1 - distance_2;
	const double vectors_similarity_3 = 1 - distance_3;
	const double vectors_similarity_4 = 1 - distance_4;

	const double cm_similarity = (vectors_similarity_1 + vectors_similarity_2 + vectors_similarity_3 + vectors_similarity_4) / 4;

	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{0};
	const auto cm_similarity_data = cm_similarity_alg.determine_similarity(cm_1_, cm_2_);

	ASSERT_NEAR(cm_similarity_data.matrices_relative_similarity().to_double(), cm_similarity, 1e0);

	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_1_.vectors_count());
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, matrices_with_different_count_of_vectors)
{
	constexpr double distance_1 = 1.0; // between first vector in cm_1_ and fourth (fictive) zero-vector in cm_2_incomplete_
	const double distance_2 = sqrt(3.0 / 77);
	const double distance_3 = sqrt(3.0 / 194);
	const double distance_4 = sqrt(3.0 / 365);

	constexpr double vectors_similarity_1 = 1 - distance_1;
	const double vectors_similarity_2 = 1 - distance_2;
	const double vectors_similarity_3 = 1 - distance_3;
	const double vectors_similarity_4 = 1 - distance_4;
	const std::vector cv_similarities_descending_order{ vectors_similarity_4, vectors_similarity_3, vectors_similarity_2 };

	const double cm_similarity = (vectors_similarity_1 + vectors_similarity_2 + vectors_similarity_3 + vectors_similarity_4) / 4;

	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const auto cm_similarity_data = cm_similarity_alg.determine_similarity(cm_1_, cm_2_incomplete_);

	ASSERT_NEAR(cm_similarity_data.matrices_relative_similarity().to_double(), cm_similarity, 1e-3);
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_2_incomplete_.vectors_count());

	for (size_t cv_pair_index = 0; cv_pair_index < cm_similarity_data.similar_cv_pairs_count(); ++cv_pair_index)
	{
		const auto& cv_pair = cm_similarity_data.get_similar_vectors_data_dont_check_index(index_of_count_vectors_pair(cv_pair_index));
		ASSERT_NEAR(cv_pair.relative_similarity().to_double(), cv_similarities_descending_order[cv_pair_index], 1e-3);
		ASSERT_EQ(cv_pair.index_of_first_vector().to_size_t(), cm_1_.vectors_count() - 1 - cv_pair_index);
		ASSERT_EQ(cv_pair.index_of_second_vector().to_size_t(), cm_2_incomplete_.vectors_count() - 1 - cv_pair_index);
	}
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, several_possible_matchings)
{
	const double distance_12 = std::sqrt(3.0 / 14);
	const double distance_34 = std::sqrt(3.0 / 77);
	const double distance_56 = std::sqrt(3.0 / 194);
	const double distance_78 = std::sqrt(3.0 / 365);

	const double vectors_similarity_12 = 1 - distance_12;
	const double vectors_similarity_34 = 1 - distance_34;
	const double vectors_similarity_56 = 1 - distance_56;
	const double vectors_similarity_78 = 1 - distance_78;
	const std::vector cv_similarities_descending_order{ vectors_similarity_78, vectors_similarity_78, vectors_similarity_56, vectors_similarity_56,
		vectors_similarity_34, vectors_similarity_34, vectors_similarity_12, vectors_similarity_12 };

	const double cm_similarity = (vectors_similarity_12 + vectors_similarity_34 + vectors_similarity_56 + vectors_similarity_78) / 4;

	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const auto cm_similarity_data = cm_similarity_alg.determine_similarity(cm_1_repeated_cv_, cm_2_repeated_cv_);

	ASSERT_NEAR(cm_similarity_data.matrices_relative_similarity().to_double(), cm_similarity, 1e-3);

	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_1_repeated_cv_.vectors_count());

	for (size_t cv_pair_index = 0; cv_pair_index < cm_similarity_data.similar_cv_pairs_count(); ++cv_pair_index)
	{
		const auto& cv_pair = cm_similarity_data.get_similar_vectors_data_dont_check_index(index_of_count_vectors_pair(cv_pair_index));
		ASSERT_NEAR(cv_pair.relative_similarity().to_double(), cv_similarities_descending_order[cv_pair_index], 1e-3);
		const int first_similar_cv_index = static_cast<int>(cv_pair.index_of_first_vector().to_size_t());
		const int second_similar_cv_index = static_cast<int>(cv_pair.index_of_second_vector().to_size_t());
		const int max_index_in_similar_cv_pair = std::max(first_similar_cv_index, second_similar_cv_index);
		ASSERT_TRUE(first_similar_cv_index == second_similar_cv_index ||
			std::abs(first_similar_cv_index - second_similar_cv_index) == 1 && max_index_in_similar_cv_pair % 2 == 1);
	}
}

/*==============================================================================================================================================================================*/

class cm_max_weighted_bipartite_matching_zero_dim_test_f : public ::testing::Test {
protected:
	count_vector<0> cv_{};
	
	std::vector<count_vector<0>> four_count_vectors_{};
	std::vector<count_vector<0>> three_count_vectors_{};

	::cm::count_matrix<0> cm_four_cv_{};
	::cm::count_matrix<0> cm_three_cv_{};
								 
	::cm::count_matrix<0> cm_empty_1_{};
	::cm::count_matrix<0> cm_empty_2_{};

	void SetUp() override
	{
		four_count_vectors_ = std::vector{ cv_, cv_, cv_, cv_ };
		cm_four_cv_ = ::cm::count_matrix{ four_count_vectors_ };

		three_count_vectors_ = std::vector{ cv_, cv_, cv_ };
		cm_three_cv_ = ::cm::count_matrix{ three_count_vectors_ };
	}
};

TEST_F(cm_max_weighted_bipartite_matching_zero_dim_test_f, default_weights_decimals)
{
	ASSERT_EQ(cm_max_weighted_bipartite_matching<0>::default_weights_decimals(), 3);
}

TEST_F(cm_max_weighted_bipartite_matching_zero_dim_test_f, ctor_too_large_weights_decimals)
{
	ASSERT_THROW(cm_max_weighted_bipartite_matching<0>{1000000}, common_exceptions::overflow_exception);
}

TEST_F(cm_max_weighted_bipartite_matching_zero_dim_test_f, two_empty_matrices)
{
	const cm_max_weighted_bipartite_matching<0> cm_similarity_alg{};
	const auto cm_similarity_data = cm_similarity_alg.determine_similarity(cm_empty_1_, cm_empty_2_);

	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity().to_double(), relative_similarity::max_similarity_value());
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), 0);
	ASSERT_EQ(cm_similarity_data.begin(), cm_similarity_data.end());
}

TEST_F(cm_max_weighted_bipartite_matching_zero_dim_test_f, one_empty_matrix)
{
	const cm_max_weighted_bipartite_matching<0> cm_similarity_alg{};
	const auto cm_similarity_data = cm_similarity_alg.determine_similarity(cm_empty_1_, cm_four_cv_);

	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity().to_double(), relative_similarity::min_similarity_value());
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), 0);
	ASSERT_EQ(cm_similarity_data.begin(), cm_similarity_data.end());
}

TEST_F(cm_max_weighted_bipartite_matching_zero_dim_test_f, two_identical_matrices)
{
	const cm_max_weighted_bipartite_matching<0> cm_similarity_alg{};
	const auto cm_similarity_data = cm_similarity_alg.determine_similarity(cm_four_cv_, cm_four_cv_);

	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity().to_double(), relative_similarity::max_similarity_value());
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_four_cv_.vectors_count());

	for (const auto& similar_cv_data : cm_similarity_data)
	{
		ASSERT_EQ(similar_cv_data.relative_similarity().to_double(), relative_similarity::max_similarity_value());
		ASSERT_EQ(similar_cv_data.index_of_first_vector().to_size_t(), similar_cv_data.index_of_second_vector().to_size_t());
	}
}

TEST_F(cm_max_weighted_bipartite_matching_zero_dim_test_f, matrices_with_different_count_of_vectors)
{
	const cm_max_weighted_bipartite_matching<0> cm_similarity_alg{};
	const auto cm_similarity_data = cm_similarity_alg.determine_similarity(cm_four_cv_, cm_three_cv_);

	ASSERT_NEAR(cm_similarity_data.matrices_relative_similarity().to_double(), relative_similarity::max_similarity_value(), 1e-3);
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), 3);

	for (size_t cv_pair_index = 0; cv_pair_index < cm_similarity_data.similar_cv_pairs_count(); ++cv_pair_index)
	{
		const auto& cv_pair = cm_similarity_data.get_similar_vectors_data_dont_check_index(index_of_count_vectors_pair(cv_pair_index));
		ASSERT_NEAR(cv_pair.relative_similarity().to_double(), relative_similarity::max_similarity_value(), 1e-3);
	}
}

/*==============================================================================================================================================================================*/

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, two_empty_matrices_through_abstract_class)
{
	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const cm_similarity_abstract_algorithm<3>& ref_to_abstract = cm_similarity_alg;
	const auto cm_similarity_data = ref_to_abstract.determine_similarity(cm_empty_1_, cm_empty_2_);

	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity().to_double(), relative_similarity::max_similarity_value());
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), 0);
	ASSERT_EQ(cm_similarity_data.begin(), cm_similarity_data.end());
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, one_empty_matrix_through_abstract_class)
{
	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const cm_similarity_abstract_algorithm<3>& ref_to_abstract = cm_similarity_alg;
	const auto cm_similarity_data = ref_to_abstract.determine_similarity(cm_empty_1_, cm_1_);

	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity().to_double(), relative_similarity::min_similarity_value());
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), 0);
	ASSERT_EQ(cm_similarity_data.begin(), cm_similarity_data.end());
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, two_identical_matrices_through_abstract_class)
{
	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const cm_similarity_abstract_algorithm<3>& ref_to_abstract = cm_similarity_alg;
	const auto cm_similarity_data = ref_to_abstract.determine_similarity(cm_1_, cm_1_copy_);

	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity().to_double(), relative_similarity::max_similarity_value());
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_1_.vectors_count());

	for (const auto& similar_cv_data : cm_similarity_data)
	{
		ASSERT_EQ(similar_cv_data.relative_similarity().to_double(), relative_similarity::max_similarity_value());
		ASSERT_EQ(similar_cv_data.index_of_first_vector().to_size_t(), similar_cv_data.index_of_second_vector().to_size_t());
	}
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, equal_vectors_but_different_order_through_abstract_class)
{
	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const cm_similarity_abstract_algorithm<3>& ref_to_abstract = cm_similarity_alg;
	const auto cm_similarity_data = ref_to_abstract.determine_similarity(cm_1_, cm_1_reversed_vectors_order_);

	ASSERT_EQ(cm_similarity_data.matrices_relative_similarity().to_double(), relative_similarity::max_similarity_value());
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_1_.vectors_count());

	for (const auto& similar_cv_data : cm_similarity_data)
	{
		ASSERT_EQ(similar_cv_data.relative_similarity().to_double(), relative_similarity::max_similarity_value());
		ASSERT_EQ(similar_cv_data.index_of_first_vector().to_size_t() + similar_cv_data.index_of_second_vector().to_size_t(), cm_1_.vectors_count() - 1);
	}
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, different_matrices_equal_cv_count_through_abstract_class)
{
	const double distance_1 = std::sqrt(3.0 / 14);
	const double distance_2 = std::sqrt(3.0 / 77);
	const double distance_3 = std::sqrt(3.0 / 194);
	const double distance_4 = std::sqrt(3.0 / 365);

	const double vectors_similarity_1 = 1 - distance_1;
	const double vectors_similarity_2 = 1 - distance_2;
	const double vectors_similarity_3 = 1 - distance_3;
	const double vectors_similarity_4 = 1 - distance_4;
	const std::vector cv_similarities_descending_order{ vectors_similarity_4, vectors_similarity_3, vectors_similarity_2, vectors_similarity_1 };

	const double cm_similarity = (vectors_similarity_1 + vectors_similarity_2 + vectors_similarity_3 + vectors_similarity_4) / 4;

	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const cm_similarity_abstract_algorithm<3>& ref_to_abstract = cm_similarity_alg;
	const auto cm_similarity_data = ref_to_abstract.determine_similarity(cm_1_, cm_2_);

	ASSERT_NEAR(cm_similarity_data.matrices_relative_similarity().to_double(), cm_similarity, 1e-3);

	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_1_.vectors_count());

	for (size_t cv_pair_index = 0; cv_pair_index < cm_similarity_data.similar_cv_pairs_count(); ++cv_pair_index)
	{
		const auto& cv_pair = cm_similarity_data.get_similar_vectors_data_dont_check_index(index_of_count_vectors_pair(cv_pair_index));
		ASSERT_NEAR(cv_pair.relative_similarity().to_double(), cv_similarities_descending_order[cv_pair_index], 1e-3);
		ASSERT_EQ(cv_pair.index_of_first_vector().to_size_t(), cm_1_.vectors_count() - 1 - cv_pair_index);
		ASSERT_EQ(cv_pair.index_of_second_vector().to_size_t(), cm_1_.vectors_count() - 1 - cv_pair_index);
	}
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, different_matrices_equal_cv_count_swap_matrices_through_abstract_class)
{
	const double distance_1 = std::sqrt(3.0 / 14);
	const double distance_2 = std::sqrt(3.0 / 77);
	const double distance_3 = std::sqrt(3.0 / 194);
	const double distance_4 = std::sqrt(3.0 / 365);

	const double vectors_similarity_1 = 1 - distance_1;
	const double vectors_similarity_2 = 1 - distance_2;
	const double vectors_similarity_3 = 1 - distance_3;
	const double vectors_similarity_4 = 1 - distance_4;
	const std::vector cv_similarities_descending_order{ vectors_similarity_4, vectors_similarity_3, vectors_similarity_2, vectors_similarity_1 };

	const double cm_similarity = (vectors_similarity_1 + vectors_similarity_2 + vectors_similarity_3 + vectors_similarity_4) / 4;

	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const cm_similarity_abstract_algorithm<3>& ref_to_abstract = cm_similarity_alg;
	const auto cm_similarity_data = ref_to_abstract.determine_similarity(cm_2_, cm_1_);

	ASSERT_NEAR(cm_similarity_data.matrices_relative_similarity().to_double(), cm_similarity, 1e-3);

	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_1_.vectors_count());

	for (size_t cv_pair_index = 0; cv_pair_index < cm_similarity_data.similar_cv_pairs_count(); ++cv_pair_index)
	{
		const auto& cv_pair = cm_similarity_data.get_similar_vectors_data_dont_check_index(index_of_count_vectors_pair(cv_pair_index));
		ASSERT_NEAR(cv_pair.relative_similarity().to_double(), cv_similarities_descending_order[cv_pair_index], 1e-3);
		ASSERT_EQ(cv_pair.index_of_first_vector().to_size_t(), cm_1_.vectors_count() - 1 - cv_pair_index);
		ASSERT_EQ(cv_pair.index_of_second_vector().to_size_t(), cm_1_.vectors_count() - 1 - cv_pair_index);
	}
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, increased_accuracy_through_abstract_class)
{
	const double distance_1 = std::sqrt(3.0 / 14);
	const double distance_2 = std::sqrt(3.0 / 77);
	const double distance_3 = std::sqrt(3.0 / 194);
	const double distance_4 = std::sqrt(3.0 / 365);

	const double vectors_similarity_1 = 1 - distance_1;
	const double vectors_similarity_2 = 1 - distance_2;
	const double vectors_similarity_3 = 1 - distance_3;
	const double vectors_similarity_4 = 1 - distance_4;
	const std::vector cv_similarities_descending_order{ vectors_similarity_4, vectors_similarity_3, vectors_similarity_2, vectors_similarity_1 };

	const double cm_similarity = (vectors_similarity_1 + vectors_similarity_2 + vectors_similarity_3 + vectors_similarity_4) / 4;

	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{10};
	const cm_similarity_abstract_algorithm<3>& ref_to_abstract = cm_similarity_alg;
	const auto cm_similarity_data = ref_to_abstract.determine_similarity(cm_1_, cm_2_);

	ASSERT_NEAR(cm_similarity_data.matrices_relative_similarity().to_double(), cm_similarity, 1e-10);

	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_1_.vectors_count());

	for (size_t cv_pair_index = 0; cv_pair_index < cm_similarity_data.similar_cv_pairs_count(); ++cv_pair_index)
	{
		const auto& cv_pair = cm_similarity_data.get_similar_vectors_data_dont_check_index(index_of_count_vectors_pair(cv_pair_index));
		ASSERT_NEAR(cv_pair.relative_similarity().to_double(), cv_similarities_descending_order[cv_pair_index], 1e-10);
		ASSERT_EQ(cv_pair.index_of_first_vector().to_size_t(), cm_1_.vectors_count() - 1 - cv_pair_index);
		ASSERT_EQ(cv_pair.index_of_second_vector().to_size_t(), cm_1_.vectors_count() - 1 - cv_pair_index);
	}
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, decreased_accuracy_through_abstract_class)
{
	const double distance_1 = std::sqrt(3.0 / 14);
	const double distance_2 = std::sqrt(3.0 / 77);
	const double distance_3 = std::sqrt(3.0 / 194);
	const double distance_4 = std::sqrt(3.0 / 365);

	const double vectors_similarity_1 = 1 - distance_1;
	const double vectors_similarity_2 = 1 - distance_2;
	const double vectors_similarity_3 = 1 - distance_3;
	const double vectors_similarity_4 = 1 - distance_4;

	const double cm_similarity = (vectors_similarity_1 + vectors_similarity_2 + vectors_similarity_3 + vectors_similarity_4) / 4;

	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{0};
	const cm_similarity_abstract_algorithm<3>& ref_to_abstract = cm_similarity_alg;
	const auto cm_similarity_data = ref_to_abstract.determine_similarity(cm_1_, cm_2_);

	ASSERT_NEAR(cm_similarity_data.matrices_relative_similarity().to_double(), cm_similarity, 1e0);

	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_1_.vectors_count());
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, matrices_with_different_count_of_vectors_through_abstract_class)
{
	constexpr double distance_1 = 1.0; // between first vector in cm_1_ and fourth (fictive) zero-vector in cm_2_incomplete_
	const double distance_2 = sqrt(3.0 / 77);
	const double distance_3 = sqrt(3.0 / 194);
	const double distance_4 = sqrt(3.0 / 365);

	constexpr double vectors_similarity_1 = 1 - distance_1;
	const double vectors_similarity_2 = 1 - distance_2;
	const double vectors_similarity_3 = 1 - distance_3;
	const double vectors_similarity_4 = 1 - distance_4;
	const std::vector cv_similarities_descending_order{ vectors_similarity_4, vectors_similarity_3, vectors_similarity_2 };

	const double cm_similarity = (vectors_similarity_1 + vectors_similarity_2 + vectors_similarity_3 + vectors_similarity_4) / 4;

	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const cm_similarity_abstract_algorithm<3>& ref_to_abstract = cm_similarity_alg;
	const auto cm_similarity_data = ref_to_abstract.determine_similarity(cm_1_, cm_2_incomplete_);

	ASSERT_NEAR(cm_similarity_data.matrices_relative_similarity().to_double(), cm_similarity, 1e-3);
	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_2_incomplete_.vectors_count());

	for (size_t cv_pair_index = 0; cv_pair_index < cm_similarity_data.similar_cv_pairs_count(); ++cv_pair_index)
	{
		const auto& cv_pair = cm_similarity_data.get_similar_vectors_data_dont_check_index(index_of_count_vectors_pair(cv_pair_index));
		ASSERT_NEAR(cv_pair.relative_similarity().to_double(), cv_similarities_descending_order[cv_pair_index], 1e-3);
		ASSERT_EQ(cv_pair.index_of_first_vector().to_size_t(), cm_1_.vectors_count() - 1 - cv_pair_index);
		ASSERT_EQ(cv_pair.index_of_second_vector().to_size_t(), cm_2_incomplete_.vectors_count() - 1 - cv_pair_index);
	}
}

TEST_F(cm_max_weighted_bipartite_matching_dim3_test_f, several_possible_matchings_through_abstract_class)
{
	const double distance_12 = std::sqrt(3.0 / 14);
	const double distance_34 = std::sqrt(3.0 / 77);
	const double distance_56 = std::sqrt(3.0 / 194);
	const double distance_78 = std::sqrt(3.0 / 365);

	const double vectors_similarity_12 = 1 - distance_12;
	const double vectors_similarity_34 = 1 - distance_34;
	const double vectors_similarity_56 = 1 - distance_56;
	const double vectors_similarity_78 = 1 - distance_78;
	const std::vector cv_similarities_descending_order{ vectors_similarity_78, vectors_similarity_78, vectors_similarity_56, vectors_similarity_56,
		vectors_similarity_34, vectors_similarity_34, vectors_similarity_12, vectors_similarity_12 };

	const double cm_similarity = (vectors_similarity_12 + vectors_similarity_34 + vectors_similarity_56 + vectors_similarity_78) / 4;

	const cm_max_weighted_bipartite_matching<3> cm_similarity_alg{};
	const cm_similarity_abstract_algorithm<3>& ref_to_abstract = cm_similarity_alg;
	const auto cm_similarity_data = ref_to_abstract.determine_similarity(cm_1_repeated_cv_, cm_2_repeated_cv_);

	ASSERT_NEAR(cm_similarity_data.matrices_relative_similarity().to_double(), cm_similarity, 1e-3);

	ASSERT_EQ(cm_similarity_data.similar_cv_pairs_count(), cm_1_repeated_cv_.vectors_count());

	for (size_t cv_pair_index = 0; cv_pair_index < cm_similarity_data.similar_cv_pairs_count(); ++cv_pair_index)
	{
		const auto& cv_pair = cm_similarity_data.get_similar_vectors_data_dont_check_index(index_of_count_vectors_pair(cv_pair_index));
		ASSERT_NEAR(cv_pair.relative_similarity().to_double(), cv_similarities_descending_order[cv_pair_index], 1e-3);
		const int first_similar_cv_index = static_cast<int>(cv_pair.index_of_first_vector().to_size_t());
		const int second_similar_cv_index = static_cast<int>(cv_pair.index_of_second_vector().to_size_t());
		const int max_index_in_similar_cv_pair = std::max(first_similar_cv_index, second_similar_cv_index);
		ASSERT_TRUE(first_similar_cv_index == second_similar_cv_index ||
			std::abs(first_similar_cv_index - second_similar_cv_index) == 1 && max_index_in_similar_cv_pair % 2 == 1);
	}
}