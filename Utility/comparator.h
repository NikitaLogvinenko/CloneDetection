#pragma once
#include <concepts>

namespace utility
{
	template <class T, class Comp>
	concept comparator = requires (Comp comp, const T& lhs, const T& rhs)
	{
		{ comp(lhs, rhs) } -> std::same_as<bool>;
	};
}
