#pragma once
#include "cm_similar_vectors_data.h"
#include "index_of_count_vectors_pair.h"
#include <utility>

namespace cm
{
	class count_matrices_similarity_data final
	{
		relative_similarity matrices_relative_similarity_{};
		std::vector<internal::cm_similar_vectors_data> vectors_similarity_data_descending_order_{};

	public:
		count_matrices_similarity_data() noexcept = default;

		explicit count_matrices_similarity_data(
			const relative_similarity& matrices_relative_similarity,
			std::vector<internal::cm_similar_vectors_data> vectors_similarity_data = {})
			: matrices_relative_similarity_(matrices_relative_similarity),
			  vectors_similarity_data_descending_order_(std::move(vectors_similarity_data))
		{
			std::ranges::sort(vectors_similarity_data_descending_order_,
			                  [](const internal::cm_similar_vectors_data& first_vectors_pair,
			                     const internal::cm_similar_vectors_data& second_vectors_pair)
			                  {
				                  return second_vectors_pair.relative_similarity() < first_vectors_pair.
					                  relative_similarity();
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

		[[nodiscard]] const internal::cm_similar_vectors_data& get_similar_vectors_data(
			const internal::index_of_count_vectors_pair& index_in_descending_similarity_order) const
		{
			return vectors_similarity_data_descending_order_.at(index_in_descending_similarity_order.to_size_t());
		}

		[[nodiscard]] const internal::cm_similar_vectors_data& get_similar_vectors_data_dont_check_index(
			const internal::index_of_count_vectors_pair& index_in_descending_similarity_order) const
		{
			return vectors_similarity_data_descending_order_[index_in_descending_similarity_order.to_size_t()];
		}

		[[nodiscard]] auto begin() const noexcept
		{
			return code_generation::iterator_wrapper{ vectors_similarity_data_descending_order_.cbegin() };
		}

		[[nodiscard]] auto end() const noexcept
		{
			return code_generation::iterator_wrapper{ vectors_similarity_data_descending_order_.cend() };
		}
	};
}
