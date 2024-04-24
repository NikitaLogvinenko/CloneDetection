#pragma once
#include "ast_dumping_config.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <istream>

namespace clang_ast_dumping
{
	class ast_dumping_config_parser_abstract
	{
	protected:
		ast_dumping_config_parser_abstract() = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(ast_dumping_config_parser_abstract)

	public:
		[[nodiscard]] virtual ast_dumping_config parse(std::istream& input) = 0;
	};
}
