#pragma once
#include "conditions_analyzers_factory_abstract.h"
#include "funcs_analysis_traits.h"
#include "var_usage_conditions.h"
#include "vars_usage_analyzers_factory_default.h"

namespace clang_code_analysis
{
	class vars_usage_analyzers_factory_default final
		: public conditions_analyzers_factory_abstract<code_analysis_through_cm::funcs_analysis_traits<var_usage_conditions_total>>
	{
	public:
		static constexpr size_t conditions_count = var_usage_conditions_total;
		using funcs_analysis_traits = code_analysis_through_cm::funcs_analysis_traits<conditions_count>;
		using condition_analyzer_abstract = condition_analyzer_abstract<funcs_analysis_traits>;
		using conditions_analyzers_vector = std::vector<std::unique_ptr<condition_analyzer_abstract>>;

		[[nodiscard]] conditions_analyzers_vector create_analyzers() override;
	};
}