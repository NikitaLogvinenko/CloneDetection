#include "pch.h"
#include "disjoint_set.h"
#include "vertex_Rn_space.h"

using namespace spanning_tree_ns;
using namespace graph_ns;

class DisjointSetIntTestFixing : public ::testing::Test {
protected:
	void SetUp() override
	{
		for (int value = -1000; value <= 1000; ++ value)
		{
			values_repeated_3_times_.push_back(value);
			values_repeated_3_times_.push_back(value);
			values_repeated_3_times_.push_back(value);
		}
	}

	std::vector<int> values_repeated_3_times_{};
};

TEST_F(DisjointSetIntTestFixing, Ctor)
{
	const disjoint_set<int> ds;
	ASSERT_EQ(ds.subsets_count(), 0);
	ASSERT_TRUE(ds.existed_subsets().empty());
}

TEST_F(DisjointSetIntTestFixing, InsertNew)
{
	disjoint_set<int> ds{};
	size_t counter = 0;
	for (auto value_it = values_repeated_3_times_.cbegin(); value_it != values_repeated_3_times_.cend(); value_it += 3)
	{
		const disjoint_set<int>::subset subset_equal{ counter };
		const auto subset = ds.insert(*value_it);
		++counter;
		ASSERT_EQ(ds.subsets_count(), counter);
		ASSERT_EQ(subset.index(), subset_equal.index());
		ASSERT_EQ(subset, subset_equal);
	}
}

TEST_F(DisjointSetIntTestFixing, InsertExisted)
{
	disjoint_set<int> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}

	size_t counter = 0;
	const size_t initial_size = ds.subsets_count();
	for (auto value_it = values_repeated_3_times_.cbegin(); value_it != values_repeated_3_times_.cend(); value_it += 3)
	{
		const disjoint_set<int>::subset subset_equal{ counter };
		const auto subset = ds.insert(*value_it);
		++counter;
		ASSERT_EQ(ds.subsets_count(), initial_size);
		ASSERT_EQ(subset.index(), subset_equal.index());
		ASSERT_EQ(subset, subset_equal);
	}
}

TEST_F(DisjointSetIntTestFixing, JoinDifferentExistent)
{
	disjoint_set<int> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}
	const size_t initial_size = ds.subsets_count();
	auto existed_subsets = ds.existed_subsets();
	auto& subset_to = existed_subsets[1], subset_from = existed_subsets[2];
	ds.join(subset_to, subset_from);
	ASSERT_EQ(ds.subsets_count() + 1, initial_size);

	subset_to = existed_subsets[4];
	subset_from = existed_subsets[3];
	ds.join(subset_to, subset_from);
	ASSERT_EQ(ds.subsets_count() + 2, initial_size);
}

TEST_F(DisjointSetIntTestFixing, JoinNonExistentTo)
{
	disjoint_set<int> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}
	const size_t initial_size = ds.subsets_count();
	const auto existed_subsets = ds.existed_subsets();
	const auto& subset_from = existed_subsets[0];
	const auto& non_existent_subset_to = ds.end();
	ds.join(non_existent_subset_to, subset_from);
	ASSERT_EQ(ds.subsets_count(), initial_size);
}

TEST_F(DisjointSetIntTestFixing, JoinNonExistentFrom)
{
	disjoint_set<int> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}
	const size_t initial_size = ds.subsets_count();
	const auto existed_subsets = ds.existed_subsets();
	const auto& subset_to = existed_subsets[0];
	const auto& non_existent_subset_from = ds.end();
	ds.join(subset_to, non_existent_subset_from);
	ASSERT_EQ(ds.subsets_count(), initial_size);
}

TEST_F(DisjointSetIntTestFixing, JoinTheSame)
{
	disjoint_set<int> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}
	const size_t initial_size = ds.subsets_count();
	for (const auto& subset : ds.existed_subsets())
	{
		ds.join(subset, subset);
		ASSERT_EQ(ds.subsets_count(), initial_size);
	}
}

TEST_F(DisjointSetIntTestFixing, JoinAllToFirst)
{
	disjoint_set<int> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}
	const auto existed_subsets = ds.existed_subsets();
	const auto& first_subset = existed_subsets[0];

	const size_t initial_size = ds.subsets_count();
	size_t joined = 0;

	for (const auto& subset : ds.existed_subsets())
	{
		ds.join(first_subset, subset);
		if (first_subset != subset)
		{
			++joined;
		}
		ASSERT_EQ(ds.subsets_count(), initial_size - joined);
	}
	ASSERT_EQ(ds.existed_subsets().size(), 1);
	ASSERT_EQ(ds.existed_subsets()[0], first_subset);
}

