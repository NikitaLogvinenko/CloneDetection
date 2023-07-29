#pragma once
#include "count_vector.h"
#include "index_of_count_vector.h"
#include <vector>
#include <algorithm>

namespace count_matrix
{
	template <size_t CountVectorDimension>
	class count_matrix final
	{
		class const_count_vectors_iterator final : public std::vector<count_vector<CountVectorDimension>>::const_iterator
		{
		public:
			explicit const_count_vectors_iterator(const typename std::vector<count_vector<CountVectorDimension>>::const_iterator& vector_iterator) noexcept
				: std::vector<count_vector<CountVectorDimension>>::const_iterator(vector_iterator) {}
		};

		std::vector<count_vector<CountVectorDimension>> count_vectors_{};

	public:
		count_matrix() noexcept = default;
		explicit count_matrix(std::vector<count_vector<CountVectorDimension>> count_vectors) noexcept : count_vectors_(std::move(count_vectors)) {}

		[[nodiscard]] size_t vectors_count() const noexcept
		{
			return count_vectors_.size();
		}

		[[nodiscard]] const count_vector<CountVectorDimension>& operator[](const index_of_count_vector count_vector_index) const
		{
			return count_vectors_[count_vector_index.to_size_t()];
		}

		[[nodiscard]] const count_vector<CountVectorDimension>& at(const index_of_count_vector count_vector_index) const
		{
			return count_vectors_.at(count_vector_index.to_size_t());
		}

		[[nodiscard]] bool empty() const noexcept
		{
			return count_vectors_.empty();
		}

		[[nodiscard]] const_count_vectors_iterator begin() const noexcept
		{
			return const_count_vectors_iterator(count_vectors_.cbegin());
		}

		[[nodiscard]] const_count_vectors_iterator end() const noexcept
		{
			return const_count_vectors_iterator(count_vectors_.cend());
		}
	};
}