#pragma once
#include "ast_dumping_config_parser_abstract.h"
#include "ast_dumper_abstract.h"

namespace clang_ast_dumping
{
	class ast_dumping_director final
	{
	public:
		ast_dumping_director() = delete;

		static void dump(std::istream& config_input, ast_dumping_config_parser_abstract& parser, ast_dumper_abstract& dumper);
	};
}