TEST_F(DisjointSetIntTestFixing, JoinAllToLast)
{
	disjoint_set<int> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}
	const auto existed_subsets = ds.existed_subsets();
	const auto& last_subset = *(existed_subsets.cend() - 1);

	const size_t initial_size = ds.subsets_count();
	size_t joined = 0;

	for (const auto& subset : ds.existed_subsets())
	{
		ds.join(last_subset, subset);
		if (last_subset != subset)
		{
			++joined;
		}
		ASSERT_EQ(ds.subsets_count(), initial_size - joined);
	}
	ASSERT_EQ(ds.existed_subsets().size(), 1);
	ASSERT_EQ(ds.existed_subsets()[0], last_subset);
}

TEST_F(DisjointSetIntTestFixing, JoinOddToEvenIndicesAndFind)
{
	disjoint_set<int> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}
	const auto existed_subsets = ds.existed_subsets();

	for (auto subset_it = existed_subsets.cbegin(); subset_it + 1 < existed_subsets.cend(); subset_it += 2)
	{
		ds.join(*subset_it, *(subset_it+1));
	}

	size_t counter = 0;
	for (auto value_it = values_repeated_3_times_.cbegin(); value_it != values_repeated_3_times_.cend(); value_it += 3, ++counter)
	{
		ASSERT_EQ(ds.find(*value_it).index(), counter - counter % 2);
	}
}

TEST_F(DisjointSetIntTestFixing, FindNonExistent)
{
	disjoint_set<int> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}

	const int max_value = values_repeated_3_times_[values_repeated_3_times_.size() - 1];
	const int min_value = values_repeated_3_times_[0];
	for (int shift = 1; shift != 100; ++shift)
	{
		ASSERT_EQ(ds.find(max_value + shift), ds.end());
		ASSERT_EQ(ds.find(min_value - shift), ds.end());
	}
}


class DisjointSetVertexR2SpaceTestFixing : public ::testing::Test {
protected:
	void SetUp() override
	{
		for (int value = -1000; value <= 1000; ++value)
		{
			const std::array coords_1{ static_cast<double>(value), static_cast<double>(value) };
			const std::array coords_2{ static_cast<double>(value), static_cast<double>(value + 1) };
			const std::array coords_3{ static_cast<double>(value + 1), static_cast<double>(value) };
			values_repeated_3_times_.emplace_back(std::to_string(value), point_Rn_space(coords_1));
			values_repeated_3_times_.emplace_back(std::to_string(value), point_Rn_space(coords_2));
			values_repeated_3_times_.emplace_back(std::to_string(value), point_Rn_space(coords_3));
		}
	}

	std::vector<vertex_Rn_space<std::string, 2>> values_repeated_3_times_{};
};

TEST_F(DisjointSetVertexR2SpaceTestFixing, Ctor)
{
	const disjoint_set<vertex_Rn_space<std::string, 2>> ds;
	ASSERT_EQ(ds.subsets_count(), 0);
	ASSERT_TRUE(ds.existed_subsets().empty());
}

TEST_F(DisjointSetVertexR2SpaceTestFixing, InsertNew)
{
	disjoint_set<vertex_Rn_space<std::string, 2>> ds;
	size_t counter = 0;
	for (auto value_it = values_repeated_3_times_.cbegin(); value_it != values_repeated_3_times_.cend(); value_it += 3)
	{
		const disjoint_set<vertex_Rn_space<std::string, 2>>::subset subset_equal{ counter };
		const auto subset = ds.insert(*value_it);
		++counter;
		ASSERT_EQ(ds.subsets_count(), counter);
		ASSERT_EQ(subset.index(), subset_equal.index());
		ASSERT_EQ(subset, subset_equal);
	}
}

TEST_F(DisjointSetVertexR2SpaceTestFixing, InsertExisted)
{
	disjoint_set<vertex_Rn_space<std::string, 2>> ds;
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}

	size_t counter = 0;
	const size_t initial_size = ds.subsets_count();
	for (auto value_it = values_repeated_3_times_.cbegin(); value_it != values_repeated_3_times_.cend(); value_it += 3)
	{
		const disjoint_set<vertex_Rn_space<std::string, 2>>::subset subset_equal{ counter };
		const auto subset = ds.insert(*value_it);
		++counter;
		ASSERT_EQ(ds.subsets_count(), initial_size);
		ASSERT_EQ(subset.index(), subset_equal.index());
		ASSERT_EQ(subset, subset_equal);
	}
}

TEST_F(DisjointSetVertexR2SpaceTestFixing, JoinDifferentExistent)
{
	disjoint_set<vertex_Rn_space<std::string, 2>> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}
	const size_t initial_size = ds.subsets_count();
	auto existed_subsets = ds.existed_subsets();
	auto& subset_to = existed_subsets[1], subset_from = existed_subsets[2];
	ds.join(subset_to, subset_from);
	ASSERT_EQ(ds.subsets_count() + 1, initial_size);

	subset_to = existed_subsets[4];
	subset_from = existed_subsets[3];
	ds.join(subset_to, subset_from);
	ASSERT_EQ(ds.subsets_count() + 2, initial_size);
}

