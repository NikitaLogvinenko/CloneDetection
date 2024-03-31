#pragma once
#include "translation_unit_raii.h"
#include "code_traverser_sharing_units.h"
#include "funcs_definitions_finder.h"

namespace clang_funcs_analysis
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_traverser_clang final :
		public code_analysis_through_cm::code_traverser_sharing_units<
			UsageConditionsCount, clang_c_adaptation::translation_unit_raii>
	{
	public:
		using translation_unit_raii = clang_c_adaptation::translation_unit_raii;
		using translation_units_container = utility::one_off_container<translation_unit_raii>;
		using traverse_results = std::vector<var_usage_condition_descriptor>;

	private:
		using base = code_analysis_through_cm::code_traverser_sharing_units<UsageConditionsCount, translation_unit_raii>;

		using var_usage_callback = typename base::var_usage_callback;

	public:
		funcs_traverser_clang(std::shared_ptr<translation_units_container> not_traversed_units,
		                      std::shared_ptr<std::mutex> units_container_mutex)
			: base(std::move(not_traversed_units), std::move(units_container_mutex)) {}


	private:
		void traverse_unit(std::unique_ptr<translation_unit_raii> translation_unit, const var_usage_callback& callback) override;
	};
}
