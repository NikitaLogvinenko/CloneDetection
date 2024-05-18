#pragma once
#include "var_id.h"
#include "func_id.h"

namespace CLR
{
	template <size_t ConditionsCount>
	struct FuncsAnalysisTraits final
	{
		using analyzed_entity_id = code_analysis::func_id;
		using nested_entity_id = code_analysis::var_id;
		static constexpr size_t conditions_count = ConditionsCount;
	};
}