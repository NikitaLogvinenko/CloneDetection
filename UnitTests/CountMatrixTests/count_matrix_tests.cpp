#include "gtest_pch.h"
#include "count_matrix.h"
#include "count_matrix_tests_miscellanious.h"

using namespace count_matrix;

TEST(index_of_count_vector_test, ctor_default)
{
	constexpr index_of_count_vector default_index{};
	ASSERT_EQ(default_index.to_size_t(), 0);
}

TEST(index_of_count_vector_test, ctor_with_index)
{
	constexpr size_t index = 42;
	const index_of_count_vector index_of_count_value(index);
	ASSERT_EQ(index_of_count_value.to_size_t(), index);
}

TEST(index_of_count_vector_test, methods_through_base_class)
{
	constexpr size_t index = 42;
	const index_of_count_vector index_of_count_value(index);
	const typed_index* const typed_index_ptr = &index_of_count_value;
	ASSERT_EQ(typed_index_ptr->to_size_t(), index);
}


class count_matrix_dim3_test_f: public ::testing::Test {
protected:
	void SetUp() override
	{
		values_1_ = std::array{ count_vector_value(0), count_vector_value(1), count_vector_value(2) };
		values_2_ = std::array{ count_vector_value(3), count_vector_value(4), count_vector_value(5) };
		values_3_ = std::array{ count_vector_value(6), count_vector_value(7), count_vector_value(8) };
		values_4_ = std::array{ count_vector_value(9), count_vector_value(10), count_vector_value(11) };

		cv_1_ = count_vector{ values_1_ };
		cv_2_ = count_vector{ values_2_ };
		cv_3_ = count_vector{ values_3_ };
		cv_4_ = count_vector{ values_4_ };

		count_vectors_ = std::vector{ cv_1_, cv_2_, cv_3_, cv_4_ };
		cm_ = ::count_matrix::count_matrix{ count_vectors_ };

		cm_from_empty_vector_ = ::count_matrix::count_matrix(std::vector<count_vector<3>>{});
	}

	std::array<count_vector_value, 3> values_1_{};
	std::array<count_vector_value, 3> values_2_{};
	std::array<count_vector_value, 3> values_3_{};
	std::array<count_vector_value, 3> values_4_{};

	count_vector<3> cv_1_{};
	count_vector<3> cv_2_{};
	count_vector<3> cv_3_{};
	count_vector<3> cv_4_{};

	std::vector<count_vector<3>> count_vectors_{};

	::count_matrix::count_matrix<3> cm_{};

	::count_matrix::count_matrix<3> cm_from_empty_vector_{};

	count_vector<3> default_cv_{};
	::count_matrix::count_matrix<3> default_cm_{};
};

TEST_F(count_matrix_dim3_test_f, ctor_default)
{
	ASSERT_TRUE(default_cm_.empty());
	ASSERT_EQ(default_cm_.vectors_count(), 0);
}

TEST_F(count_matrix_dim3_test_f, ctor_with_empty_vector_of_count_vectors)
{
	ASSERT_TRUE(cm_from_empty_vector_.empty());
	ASSERT_EQ(cm_from_empty_vector_.vectors_count(), 0);
}

TEST_F(count_matrix_dim3_test_f, ctor_with_not_empty_vector_of_count_vectors)
{
	size_t cv_index = 0;
	for (const auto& count_vector : cm_)
	{
		assert_equal_count_vectors(count_vector, count_vectors_[cv_index]);
		++cv_index;
	}
}

TEST_F(count_matrix_dim3_test_f, vectors_count)
{
	ASSERT_EQ(default_cm_.vectors_count(), 0);
	ASSERT_EQ(cm_from_empty_vector_.vectors_count(), 0);
	ASSERT_EQ(cm_.vectors_count(), count_vectors_.size());
}

TEST_F(count_matrix_dim3_test_f, empty)
{
	ASSERT_TRUE(default_cm_.empty());
	ASSERT_TRUE(cm_from_empty_vector_.empty());
	ASSERT_FALSE(cm_.empty());
}

TEST_F(count_matrix_dim3_test_f, operator_square_brackets)
{
	for (size_t cv_index = 0; cv_index < cm_.vectors_count(); ++cv_index)
	{
		assert_equal_count_vectors(cm_[index_of_count_vector(cv_index)], count_vectors_[cv_index]);
	}
}

TEST_F(count_matrix_dim3_test_f, at)
{
	for (size_t cv_index = 0; cv_index < cm_.vectors_count(); ++cv_index)
	{
		assert_equal_count_vectors(cm_.at(index_of_count_vector(cv_index)), count_vectors_[cv_index]);
	}
}

TEST_F(count_matrix_dim3_test_f, at_invalid_index)
{
	ASSERT_THROW(const auto& _ = cm_.at(index_of_count_vector(cm_.vectors_count())), std::out_of_range);
}

