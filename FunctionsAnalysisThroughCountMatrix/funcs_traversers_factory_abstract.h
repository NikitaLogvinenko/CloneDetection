#pragma once
#include "funcs_traverser_abstract.h"

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_traversers_factory_abstract
	{
	public:
		constexpr funcs_traversers_factory_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(funcs_traversers_factory_abstract)

		[[nodiscard]] virtual std::unique_ptr<funcs_traverser_abstract<UsageConditionsCount>> generate() const = 0;
	};
}