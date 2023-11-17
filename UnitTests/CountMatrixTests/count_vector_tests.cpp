#include "gtest_pch.h"
#include "count_vector.h"

using namespace cm;

TEST(index_of_count_value_test, ctor_default)
{
	constexpr internal::index_of_counted_value default_index{};
	ASSERT_EQ(default_index.to_size_t(), 0);
}

TEST(index_of_count_value_test, ctor_with_index)
{
	constexpr size_t index = 42;
	const internal::index_of_counted_value index_of_count_value(index);
	ASSERT_EQ(index_of_count_value.to_size_t(), index);
}


TEST(count_vector_dim42_test, default_ctor)
{
	for (const auto& value : cm::count_vector<42>{})
	{
		ASSERT_EQ(value.value(), 0);
	}
}

TEST(count_vector_dim42_test, ctor_with_array)
{
	const std::array<counted_value, 42> count_values{
		counted_value(1), counted_value(3), counted_value(5), counted_value(777)
	};
	size_t index = 0;
	for (const auto& value : count_vector{count_values})
	{
		ASSERT_EQ(value.value(), count_values[index].value());
		++index;
	}
}

TEST(count_vector_dim42_test, size)
{
	const count_vector<42> default_cv{};
	ASSERT_EQ(default_cv.size(), 42);
}

TEST(count_vector_dim4_test, operator_square_brackets)
{
	const std::array count_values{
		counted_value(1), counted_value(3), counted_value(5), counted_value(7)
	};
	const count_vector cv{count_values};
	for (size_t index = 0; index < cv.size(); ++index)
	{
		ASSERT_EQ(count_values[index].value(), cv[internal::index_of_counted_value{index}].value());
	}
}

TEST(count_vector_dim4_test, at)
{
	const std::array count_values{
		counted_value(1), counted_value(3), counted_value(5), counted_value(7)
	};
	const count_vector cv{count_values};
	for (size_t index = 0; index < cv.size(); ++index)
	{
		ASSERT_EQ(count_values[index].value(), cv.at(internal::index_of_counted_value{index}).value());
	}
}

TEST(count_vector_dim4_test, at_invalid_index)
{
	const std::array count_values{
		counted_value(1), counted_value(3), counted_value(5), counted_value(7)
	};
	const count_vector cv{count_values};
	ASSERT_THROW(const auto& _ = cv.at(internal::index_of_counted_value{ cv.size() }), std::out_of_range);
}

TEST(count_vector_dim4_test, iterators)
{
	const std::array<counted_value, 42> count_values{
		counted_value(1), counted_value(3), counted_value(5), counted_value(777)
	};
	const count_vector cv{count_values};

	size_t forward_index = 0;
	for (auto iterator = cv.begin(); iterator != cv.end(); ++iterator)
	{
		ASSERT_EQ(iterator->value(), count_values[forward_index].value());
		ASSERT_EQ(cv.begin() + forward_index, iterator);
		ASSERT_EQ(cv.end() - (cv.size() - forward_index), iterator);
		++forward_index;
	}

	size_t backward_index = cv.size() - 1;
	for (auto iterator = cv.end(); iterator > cv.begin();)
	{
		--iterator;
		ASSERT_EQ(iterator->value(), count_values[backward_index].value());
		--backward_index;
	}

	ASSERT_EQ(cv.begin() + cv.size(), cv.end());
	ASSERT_EQ(cv.begin(), cv.end() - cv.size());
}


TEST(count_vector_zero_dim_test, default_ctor)
{
	for (const auto& value : cm::count_vector<0>{})
	{
		ASSERT_EQ(value.value(), 0);
	}
}

TEST(count_vector_zero_dim_test, ctor_with_array)
{
	constexpr std::array<counted_value, 0> count_values{};
	size_t index = 0;
	for (const auto& value : count_vector{count_values})
	{
		ASSERT_EQ(value.value(), count_values[index].value());
		++index;
	}
}

TEST(count_vector_zero_dim_test, size)
{
	const count_vector<0> default_cv{};
	ASSERT_EQ(default_cv.size(), 0);
}

TEST(count_vector_zero_dim_test, at_invalid_index)
{
	const count_vector<0> default_cv{};
	ASSERT_THROW(const auto & _ = default_cv.at(internal::index_of_counted_value{ default_cv.size() }),
	             std::out_of_range);
}

TEST(count_vector_0_test, iterators)
{
	const count_vector<0> default_cv{};
	ASSERT_EQ(default_cv.begin(), default_cv.end());
}
