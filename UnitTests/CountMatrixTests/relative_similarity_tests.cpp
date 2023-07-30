#include "gtest_pch.h"
#include "relative_similarity.h"

TEST(relative_similarity_test, default_ctor)
{
	constexpr count_matrix::relative_similarity default_relative_similarity{};
	ASSERT_EQ(default_relative_similarity.to_double(), 0.0);
}

TEST(relative_similarity_test, ctor_with_valid_similarity)
{
	constexpr double valid_value = 0.5;
	const count_matrix::relative_similarity valid_relative_similarity{valid_value};
	ASSERT_EQ(valid_relative_similarity.to_double(), valid_value);
}

TEST(relative_similarity_test, ctor_with_invalid_similarity)
{
	ASSERT_THROW(count_matrix::relative_similarity(-0.1), common_exceptions::initialized_value_out_of_range);
	ASSERT_THROW(count_matrix::relative_similarity(1.1), common_exceptions::initialized_value_out_of_range);
}

TEST(relative_similarity_test, comparing_operators)
{
	const count_matrix::relative_similarity small_similarity{0.1};
	const count_matrix::relative_similarity small_similarity_copy(small_similarity);
	const count_matrix::relative_similarity big_similarity{0.9};

	ASSERT_EQ(small_similarity, small_similarity_copy);
	ASSERT_FALSE(small_similarity == big_similarity);
	ASSERT_NE(small_similarity, big_similarity);
	ASSERT_FALSE(small_similarity != small_similarity_copy);

	ASSERT_LT(small_similarity, big_similarity);
	ASSERT_FALSE(big_similarity < small_similarity);
	ASSERT_GT(big_similarity, small_similarity);
	ASSERT_FALSE(small_similarity > big_similarity);

	ASSERT_LE(small_similarity, big_similarity);
	ASSERT_LE(small_similarity, small_similarity_copy);
	ASSERT_FALSE(big_similarity <= small_similarity);
	ASSERT_GE(big_similarity, small_similarity);
	ASSERT_GE(small_similarity, small_similarity_copy);
	ASSERT_FALSE(small_similarity >= big_similarity);
}

TEST(relative_similarity_test, min_max_similarity_value)
{
	ASSERT_EQ(count_matrix::relative_similarity::min_similarity_value(), 0);
	ASSERT_EQ(count_matrix::relative_similarity::max_similarity_value(), 1);
}
