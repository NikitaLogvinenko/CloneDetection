#pragma once
#include "var_id.h"
#include "var_descriptor.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"

namespace code_analysis
{
	class var_descriptor_creator_abstract
	{
	protected:
		constexpr var_descriptor_creator_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(var_descriptor_creator_abstract)

		[[nodiscard]] virtual var_descriptor create(var_id id) const = 0;
	};
}