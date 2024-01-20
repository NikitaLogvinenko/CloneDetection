#pragma once
#include "translation_unit_raii.h"
#include "funcs_traverser_sharing_units.h"

namespace clang_funcs_analysis
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_traverser_clang final :
		public funcs_analysis_through_cm::funcs_traverser_sharing_units<UsageConditionsCount, clang_c_adaptation::translation_unit_raii>
	{
	public:
		using translation_unit_raii = clang_c_adaptation::translation_unit_raii;
		using translation_units_container = utility::disposable_container<translation_unit_raii>;
		using var_usage_condition_descriptor = funcs_analysis_through_cm::var_usage_condition_descriptor<UsageConditionsCount>;
		using traverse_results = std::vector<var_usage_condition_descriptor>;

	private:
		using base = funcs_analysis_through_cm::funcs_traverser_sharing_units<UsageConditionsCount, translation_unit_raii>;

	public:
		funcs_traverser_clang(std::shared_ptr<translation_units_container> not_traversed_units,
			std::shared_ptr<std::mutex> units_container_mutex)
		: base(std::move(not_traversed_units), std::move(units_container_mutex)) {}

		[[nodiscard]] traverse_results traverse_unit(std::unique_ptr<translation_unit_raii> translation_unit) override
		{
			
		}
	};
}