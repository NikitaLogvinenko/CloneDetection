#pragma once
#include "translation_unit_wrapper.h"
#include "analysed_functions_info.h"

namespace cpp_code_analysis
{
	template <size_t VarUsageConditionsN>
	class func_implementation_analysis_builder_abstract
	{
	public:
		virtual ~func_implementation_analysis_builder_abstract() = default;

		virtual void analyse_functions(const clang_c_adaptation::translation_unit_wrapper& translation_unit) = 0;
		virtual analysed_functions_info<VarUsageConditionsN> build_analysed_functions_implementation_info() = 0;
	};
}