#pragma once
#include "var_usage_condition_descriptor.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"

namespace func_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class functions_implementations_traverser_abstract
	{
	public:
		using vector_of_usage_condition_descriptors = std::vector<var_usage_condition_descriptor<UsageConditionsCount>>;

	private:
		 

	public:
		constexpr functions_implementations_traverser_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(functions_implementations_traverser_abstract)

		virtual void traverse() = 0;

		
	};
}
