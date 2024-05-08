#pragma once
#include "cmcd_results_saver_abstract.h"
#include "cmcd_config.h"

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class cmcd_results_saver_factory_abstract
	{
	protected:
		constexpr cmcd_results_saver_factory_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(cmcd_results_saver_factory_abstract)

		[[nodiscard]] virtual std::unique_ptr<cmcd_results_saver_abstract<ConditionsCount>> create_saver(const cmcd_config& config) const = 0;
	};
}