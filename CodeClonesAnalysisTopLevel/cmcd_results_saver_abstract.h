#pragma once
#include "cmcd_results_saver_config.h"
#include "cmcd_results.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <ostream>

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class cmcd_results_saver_abstract
	{
	protected:
		cmcd_results_saver_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(cmcd_results_saver_abstract)

	public:
		virtual void save(std::ostream& output, const cmcd_result<ConditionsCount>& result, const cmcd_results_saver_config& config) const = 0;
	};
}