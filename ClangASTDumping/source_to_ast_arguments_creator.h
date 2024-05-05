#pragma once
#include "ast_dumping_config.h"
#include "source_to_ast_arguments.h"

namespace clang_ast_dumping
{
	class source_to_ast_arguments_creator final
	{
	public:
		source_to_ast_arguments_creator() = delete;

		[[nodiscard]] static std::vector<source_to_ast_arguments> create(const ast_dumping_config& config);
	};
}
