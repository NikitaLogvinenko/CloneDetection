#pragma once
#include "funcs_implementations_info_builder_abstract.h"
#include "functions_traversers_factory_abstract.h"

namespace func_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_implementations_info_director_abstract
	{
	public:
		using func_info = func_implementation_info<UsageConditionsCount>;
		using abstract_builder = funcs_implementations_info_builder_abstract<UsageConditionsCount>;
		using abstract_functions_traversers_factory = functions_traversers_factory_abstract<UsageConditionsCount>;

		constexpr funcs_implementations_info_director_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(funcs_implementations_info_director_abstract)

		[[nodiscard]] virtual std::vector<func_info> analyse_functions_implementations(std::unique_ptr<abstract_builder> builder, 
			std::unique_ptr<abstract_functions_traversers_factory> functions_traversers_factory) = 0;
	};
}
