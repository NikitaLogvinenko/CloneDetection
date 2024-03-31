#pragma once
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
#include "func_implementation_info.h"
#include "condition_index.h"

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_implementations_info_builder_abstract
	{
	protected:
		constexpr funcs_implementations_info_builder_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(funcs_implementations_info_builder_abstract)

		virtual void increment_condition(const code_analysis::func_id& func, const code_analysis::var_id& var, condition_index<UsageConditionsCount> index) = 0;

		[[nodiscard]] virtual std::vector<func_implementation_info<UsageConditionsCount>> build_and_reset() = 0;
	};
}
