#pragma once
#include <concepts>

namespace utility
{
	template <class T>
	concept id_concept = std::is_constructible_v<T, size_t>;
}