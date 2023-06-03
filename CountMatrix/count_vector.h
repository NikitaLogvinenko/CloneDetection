#pragma once
#include "count_vector_value.h"
#include <array>

template <size_t Dimension>
class count_vector final
{
	class const_iterator final : public std::array<count_vector_value, Dimension>::const_iterator
	{
	public:
		explicit const_iterator(const typename std::array<count_vector_value, Dimension>::const_iterator& array_iterator) noexcept
			: std::array<count_vector_value, Dimension>::const_iterator(array_iterator) {}
	};

	std::array<count_vector_value, Dimension> conditions_{};

public:
	count_vector() noexcept = default;
	explicit count_vector(const std::array<count_vector_value, Dimension>& usage_conditions) noexcept : conditions_(usage_conditions) {}

	[[nodiscard]] auto begin() const noexcept
	{
		return const_iterator(conditions_.begin());
	}

	[[nodiscard]] auto end() const noexcept
	{
		return const_iterator(conditions_.end());
	}
};
