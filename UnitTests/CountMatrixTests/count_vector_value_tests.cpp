﻿#include "gtest_pch.h"
#include "counted_value.h"

TEST(count_vector_value_test, ctor_default)
{
	constexpr cm::counted_value default_count_vector_value{};
	ASSERT_EQ(default_count_vector_value.value(), 0);
}

TEST(count_vector_value_test, ctor_with_value)
{
	constexpr size_t value = 42;
	const cm::counted_value count_vector_value(value);
	ASSERT_EQ(count_vector_value.value(), value);
}

TEST(count_vector_value_test, increment_operator)
{
	size_t value = 0;
	cm::counted_value count_vector_value(value);
	for (size_t step = 1; step < 100; ++step, ++value, ++count_vector_value)
	{
		ASSERT_EQ(count_vector_value.value(), value);
	}
}
