#pragma once
#include "count_vector_value.h"
#include <array>

namespace count_matrix
{
	template <size_t Dimension>
	class count_vector final
	{
		class const_iterator final : public std::array<count_vector_value, Dimension>::const_iterator
		{
		public:
			explicit const_iterator(const typename std::array<count_vector_value, Dimension>::const_iterator& array_iterator) noexcept
				: std::array<count_vector_value, Dimension>::const_iterator(array_iterator) {}
		};

		std::array<count_vector_value, Dimension> count_values_{};

	public:
		count_vector() noexcept = default;
		explicit count_vector(const std::array<count_vector_value, Dimension>& count_values) noexcept : count_values_(count_values) {}

		[[nodiscard]] static size_t size() noexcept
		{
			return Dimension;
		}

		[[nodiscard]] count_vector_value operator[](const size_t value_index) const noexcept
		{
			return count_values_[value_index];
		}

		[[nodiscard]] count_vector_value at(const size_t value_index) const
		{
			return count_values_.at(value_index);
		}

		[[nodiscard]] auto begin() const noexcept
		{
			return const_iterator(count_values_.cbegin());
		}

		[[nodiscard]] auto end() const noexcept
		{
			return const_iterator(count_values_.cend());
		}
	};

}
