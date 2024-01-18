#pragma once
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
#include "funcs_pairwise_comparing_result.h"
#include "func_implementation_info.h"

namespace funcs_comparing_through_cm
{
	template <funcs_comparing_traits CompTraits>
	class funcs_pairwise_comparing_abstract
	{
		using func_implementation_info = funcs_analysis_through_cm::func_implementation_info<CompTraits::count_vector_length>;

	public:
		constexpr funcs_pairwise_comparing_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(funcs_pairwise_comparing_abstract)

		[[nodiscard]] virtual funcs_pairwise_comparing_result<CompTraits> compare(
			const std::vector<func_implementation_info>& first_set_of_functions, 
			const std::vector<func_implementation_info>& second_set_of_functions) const = 0;
	};
}
