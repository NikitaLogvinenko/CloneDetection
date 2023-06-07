#pragma once
#include <stdexcept>

namespace count_matrix
{
	class cm_similar_vectors_indices final
	{
		size_t vector_from_first_cm_index_{};
		size_t vector_from_second_cm_index_{};
		double similarity_{};

		inline static const std::string wrong_similarity_msg{"Vectors similarity is relative value and must be from 0 (absolutely different vectors) to 1 (equal vectors)"};

	public:
		cm_similar_vectors_indices() noexcept = default;
		explicit cm_similar_vectors_indices(const size_t vector_from_first_cm_index, const size_t vector_from_second_cm_index, const double similarity)
			: vector_from_first_cm_index_(vector_from_first_cm_index), vector_from_second_cm_index_(vector_from_second_cm_index), similarity_(similarity)
		{
			if (similarity < 0 || similarity > 1)
			{
				throw std::range_error(wrong_similarity_msg);
			}
		}

		[[nodiscard]] size_t vector_from_first_cm_index() const noexcept
		{
			return vector_from_first_cm_index_;
		}

		[[nodiscard]] size_t vector_from_second_cm_index() const noexcept
		{
			return vector_from_second_cm_index_;
		}

		[[nodiscard]] double similarity() const noexcept
		{
			return similarity_;
		}
	};
}