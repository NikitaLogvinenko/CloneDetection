#pragma once
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
#include "funcs_pair_comparing_result.h"
#include "cmcd_results_saver_config.h"

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class clones_filter_abstract
	{
	public:
		
	protected:
		clones_filter_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(clones_filter_abstract)

	public:
		[[nodiscard]] virtual bool clones(const funcs_pair_comparing_result<ConditionsCount>& result, 
			const cmcd_results_saver_config& config) const = 0;
	};
}