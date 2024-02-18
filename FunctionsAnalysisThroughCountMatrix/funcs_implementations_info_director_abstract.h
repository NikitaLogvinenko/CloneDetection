#pragma once
#include "funcs_implementations_info_builder_abstract.h"
#include "funcs_traversers_factory_abstract.h"

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_implementations_info_director_abstract
	{
	protected:
		constexpr funcs_implementations_info_director_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(funcs_implementations_info_director_abstract)

		[[nodiscard]] virtual std::vector<func_implementation_info<UsageConditionsCount>> analyse_functions_implementations(
			std::unique_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>> builder,
			std::unique_ptr<funcs_traversers_factory_abstract<UsageConditionsCount>> functions_traversers_factory) const = 0;
	};
}
