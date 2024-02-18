#pragma once
#include "func_implementation_analysis_builder_abstract.h"
#include "translation_unit_raii.h"
#include <memory>

namespace cpp_code_analysis
{
	class func_implementation_analysis_director final
	{
	public:
		func_implementation_analysis_director() = delete;

		template <size_t VarUsageConditionsN>
		[[nodiscard]] static analysed_functions_info<VarUsageConditionsN> analyse_all_units(
			std::unique_ptr<func_implementation_analysis_builder_abstract<VarUsageConditionsN>> abstract_builder_ptr,
			const std::vector<clang_c_adaptation::translation_unit_raii>& translation_units);
	};

	template <size_t VarUsageConditionsN>
	analysed_functions_info<VarUsageConditionsN> func_implementation_analysis_director::analyse_all_units(
		std::unique_ptr<func_implementation_analysis_builder_abstract<VarUsageConditionsN>> abstract_builder_ptr,
		const std::vector<clang_c_adaptation::translation_unit_raii>& translation_units)
	{
		abstract_builder_ptr->initialize_builder();

		for (const auto& translation_unit : translation_units)
		{
			abstract_builder_ptr->analyse_translation_unit(translation_unit);
		}

		auto returned_value = abstract_builder_ptr->build_analysed_functions_implementation_info();

		abstract_builder_ptr->clear_builder();

		return returned_value;
	}
}
