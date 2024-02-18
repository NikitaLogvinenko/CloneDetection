#pragma once
#include "func_implementation_info.h"
#include <functional>

namespace cpp_code_analysis::internal
{
	template <size_t VarUsageConditionsN>
	class func_implementation_info_hash final
	{
	public:
		[[nodiscard]] size_t operator()(const func_implementation_info<VarUsageConditionsN>& func_info) const noexcept
		{
			return std::hash<std::string>
				{}(func_info.spelling().to_code_entity_spelling().to_string() 
					+ func_info.location().to_code_entity_location().filename().string());
		}
	};
}
