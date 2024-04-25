#pragma once
#include "count_matrix_clones_detection_config.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
#include <iostream>

namespace code_clones_analysis_top_level
{
	class cmcd_config_parser_abstract
	{
	protected:
		cmcd_config_parser_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(cmcd_config_parser_abstract)

	public:
		[[nodiscard]] virtual count_matrix_clones_detection_config parse(std::istream& input) = 0;
	};
}