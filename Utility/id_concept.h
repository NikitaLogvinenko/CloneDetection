#pragma once
#include <concepts>

namespace utility
{
	template <class T>
	concept id_concept = std::is_constructible_v<T, size_t> && std::equality_comparable<T> &&
		requires(const T id)
	{
		{ id.to_size_t() } -> std::same_as<size_t>;
	};
}