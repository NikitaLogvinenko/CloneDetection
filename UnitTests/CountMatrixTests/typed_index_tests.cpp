#include "gtest_pch.h"
#include "typed_index.h"

TEST(typed_index_test, ctor_default)
{
	constexpr count_matrix::typed_index default_typed_index{};
	ASSERT_EQ(default_typed_index.to_size_t(), 0);
}

TEST(typed_index_test, ctor_with_index)
{
	constexpr size_t index = 42;
	const count_matrix::typed_index typed_index(index);
	ASSERT_EQ(typed_index.to_size_t(), index);
}