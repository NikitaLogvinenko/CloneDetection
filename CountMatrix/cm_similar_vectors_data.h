#pragma once
#include "count_matrix.h"
#include "relative_similarity.h"

namespace cm::internal
{
	class cm_similar_vectors_data final
	{
		index_of_count_vector index_of_first_vector_{};
		index_of_count_vector index_of_second_vector_{};
		relative_similarity relative_similarity_{};

	public:
		cm_similar_vectors_data() noexcept = default;
		explicit cm_similar_vectors_data(
			const index_of_count_vector& index_of_first_vector, 
			const index_of_count_vector& index_of_second_vector, 
			const relative_similarity& relative_similarity) noexcept
			: index_of_first_vector_(index_of_first_vector), index_of_second_vector_(index_of_second_vector), relative_similarity_(relative_similarity) {}

		[[nodiscard]] const index_of_count_vector& index_of_first_vector() const noexcept
		{
			return index_of_first_vector_;
		}

		[[nodiscard]] const index_of_count_vector& index_of_second_vector() const noexcept
		{
			return index_of_second_vector_;
		}

		[[nodiscard]] const relative_similarity& relative_similarity() const noexcept
		{
			return relative_similarity_;
		}
	};
}