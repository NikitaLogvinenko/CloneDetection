#pragma once
#include "count_vector.h"
#include "index_of_count_vector.h"
#include <vector>
#include <algorithm>

namespace cm
{
	template <size_t CountVectorDimension> requires cv_dimension_concept<CountVectorDimension>
	class count_matrix final
	{
		std::vector<count_vector<CountVectorDimension>> count_vectors_{};

	public:
		constexpr count_matrix() noexcept = default;

		constexpr explicit count_matrix(std::vector<count_vector<CountVectorDimension>> count_vectors) noexcept
			: count_vectors_(std::move(count_vectors)) {}

		[[nodiscard]] constexpr size_t vectors_count() const noexcept
		{
			return count_vectors_.size();
		}

		[[nodiscard]] constexpr const count_vector<CountVectorDimension>& at(
			const index_of_count_vector count_vector_index) const
		{
			return count_vectors_.at(count_vector_index.to_size_t());
		}

		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return count_vectors_.empty();
		}

		[[nodiscard]] constexpr auto begin() const noexcept
		{
			return count_vectors_.cbegin();
		}

		[[nodiscard]] constexpr auto end() const noexcept
		{
			return count_vectors_.cend();
		}
	};
}
