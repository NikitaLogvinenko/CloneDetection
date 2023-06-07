#pragma once
#include <utility>
#include <vector>
#include <algorithm>
#include "cm_similar_vectors_indices.h"

namespace count_matrix
{
	class cm_similarity_result final
	{
		class const_iterator final : public std::vector<cm_similar_vectors_indices>::const_iterator
		{
		public:
			explicit const_iterator(const std::vector<cm_similar_vectors_indices>::const_iterator& vector_iterator) noexcept
				: std::vector<cm_similar_vectors_indices>::const_iterator(vector_iterator) {}
		};

		double matrices_similarity_{};
		std::vector<cm_similar_vectors_indices> similar_vectors_pairs_of_indices_{};

		inline static const std::string wrong_similarity_msg{"Matrices similarity is relative value and must be from 0 (absolutely different matrices) to 1 (equal matrices)"};

	public:
		cm_similarity_result() noexcept = default;
		explicit cm_similarity_result(const double matrices_similarity, std::vector<cm_similar_vectors_indices> similar_vectors_pairs_of_indices = {})
			: matrices_similarity_(matrices_similarity), similar_vectors_pairs_of_indices_(std::move(similar_vectors_pairs_of_indices))
		{
			if (matrices_similarity_ < 0 || matrices_similarity_ > 1)
			{
				throw std::range_error(wrong_similarity_msg);
			}
		}

		[[nodiscard]] double matrices_similarity() const noexcept
		{
			return matrices_similarity_;
		}

		[[nodiscard]] size_t similar_vectors_count() const noexcept
		{
			return similar_vectors_pairs_of_indices_.size();
		}

		[[nodiscard]] const cm_similar_vectors_indices& operator[](const size_t index_of_pair_similar_vectors) const noexcept
		{
			return similar_vectors_pairs_of_indices_[index_of_pair_similar_vectors];
		}

		[[nodiscard]] const cm_similar_vectors_indices& at(const size_t index_of_pair_similar_vectors) const
		{
			return similar_vectors_pairs_of_indices_.at(index_of_pair_similar_vectors);
		}

		[[nodiscard]] auto begin() const noexcept
		{
			return const_iterator(similar_vectors_pairs_of_indices_.cbegin());
		}

		[[nodiscard]] auto end() const noexcept
		{
			return const_iterator(similar_vectors_pairs_of_indices_.cend());
		}
	};
}
