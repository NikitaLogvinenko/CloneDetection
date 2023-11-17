#pragma once
#include "cv_similarity_with_indices.h"
#include "index_of_count_vectors_pair.h"
#include <utility>

namespace cm
{
	class count_matrices_similarity_data final
	{
		relative_similarity matrices_relative_similarity_{};
		std::vector<internal::cv_similarity_with_indices> vectors_similarity_data_descending_order_{};

	public:
		count_matrices_similarity_data() = default;

		explicit count_matrices_similarity_data(
			const relative_similarity& matrices_relative_similarity,
			std::vector<internal::cv_similarity_with_indices> vectors_similarity_data = {})
			: matrices_relative_similarity_(matrices_relative_similarity),
			  vectors_similarity_data_descending_order_(std::move(vectors_similarity_data))
		{
			std::ranges::sort(
				vectors_similarity_data_descending_order_,
				std::greater_equal(),
				[](const internal::cv_similarity_with_indices& similar_vectors_pair)
				{
					return similar_vectors_pair.relative_similarity();
				}
			);
		}

		[[nodiscard]] const relative_similarity& matrices_relative_similarity() const noexcept
		{
			return matrices_relative_similarity_;
		}

		[[nodiscard]] size_t similar_cv_pairs_count() const noexcept
		{
			return vectors_similarity_data_descending_order_.size();
		}

		[[nodiscard]] const internal::cv_similarity_with_indices& get_similar_vectors_data(
			const internal::index_of_count_vectors_pair& index_in_descending_similarity_order) const
		{
			return vectors_similarity_data_descending_order_.at(index_in_descending_similarity_order.to_size_t());
		}

		[[nodiscard]] const internal::cv_similarity_with_indices& get_similar_vectors_data_dont_check_index(
			const internal::index_of_count_vectors_pair& index_in_descending_similarity_order) const
		{
			return vectors_similarity_data_descending_order_[index_in_descending_similarity_order.to_size_t()];
		}

		[[nodiscard]] auto begin() const noexcept
		{
			return vectors_similarity_data_descending_order_.cbegin();
		}

		[[nodiscard]] auto end() const noexcept
		{
			return vectors_similarity_data_descending_order_.cend();
		}
	};
}
