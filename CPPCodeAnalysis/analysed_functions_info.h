#pragma once
#include "func_implementation_info_hash.h"
#include "func_implementation_info_equal_spelling_and_location.h"
#include "analysed_func_info_index.h"
#include <unordered_set>

namespace cpp_code_analysis
{
	template <size_t VarUsageConditionsN>
	class analysed_functions_info final
	{
		std::vector<func_implementation_info<VarUsageConditionsN>> functions_info_{};

	public:
		analysed_functions_info() = default;
		explicit analysed_functions_info(
			const std::unordered_set<
				func_implementation_info<VarUsageConditionsN>,
				func_implementation_info_hash<VarUsageConditionsN>,
				func_implementation_info_equal_spelling_and_location<VarUsageConditionsN>>& functions_info)
			: functions_info_(functions_info.cbegin(), functions_info.cend()) {}

		[[nodiscard]] size_t functions_count() const noexcept
		{
			return functions_info_.size();
		}

		[[nodiscard]] const func_implementation_info<VarUsageConditionsN>& operator[](const analysed_func_info_index func_info_index) const noexcept
		{
			return functions_info_[func_info_index.to_size_t()];
		}

		[[nodiscard]] const func_implementation_info<VarUsageConditionsN>& at(const analysed_func_info_index func_info_index) const
		{
			return functions_info_.at(func_info_index.to_size_t());
		}
	};
}