#pragma once
#include "count_vector_value.h"
#include "index_of_count_value.h"
#include "iterator_wrapper.h"
#include <array>
#include <vector>

namespace cm
{
	template <size_t Dimension>
	class count_vector final
	{
		std::vector<count_vector_value> count_values_{std::vector<count_vector_value>(Dimension)};

	public:
		count_vector() noexcept = default;

		explicit count_vector(const std::array<count_vector_value, Dimension>& count_values) : count_values_(
			count_values.cbegin(), count_values.cend()) {}

		[[nodiscard]] size_t size() const noexcept
		{
			return count_values_.size();
		}

		[[nodiscard]] const count_vector_value& operator[](
			const internal::index_of_count_value value_index) const
		{
			return count_values_[value_index.to_size_t()];
		}

		[[nodiscard]] const count_vector_value& at(
			const internal::index_of_count_value value_index) const
		{
			return count_values_.at(value_index.to_size_t());
		}

		[[nodiscard]] auto begin() const noexcept
		{
			return code_generation::iterator_wrapper{count_values_.cbegin()};
		}

		[[nodiscard]] auto end() const noexcept
		{
			return code_generation::iterator_wrapper{count_values_.cend()};
		}
	};
}
