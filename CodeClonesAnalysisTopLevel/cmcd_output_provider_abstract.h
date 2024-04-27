#pragma once
#include "cmcd_config.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <ostream>

namespace code_clones_analysis_top_level
{
	class cmcd_output_provider_abstract
	{
	protected:
		constexpr cmcd_output_provider_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(cmcd_output_provider_abstract)

		virtual void init(const cmcd_config& config) = 0;

		[[nodiscard]] virtual std::ostream& output() = 0;
	};
}