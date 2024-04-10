#pragma once
#include "non_const_arithmetic.h"

namespace utility
{
	template <class T>
	concept non_const_unsigned_arithmetic = non_const_arithmetic<T> && std::is_unsigned_v<T>;
}
