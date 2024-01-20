#pragma once
#include "funcs_traverser_abstract.h"

namespace clang_funcs_analysis
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_traverser_clang final : public funcs_analysis_through_cm::funcs_traverser_abstract<UsageConditionsCount>
	{
		
	};
}