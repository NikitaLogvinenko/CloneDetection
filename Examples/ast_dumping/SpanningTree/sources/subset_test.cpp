#include "pch.h"
#include "disjoint_set.h"

using namespace spanning_tree_ns;

TEST(DisjointSet, CtorDefault)
{
	const disjoint_set<int>::subset default_subset{};
	ASSERT_EQ(default_subset.index(), 0);
}

TEST(DisjointSet, CtorWithIndex)
{
	for (size_t index = 0; index < 1000; ++index)
	{
		const disjoint_set<int>::subset subset{index};
		ASSERT_EQ(subset.index(), index);
	}
}

TEST(DisjointSet, OperatorEqual)
{
	for (size_t index = 1; index < 1000; ++index)
	{
		const disjoint_set<int>::subset subset{ index };
		const disjoint_set<int>::subset equal_subset{ index };
		const disjoint_set<int>::subset subset_greater{ index + 1 };
		const disjoint_set<int>::subset subset_less{ index - 1 };
		ASSERT_EQ(subset, subset);
		ASSERT_EQ(subset, equal_subset);
		ASSERT_NE(subset, subset_greater);
		ASSERT_NE(subset, subset_less);
	}
}

TEST(DisjointSet, OperatorLess)
{
	for (size_t index = 1; index < 1000; ++index)
	{
		const disjoint_set<int>::subset subset{ index };
		const disjoint_set<int>::subset equal_subset{ index };
		const disjoint_set<int>::subset subset_greater{ index + 1 };
		const disjoint_set<int>::subset subset_less{ index - 1 };
		ASSERT_FALSE(subset < subset);
		ASSERT_FALSE(subset < equal_subset);
		ASSERT_TRUE(subset < subset_greater);
		ASSERT_TRUE(subset_less < subset);
		ASSERT_FALSE(subset_greater < subset);
		ASSERT_FALSE(subset < subset_less);
	}
}
