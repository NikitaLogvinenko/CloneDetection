#pragma once
#include "nested_entity_condition_callback.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"

namespace code_analysis_through_cm
{
	template <code_analysis_traits AnalysisTraits>
	class code_traverser_abstract
	{
	protected:
		constexpr code_traverser_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(code_traverser_abstract)

		virtual void traverse(nested_entity_condition_callback<AnalysisTraits> callback) = 0;
	};
}
