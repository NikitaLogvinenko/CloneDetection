#pragma once
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
#include "code_entities_pairwise_comparer_through_cm.h"
#include "funcs_pairwise_comparing_traits.h"
#include "cmcd_config.h"

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class funcs_comparing_through_count_matrix_factory_abstract
	{
	public:
		using traits = funcs_pairwise_comparing_traits<ConditionsCount>;

		using funcs_pairwise_comparer_through_cm = code_clones_analysis_through_cm::code_entities_pairwise_comparer_through_cm<traits>;

	protected:
		funcs_comparing_through_count_matrix_factory_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(funcs_comparing_through_count_matrix_factory_abstract)

	public:
		[[nodiscard]] virtual std::unique_ptr<funcs_pairwise_comparer_through_cm> create_funcs_comparer(
			const cmcd_config& config) const = 0;
	};
}