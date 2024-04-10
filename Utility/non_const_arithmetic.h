#pragma once
#include <type_traits>

namespace utility
{
	template <class T>
	concept non_const_arithmetic = std::is_arithmetic_v<T> && !std::is_const_v<T>;
}
