#pragma once
#include "translation_unit_raii.h"
#include "code_traverser_sharing_units.h"
#include "funcs_analysis_traits.h"
#include "funcs_definitions_finder.h"

namespace clang_funcs_analysis
{
	class clang_funcs_traverser final :
		public code_analysis_through_cm::code_traverser_sharing_units<
		funcs_analysis_traits, clang_c_adaptation::translation_unit_raii>
	{
	public:
		using translation_unit_raii = clang_c_adaptation::translation_unit_raii;
		using translation_units_container = utility::one_off_container<translation_unit_raii>;
		using var_usage_callback = code_analysis_through_cm::nested_entity_condition_callback<funcs_analysis_traits>;

	private:
		using base = code_traverser_sharing_units;

	public:
		clang_funcs_traverser(std::shared_ptr<translation_units_container> not_traversed_units,
			std::shared_ptr<utility::spinlock> units_container_lock)
			: base(std::move(not_traversed_units), std::move(units_container_lock)) {}

	private:
		void traverse_unit(std::unique_ptr<translation_unit_raii> translation_unit, const var_usage_callback& callback) override;
	};
}
