#pragma once
#include "var_usage_condition_descriptor.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"

namespace functions_analysis
{
	class functions_traverser_abstract
	{
	public:
		constexpr functions_traverser_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(functions_traverser_abstract)


	};
}
