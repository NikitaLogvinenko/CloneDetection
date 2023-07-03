#pragma once
#include "func_implementation_info.h"

namespace cpp_code_analysis
{
	template <size_t VarUsageConditionsN>
	class func_implementation_info_equal_spelling_and_location final
	{
	public:
		[[nodiscard]] bool operator()(
			const func_implementation_info<VarUsageConditionsN>& first_func_info, 
			const func_implementation_info<VarUsageConditionsN>& second_func_info) const noexcept
		{
			return first_func_info.spelling() == second_func_info.spelling() && first_func_info.location() == second_func_info.location();
		}
	};
}