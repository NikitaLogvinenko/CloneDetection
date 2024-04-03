#pragma once
#include "code_analysis_traits.h"
#include "var_usage_conditions.h"
#include "var_id.h"
#include "func_id.h"

namespace clang_funcs_analysis
{
	struct funcs_analysis_traits final
	{
		funcs_analysis_traits() = delete;

		using analyzed_entity_id = code_analysis::func_id;
		using nested_entity_id = code_analysis::var_id;
		static constexpr size_t conditions_count = var_usage_conditions_total;
	};

	static_assert(code_analysis_through_cm::code_analysis_traits<funcs_analysis_traits>);
}