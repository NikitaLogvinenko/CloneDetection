#pragma once
#include "func_descriptor.h"
#include "var_descriptor.h"
#include "condition_index.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
#include <functional>

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_traverser_abstract
	{
	public:
		using condition_index = condition_index<UsageConditionsCount>;

		using var_usage_callback = std::function<
			void(const code_analysis::func_descriptor&, const code_analysis::var_descriptor&, condition_index)>;

	protected:
		constexpr funcs_traverser_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(funcs_traverser_abstract)

		virtual void traverse(const var_usage_callback& callback) = 0;
	};
}
