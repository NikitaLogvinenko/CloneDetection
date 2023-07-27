#pragma once
#include "translation_unit_wrapper.h"
#include "analysed_functions_info.h"

namespace cpp_code_analysis
{
	template <size_t VarUsageConditionsN>
	class func_implementation_analysis_builder_abstract
	{
	public:
		func_implementation_analysis_builder_abstract() noexcept = default;

		func_implementation_analysis_builder_abstract(
			const func_implementation_analysis_builder_abstract& other) noexcept = default;
		func_implementation_analysis_builder_abstract(
			func_implementation_analysis_builder_abstract&& other) noexcept = default;
		func_implementation_analysis_builder_abstract& operator=(
			const func_implementation_analysis_builder_abstract& other) noexcept = default;
		func_implementation_analysis_builder_abstract& operator=(
			func_implementation_analysis_builder_abstract&& other) noexcept = default;
		virtual ~func_implementation_analysis_builder_abstract() = default;

		virtual void initialize_builder() = 0;
		virtual void analyse_translation_unit(const clang_c_adaptation::translation_unit_wrapper& translation_unit) = 0;
		virtual analysed_functions_info<VarUsageConditionsN> build_analysed_functions_implementation_info() = 0;
		virtual void clear_builder() = 0;
	};
}