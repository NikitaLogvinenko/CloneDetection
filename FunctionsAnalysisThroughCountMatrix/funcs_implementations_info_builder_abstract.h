#pragma once
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
#include "func_implementation_info.h"
#include "func_descriptor.h"
#include "var_descriptor.h"
#include "condition_index.h"

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_implementations_info_builder_abstract
	{
	public:
		using code_analysis::func_descriptor;
		using code_analysis::var_descriptor;

		using func_implementation_info = func_implementation_info<UsageConditionsCount>;

		using condition_index = condition_index<UsageConditionsCount>;

	protected:
		constexpr funcs_implementations_info_builder_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(funcs_implementations_info_builder_abstract)

		virtual void add_condition(const func_descriptor& func, const var_descriptor& var, condition_index index) = 0;

		[[nodiscard]] virtual std::vector<func_implementation_info> build_and_reset() = 0;
	};
}
