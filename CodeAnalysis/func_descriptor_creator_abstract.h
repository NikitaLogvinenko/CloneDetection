#pragma once
#include "func_id.h"
#include "func_descriptor.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"

namespace code_analysis
{
	class func_descriptor_creator_abstract
	{
	protected:
		constexpr func_descriptor_creator_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(func_descriptor_creator_abstract)

		[[nodiscard]] virtual func_descriptor create(func_id id) const = 0;
	};
}