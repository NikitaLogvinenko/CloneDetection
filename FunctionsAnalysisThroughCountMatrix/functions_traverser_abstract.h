#pragma once
#include "var_usage_condition_descriptor.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"

namespace func_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class functions_traverser_abstract
	{
	public:
		using vector_of_usage_conditions_descriptors = std::vector<var_usage_condition_descriptor<UsageConditionsCount>>;

	private:
		vector_of_usage_conditions_descriptors usage_conditions_descriptors_{};

	public:
		constexpr functions_traverser_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(functions_traverser_abstract)

		virtual void traverse() = 0;

		[[nodiscard]] constexpr vector_of_usage_conditions_descriptors&& usage_conditions_descriptors() && noexcept
		{
			return std::move(usage_conditions_descriptors_);
		}
	};
}
