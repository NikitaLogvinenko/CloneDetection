#pragma once
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
#include "code_entity_implementation_info.h"
#include "condition_index.h"

namespace code_analysis_through_cm
{
	template <code_analysis_traits AnalysisTraits>
	class code_implementation_info_builder_abstract
	{
	public:
		using analyzed_entity_id = typename AnalysisTraits::AnalyzedEntityId;
		using nested_entity_id = typename AnalysisTraits::NestedEntityId;

	protected:
		constexpr code_implementation_info_builder_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(code_implementation_info_builder_abstract)

		virtual void increment_condition(const analyzed_entity_id& analyzed_entity_id, const nested_entity_id& nested_entity_id, 
			condition_index<AnalysisTraits::conditions_count> condition) = 0;

		[[nodiscard]] virtual std::vector<code_entity_implementation_info<AnalysisTraits>> build_and_reset() = 0;
	};
}
