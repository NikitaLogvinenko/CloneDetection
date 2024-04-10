#pragma once
#include <concepts>

namespace utility
{
	template <class T, class Comp>
	concept strict_weak_order = std::strict_weak_order<Comp, T, T>;
}
