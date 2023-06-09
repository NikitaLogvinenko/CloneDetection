#pragma once
#include "cm_similar_vectors_data.h"
#include <utility>

namespace count_matrix_ns
{
	class index_of_count_vectors_pair final : public typed_index
	{
	public:
		index_of_count_vectors_pair() noexcept = default;
		explicit index_of_count_vectors_pair(const size_t index) noexcept : typed_index(index) {}
	};

	class count_matrices_similarity_data final
	{
		class const_similar_vectors_data_iterator final : public std::vector<cm_similar_vectors_data>::const_iterator
		{
		public:
			explicit const_similar_vectors_data_iterator(const std::vector<cm_similar_vectors_data>::const_iterator& vector_iterator) noexcept
				: std::vector<cm_similar_vectors_data>::const_iterator(vector_iterator) {}
		};

		relative_similarity matrices_relative_similarity_{};
		std::vector<cm_similar_vectors_data> vectors_similarity_data_descending_order_{};

	public:
		count_matrices_similarity_data() noexcept = default;
		explicit count_matrices_similarity_data(
			const relative_similarity& matrices_relative_similarity, std::vector<cm_similar_vectors_data> vectors_similarity_data = {})
			: matrices_relative_similarity_(matrices_relative_similarity), vectors_similarity_data_descending_order_(std::move(vectors_similarity_data))
		{
			std::ranges::sort(vectors_similarity_data_descending_order_,
			                  [](const cm_similar_vectors_data& first_vectors_pair, const cm_similar_vectors_data& second_vectors_pair)
			                  { return second_vectors_pair.relative_similarity() < first_vectors_pair.relative_similarity(); });
		}

		[[nodiscard]] const relative_similarity& matrices_relative_similarity() const noexcept
		{
			return matrices_relative_similarity_;
		}

		[[nodiscard]] size_t similar_vectors_count() const noexcept
		{
			return vectors_similarity_data_descending_order_.size();
		}

		[[nodiscard]] const cm_similar_vectors_data& get_similar_vectors_data(
			const index_of_count_vectors_pair& pair_index_in_descending_order) const
		{
			return vectors_similarity_data_descending_order_.at(pair_index_in_descending_order.to_size_t());
		}

		[[nodiscard]] const cm_similar_vectors_data& get_similar_vectors_data_dont_check_index(
			const index_of_count_vectors_pair& pair_index_in_descending_order) const noexcept
		{
			return vectors_similarity_data_descending_order_[pair_index_in_descending_order.to_size_t()];
		}

		[[nodiscard]] const_similar_vectors_data_iterator begin() const noexcept
		{
			return const_similar_vectors_data_iterator(vectors_similarity_data_descending_order_.cbegin());
		}

		[[nodiscard]] const_similar_vectors_data_iterator end() const noexcept
		{
			return const_similar_vectors_data_iterator(vectors_similarity_data_descending_order_.cend());
		}
	};
}
