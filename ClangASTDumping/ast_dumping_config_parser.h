#pragma once
#include "source_to_ast_arguments.h"
#include <vector>
#include <string>
#include <istream>

namespace clang_ast_dumping
{
	class ast_dumping_config_parser final
	{
	public:
		ast_dumping_config_parser() = delete;

		static std::vector<source_to_ast_arguments> parse(std::istream& config);

	private:
		static std::vector<std::string> parse_dirs(std::istream& config, const char* prefix);
	};
}
