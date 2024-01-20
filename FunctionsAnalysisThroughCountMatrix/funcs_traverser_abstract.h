#pragma once
#include "var_usage_condition_descriptor.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_traverser_abstract
	{
	protected:
		constexpr funcs_traverser_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(funcs_traverser_abstract)

		[[nodiscard]] virtual std::vector<var_usage_condition_descriptor<UsageConditionsCount>> traverse() = 0;
	};
}
