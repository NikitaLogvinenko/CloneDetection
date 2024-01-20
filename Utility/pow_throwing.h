#pragma once
#include "non_const_arithmetic.h"
#include "overflow_error.h"
#include "nan_error.h"
#include <string>
#include <cmath>

namespace utility
{
	template <non_const_arithmetic BaseT, non_const_arithmetic ExpT>
	BaseT pow_throwing(const BaseT base, const ExpT exp, const std::string& error_msg = {})
	{
		const double pow_result = pow(static_cast<double>(base), static_cast<double>(exp));

		if (std::isnan(pow_result))
		{
			throw common_exceptions::nan_error(error_msg);
		}

		if (pow_result > std::numeric_limits<BaseT>::max() || pow_result < std::numeric_limits<BaseT>::min())
		{
			throw common_exceptions::overflow_error(error_msg);
		}

		return static_cast<BaseT>(pow_result);
	}
}
