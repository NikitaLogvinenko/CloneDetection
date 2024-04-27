#pragma once
#include "cmcd_config.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <iostream>

namespace code_clones_analysis_top_level
{
	class cmcd_config_parser_abstract
	{
	protected:
		constexpr cmcd_config_parser_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(cmcd_config_parser_abstract)

		[[nodiscard]] virtual cmcd_config parse(std::istream& input) const = 0;
	};
}