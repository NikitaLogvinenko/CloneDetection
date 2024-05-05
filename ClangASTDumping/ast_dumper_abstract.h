#pragma once
#include "source_to_ast_arguments.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"

namespace clang_ast_dumping
{
	class ast_dumper_abstract
	{
	protected:
		constexpr ast_dumper_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(ast_dumper_abstract)

		virtual void dump(const source_to_ast_arguments&) = 0;
	};
}
