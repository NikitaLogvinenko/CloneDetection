#pragma once
#include "functions_analysis_builder_abstract.h"
#include "translation_unit_wrapper.h"
#include <memory>
#include <functional>

namespace cpp_code_analysis
{
	class functions_analysis_director final
	{
	public:
		functions_analysis_director() = delete;

		template <size_t VarUsageConditionsN>
		[[nodiscard]] static auto analyse_all_units(std::unique_ptr<functions_analysis_builder_abstract<VarUsageConditionsN>> abstract_builder_ptr,
			const std::vector<std::reference_wrapper<clang_c_adaptation::translation_unit_wrapper>>& translation_units);
	};

	template <size_t VarUsageConditionsN>
	auto functions_analysis_director::analyse_all_units(
		std::unique_ptr<functions_analysis_builder_abstract<VarUsageConditionsN>> abstract_builder_ptr,
		const std::vector<std::reference_wrapper<clang_c_adaptation::translation_unit_wrapper>>& translation_units)
	{
		abstract_builder_ptr->clear_analysed_data();
		for (const auto& translation_unit_wrapper_ref : translation_units)
		{
			abstract_builder_ptr->analyse_functions(translation_unit_wrapper_ref);
		}
		return abstract_builder_ptr->build_analysed_functions_implementation_info();
	}
}
