#include "ast_dumping_config_parser.h"

namespace
{
	const char* const include_dirs_prefix = "I: ";
	const char* const include_dirs_recursive_prefix = "IR: ";

	const char* const libraries_dirs_prefix = "L: ";
	const char* const libraries_dirs_recursive_prefix = "LR: ";

	const char* const source_dirs_prefix = "S: ";
	const char* const source_dirs_recursive_prefix = "SR: ";

	const char* const dumps_dir_prefix = "";
}

namespace clang_ast_dumping
{
	std::vector<source_to_ast_arguments> ast_dumping_config_parser::parse(std::istream& config)
	{

	}

	std::vector<std::string> ast_dumping_config_parser::parse_dirs(
		std::istream& config, const char* const prefix)
	{

	}
}
