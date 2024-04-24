#include "ast_dumping_director.h"
#include "source_to_ast_arguments_creator.h"

void clang_ast_dumping::ast_dumping_director::dump(std::istream& config_input, ast_dumping_config_parser_abstract& parser,
	ast_dumper_abstract& dumper)
{
	// ReSharper disable once CppTooWideScopeInitStatement
	const ast_dumping_config& config = parser.parse(config_input);

	for (const auto& source_args : source_to_ast_arguments_creator::create(config))
	{
		dumper.dump(source_args);
	}
}
