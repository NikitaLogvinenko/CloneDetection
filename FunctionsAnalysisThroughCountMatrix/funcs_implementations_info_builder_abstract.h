#pragma once
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
#include "func_implementation_info.h"
#include "var_usage_condition_descriptor.h"

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_implementations_info_builder_abstract
	{
	protected:
		constexpr funcs_implementations_info_builder_abstract() noexcept = default;

	public:
		using func_info = func_implementation_info<UsageConditionsCount>;


		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(funcs_implementations_info_builder_abstract)

		virtual void add_condition(var_usage_condition_descriptor<UsageConditionsCount> usage_condition) = 0;

		[[nodiscard]] virtual std::vector<func_info> build_and_reset() = 0;
	};
}
