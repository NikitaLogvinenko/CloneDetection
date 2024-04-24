#include "ast_dumping_config.h"
#include "invalid_operation_error.h"

void clang_ast_dumping::ast_dumping_config::set_dumps_dir(path dir)
{
	if (!dumps_dir_.empty())
	{
		throw common_exceptions::invalid_operation_error{ std::format(
		"::ast_dumping_config::set_dumps_dir: Can not reassign dumps directory. Previously"
  "set directory: {}, reassigning directory: {}.", dumps_dir_.string(), dir.string()) };
	}

	std::swap(dumps_dir_, dir);
}

void clang_ast_dumping::ast_dumping_config::add_source(path source, std::vector<std::string> args)
{
	if (args_by_source_.contains(source))
	{
		throw common_exceptions::invalid_operation_error(std::format(
			"::ast_dumping_config::add_source: {} arguments were set already, can not rewrite",
			source.string()));
	}

	args_by_source_.try_emplace(std::move(source), std::move(args));
}

void clang_ast_dumping::ast_dumping_config::add_include_dir(path dir)
{
	include_dirs_.emplace(std::move(dir));
}

void clang_ast_dumping::ast_dumping_config::add_lib_dir(path dir)
{
	libs_dirs_.emplace(std::move(dir));
}

void clang_ast_dumping::ast_dumping_config::add_lib(path lib)
{
	libs_.emplace(std::move(lib));
}

void clang_ast_dumping::ast_dumping_config::add_args(std::vector<std::string> args)
{
	if (!common_args_)
	{
		common_args_ = args;
		return;
	}

	args.insert(args.cend(), std::make_move_iterator(args.begin()), std::make_move_iterator(args.end()));
}