TEST_F(count_matrix_dim3_test_f, iterators)
{
	size_t forward_index = 0;
	for (auto iterator = cm_.begin(); iterator != cm_.end(); ++iterator)
	{
		assert_equal_count_vectors(*iterator, count_vectors_[forward_index]);
		ASSERT_EQ(cm_.begin() + forward_index, iterator);
		ASSERT_EQ(cm_.end() - (cm_.vectors_count() - forward_index), iterator);
		++forward_index;
	}

	size_t backward_index = cm_.vectors_count() - 1;
	for (auto iterator = cm_.end(); iterator > cm_.begin();)
	{
		--iterator;
		assert_equal_count_vectors(*iterator, count_vectors_[backward_index]);
		--backward_index;
	}
	ASSERT_EQ(cm_.begin() + cm_.vectors_count(), cm_.end());
	ASSERT_EQ(cm_.begin(), cm_.end() - cm_.vectors_count());

	ASSERT_EQ(default_cm_.begin(), default_cm_.end());
	ASSERT_EQ(cm_from_empty_vector_.begin(), cm_from_empty_vector_.end());
}


class count_matrix_zero_dim_test_f : public ::testing::Test {
protected:
	void SetUp() override
	{
		count_vectors_ = std::vector{ cv_default_, cv_default_, cv_default_, cv_default_ };
		cm_ = ::count_matrix::count_matrix{ count_vectors_ };

		cm_from_empty_vector_ = ::count_matrix::count_matrix(std::vector<count_vector<0>>{});
	}

	count_vector<0> cv_default_{};
	
	std::vector<count_vector<0>> count_vectors_{};

	::count_matrix::count_matrix<0> cm_{};

	::count_matrix::count_matrix<0> cm_from_empty_vector_{};

	::count_matrix::count_matrix<0> default_cm_{};
};

TEST_F(count_matrix_zero_dim_test_f, ctor_default)
{
	ASSERT_TRUE(default_cm_.empty());
	ASSERT_EQ(default_cm_.vectors_count(), 0);
}

TEST_F(count_matrix_zero_dim_test_f, ctor_with_empty_vector_of_count_vectors)
{
	ASSERT_TRUE(cm_from_empty_vector_.empty());
	ASSERT_EQ(cm_from_empty_vector_.vectors_count(), 0);
}

TEST_F(count_matrix_zero_dim_test_f, ctor_with_not_empty_vector_of_count_vectors)
{
	size_t cv_index = 0;
	for (const auto& count_vector : cm_)
	{
		assert_equal_count_vectors(count_vector, cv_default_);
		++cv_index;
	}
}

TEST_F(count_matrix_zero_dim_test_f, vectors_count)
{
	ASSERT_EQ(default_cm_.vectors_count(), 0);
	ASSERT_EQ(cm_from_empty_vector_.vectors_count(), 0);
	ASSERT_EQ(cm_.vectors_count(), count_vectors_.size());
}

TEST_F(count_matrix_zero_dim_test_f, empty)
{
	ASSERT_TRUE(default_cm_.empty());
	ASSERT_TRUE(cm_from_empty_vector_.empty());
	ASSERT_FALSE(cm_.empty());
}

TEST_F(count_matrix_zero_dim_test_f, operator_square_brackets)
{
	for (size_t cv_index = 0; cv_index < cm_.vectors_count(); ++cv_index)
	{
		assert_equal_count_vectors(cm_[index_of_count_vector(cv_index)], cv_default_);
	}
}

TEST_F(count_matrix_zero_dim_test_f, at)
{
	for (size_t cv_index = 0; cv_index < cm_.vectors_count(); ++cv_index)
	{
		assert_equal_count_vectors(cm_.at(index_of_count_vector(cv_index)), cv_default_);
	}
}

TEST_F(count_matrix_zero_dim_test_f, at_invalid_index)
{
	ASSERT_THROW(const auto & _ = cm_.at(index_of_count_vector(cm_.vectors_count())), std::out_of_range);
}

TEST_F(count_matrix_zero_dim_test_f, iterators)
{
	size_t forward_index = 0;
	for (auto iterator = cm_.begin(); iterator != cm_.end(); ++iterator)
	{
		assert_equal_count_vectors(*iterator, count_vectors_[forward_index]);
		++forward_index;
	}

	size_t backward_index = cm_.vectors_count() - 1;
	for (auto iterator = cm_.end(); iterator > cm_.begin();)
	{
		--iterator;
		assert_equal_count_vectors(*iterator, count_vectors_[backward_index]);
		--backward_index;
	}
	ASSERT_EQ(cm_.begin() + cm_.vectors_count(), cm_.end());
	ASSERT_EQ(cm_.begin(), cm_.end() - cm_.vectors_count());

	ASSERT_EQ(default_cm_.begin(), default_cm_.end());
	ASSERT_EQ(cm_from_empty_vector_.begin(), cm_from_empty_vector_.end());
}
