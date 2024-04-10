#pragma once
#include "non_const_arithmetic.h"

namespace utility
{
	template <class T>
	concept non_const_integral = non_const_arithmetic<T> && std::is_integral_v<T>;
}