#pragma once
#include "code_analysis_traits.h"
#include "var_id.h"
#include "func_id.h"

namespace code_analysis_through_cm
{
	template <size_t ConditionsCount> requires cm::count_vector_length<ConditionsCount>
	struct funcs_analysis_traits final
	{
		funcs_analysis_traits() = delete;

		using analyzed_entity_id = code_analysis::func_id;
		using nested_entity_id = code_analysis::var_id;
		static constexpr size_t conditions_count = ConditionsCount;
	};
}