TEST_F(DisjointSetVertexR2SpaceTestFixing, JoinNonExistentTo)
{
	disjoint_set<vertex_Rn_space<std::string, 2>> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}
	const size_t initial_size = ds.subsets_count();
	const auto existed_subsets = ds.existed_subsets();
	const auto& subset_from = existed_subsets[0];
	const auto& non_existent_subset_to = ds.end();
	ds.join(non_existent_subset_to, subset_from);
	ASSERT_EQ(ds.subsets_count(), initial_size);
}

TEST_F(DisjointSetVertexR2SpaceTestFixing, JoinNonExistentFrom)
{
	disjoint_set<vertex_Rn_space<std::string, 2>> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}
	const size_t initial_size = ds.subsets_count();
	const auto existed_subsets = ds.existed_subsets();
	const auto& subset_to = existed_subsets[0];
	const auto& non_existent_subset_from = ds.end();
	ds.join(subset_to, non_existent_subset_from);
	ASSERT_EQ(ds.subsets_count(), initial_size);
}

TEST_F(DisjointSetVertexR2SpaceTestFixing, JoinTheSame)
{
	disjoint_set<vertex_Rn_space<std::string, 2>> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}
	const size_t initial_size = ds.subsets_count();
	for (const auto& subset : ds.existed_subsets())
	{
		ds.join(subset, subset);
		ASSERT_EQ(ds.subsets_count(), initial_size);
	}
}

TEST_F(DisjointSetVertexR2SpaceTestFixing, JoinAllToFirst)
{
	disjoint_set<vertex_Rn_space<std::string, 2>> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}
	const auto existed_subsets = ds.existed_subsets();
	const auto& first_subset = existed_subsets[0];

	const size_t initial_size = ds.subsets_count();
	size_t joined = 0;

	for (const auto& subset : ds.existed_subsets())
	{
		ds.join(first_subset, subset);
		if (first_subset != subset)
		{
			++joined;
		}
		ASSERT_EQ(ds.subsets_count(), initial_size - joined);
	}
	ASSERT_EQ(ds.existed_subsets().size(), 1);
	ASSERT_EQ(ds.existed_subsets()[0], first_subset);
}

TEST_F(DisjointSetVertexR2SpaceTestFixing, JoinAllToLast)
{
	disjoint_set<vertex_Rn_space<std::string, 2>> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}
	const auto existed_subsets = ds.existed_subsets();
	const auto& last_subset = *(existed_subsets.cend() - 1);

	const size_t initial_size = ds.subsets_count();
	size_t joined = 0;

	for (const auto& subset : ds.existed_subsets())
	{
		ds.join(last_subset, subset);
		if (last_subset != subset)
		{
			++joined;
		}
		ASSERT_EQ(ds.subsets_count(), initial_size - joined);
	}
	ASSERT_EQ(ds.existed_subsets().size(), 1);
	ASSERT_EQ(ds.existed_subsets()[0], last_subset);
}

TEST_F(DisjointSetVertexR2SpaceTestFixing, JoinOddToEvenIndicesAndFind)
{
	disjoint_set<vertex_Rn_space<std::string, 2>> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}
	const auto existed_subsets = ds.existed_subsets();

	for (auto subset_it = existed_subsets.cbegin(); subset_it + 1 < existed_subsets.cend(); subset_it += 2)
	{
		ds.join(*subset_it, *(subset_it + 1));
	}

	size_t counter = 0;
	for (auto value_it = values_repeated_3_times_.cbegin(); value_it != values_repeated_3_times_.cend(); value_it += 3, ++counter)
	{
		ASSERT_EQ(ds.find(*value_it).index(), counter - counter % 2);
	}
}

TEST_F(DisjointSetVertexR2SpaceTestFixing, FindNonExistent)
{
	disjoint_set<vertex_Rn_space<std::string, 2>> ds{};
	for (const auto& value : values_repeated_3_times_)
	{
		ds.insert(value);
	}

	const double max_value = values_repeated_3_times_[values_repeated_3_times_.size() - 1].value().coordinates()[0];
	const double min_value = values_repeated_3_times_[0].value().coordinates()[0];
	for (int shift = 1; shift != 100; ++shift)
	{
		const double non_existent_greater_coord = max_value + static_cast<double>(shift);
		const double non_existent_less_coord = min_value - static_cast<double>(shift);
		const std::array coords_greater{ non_existent_greater_coord, non_existent_greater_coord };
		const std::array coords_less{ non_existent_less_coord, non_existent_less_coord };
		const vertex_Rn_space v1(std::to_string(non_existent_greater_coord), point_Rn_space<2>(coords_greater));
		const vertex_Rn_space v2(std::to_string(non_existent_less_coord), point_Rn_space<2>(coords_less));
		ASSERT_EQ(ds.find(v1), ds.end());
		ASSERT_EQ(ds.find(v2), ds.end());
	}
}