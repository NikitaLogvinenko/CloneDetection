#pragma once
#include "translation_unit_raii.h"
#include "analysed_functions_info.h"
#include "default_copy_move_constructor_assignment_virtual_destructor.h"

namespace cpp_code_analysis
{
	template <size_t VarUsageConditionsN>
	class func_implementation_analysis_builder_abstract
	{
	public:
		func_implementation_analysis_builder_abstract() noexcept = default;

		DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR(func_implementation_analysis_builder_abstract)

		virtual void initialize_builder() = 0;
		virtual void analyse_translation_unit(const clang_c_adaptation::translation_unit_raii& translation_unit) = 0;
		virtual analysed_functions_info<VarUsageConditionsN> build_analysed_functions_implementation_info() = 0;
		virtual void clear_builder() = 0;
	};
}
