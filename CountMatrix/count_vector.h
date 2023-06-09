#pragma once
#include "count_vector_value.h"
#include "typed_index.h"
#include <array>
#include <vector>

namespace count_matrix_ns
{
	class index_of_count_value final : public typed_index
	{
	public:
		index_of_count_value() noexcept = default;
		explicit index_of_count_value(const size_t index) noexcept : typed_index(index) {}
	};

	template <size_t Dimension>
	class count_vector final
	{
		class const_values_iterator final : public std::vector<count_vector_value>::const_iterator
		{
		public:
			explicit const_values_iterator(const std::vector<count_vector_value>::const_iterator& vector_iterator) noexcept
				: std::vector<count_vector_value>::const_iterator(vector_iterator) {}
		};

		std::vector<count_vector_value> count_values_{ std::vector<count_vector_value>(Dimension) };

	public:
		count_vector() noexcept = default;
		explicit count_vector(const std::array<count_vector_value, Dimension>& count_values) : count_values_(count_values.cbegin(), count_values.cend()) {}

		[[nodiscard]] static size_t size() noexcept
		{
			return Dimension;
		}

		[[nodiscard]] count_vector_value operator[](const index_of_count_value value_index) const noexcept
		{
			return count_values_[value_index.to_size_t()];
		}

		[[nodiscard]] count_vector_value at(const index_of_count_value value_index) const
		{
			return count_values_.at(value_index.to_size_t());
		}

		[[nodiscard]] const_values_iterator begin() const noexcept
		{
			return const_values_iterator(count_values_.cbegin());
		}

		[[nodiscard]] const_values_iterator end() const noexcept
		{
			return const_values_iterator(count_values_.cend());
		}
	};

}
