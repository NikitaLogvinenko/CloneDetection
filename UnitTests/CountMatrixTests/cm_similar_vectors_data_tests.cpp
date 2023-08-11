#include "gtest_pch.h"
#include "cm_similar_vectors_data.h"

TEST(cm_similar_vectors_data_test, ctor_default)
{
	constexpr cm::internal::cm_similar_vectors_data default_data{};
	ASSERT_EQ(default_data.index_of_first_vector().to_size_t(), cm::internal::index_of_count_vector{}.to_size_t());
	ASSERT_EQ(default_data.index_of_second_vector().to_size_t(), cm::internal::index_of_count_vector{}.to_size_t());
	ASSERT_EQ(default_data.relative_similarity(), cm::relative_similarity{});
}

TEST(cm_similar_vectors_data_test, ctor_with_args)
{
	const cm::internal::index_of_count_vector first_index{1};
	const cm::internal::index_of_count_vector second_index{2};
	const cm::relative_similarity similarity{0.5};
	const cm::internal::cm_similar_vectors_data similar_vectors_data{ first_index, second_index, similarity };
	ASSERT_EQ(similar_vectors_data.index_of_first_vector().to_size_t(), first_index.to_size_t());
	ASSERT_EQ(similar_vectors_data.index_of_second_vector().to_size_t(), second_index.to_size_t());
	ASSERT_EQ(similar_vectors_data.relative_similarity(), similarity);
}
