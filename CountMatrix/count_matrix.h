#pragma once
#include "count_vector.h"
#include "index_of_count_vector.h"
#include <vector>
#include <algorithm>

namespace cm
{
	template <size_t CountVectorDimension>
	class count_matrix final
	{
		std::vector<count_vector<CountVectorDimension>> count_vectors_{};

	public:
		count_matrix() = default;

		explicit count_matrix(std::vector<count_vector<CountVectorDimension>> count_vectors) noexcept
			: count_vectors_(std::move(count_vectors)) {}

		[[nodiscard]] size_t vectors_count() const noexcept
		{
			return count_vectors_.size();
		}

		[[nodiscard]] const count_vector<CountVectorDimension>& operator[](
			const internal::index_of_count_vector count_vector_index) const
		{
			return count_vectors_[count_vector_index.to_size_t()];
		}

		[[nodiscard]] const count_vector<CountVectorDimension>& at(
			const internal::index_of_count_vector count_vector_index) const
		{
			return count_vectors_.at(count_vector_index.to_size_t());
		}

		[[nodiscard]] bool empty() const noexcept
		{
			return count_vectors_.empty();
		}

		[[nodiscard]] auto begin() const noexcept
		{
			return count_vectors_.cbegin();
		}

		[[nodiscard]] auto end() const noexcept
		{
			return count_vectors_.cend();
		}
	};
}
