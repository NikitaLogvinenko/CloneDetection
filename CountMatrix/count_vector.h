#pragma once
#include "counted_value.h"
#include "index_of_counted_value.h"
#include <array>
#include <vector>

namespace cm
{
	template <size_t Dimension>
	class count_vector final
	{
		std::vector<counted_value> counted_values_{std::vector<counted_value>(Dimension)};

	public:
		count_vector() = default;

		explicit count_vector(const std::array<counted_value, Dimension>& counted_values) : counted_values_(
			counted_values.cbegin(), counted_values.cend()) {}

		[[nodiscard]] size_t size() const noexcept
		{
			return counted_values_.size();
		}

		[[nodiscard]] const counted_value& operator[](
			const internal::index_of_counted_value value_index) const
		{
			return counted_values_[value_index.to_size_t()];
		}

		[[nodiscard]] const counted_value& at(
			const internal::index_of_counted_value value_index) const
		{
			return counted_values_.at(value_index.to_size_t());
		}

		[[nodiscard]] auto begin() const noexcept
		{
			return counted_values_.cbegin();
		}

		[[nodiscard]] auto end() const noexcept
		{
			return counted_values_.cend();
		}
	};
}
