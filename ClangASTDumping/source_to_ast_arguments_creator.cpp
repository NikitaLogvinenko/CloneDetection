#include "source_to_ast_arguments_creator.h"
#include "input_format_error.h"

namespace
{
	const std::string include_dir_option = "-I";
	const std::string libs_dir_option = "-L";
	const std::string lib_option = "-l";

	[[nodiscard]] size_t calculate_total_args_count(const std::vector<std::string>& specific_args, 
		const clang_ast_dumping::ast_dumping_config& config)
	{
		const size_t specific_args_count = specific_args.size();
		const size_t include_dirs_args_count = config.include_dirs().size() * 2;
		const size_t libs_dirs_args_count = config.libs_dirs().size() * 2;
		const size_t libs_args_count = config.libs().size() * 2;
		const size_t common_args_count = config.common_args() ? config.common_args().value().size() : 0;

		return specific_args_count + include_dirs_args_count + libs_dirs_args_count + libs_args_count + common_args_count;
	}
}

std::vector<clang_ast_dumping::source_to_ast_arguments> clang_ast_dumping::source_to_ast_arguments_creator::create(
	const ast_dumping_config& config)
{
	std::vector<source_to_ast_arguments> result{};
	result.reserve(config.args_by_source().size());

	for (const auto& [source, specific_args] : config.args_by_source())
	{
		const size_t args_count = calculate_total_args_count(specific_args, config);
		std::vector<std::string> args{};
		args.reserve(args_count);

		args.insert(args.cend(), specific_args.cbegin(), specific_args.cend());

		for (const auto& include_dir : config.include_dirs())
		{
			args.emplace_back(include_dir_option);
			args.emplace_back(include_dir.string());
		}

		for (const auto& libs_dir : config.include_dirs())
		{
			args.emplace_back(libs_dir_option);
			args.emplace_back(libs_dir.string());
		}

		for (const auto& lib : config.include_dirs())
		{
			args.emplace_back(lib_option);
			args.emplace_back(lib.string());
		}

		if (config.common_args())
		{
			args.insert(args.cend(), 
				config.common_args().value().cbegin(),
				config.common_args().value().cend());
		}

		if (config.dumps_dir().empty())
		{
			throw common_exceptions::input_format_error{ "source_to_ast_arguments_creator::create: dumps directory wasn't provided." };
		}
		create_directories(config.dumps_dir());

		std::filesystem::path ast_path{ (config.dumps_dir() / source.stem()).string() + ".ast" };

		result.emplace_back(source, std::move(ast_path), std::move(args));
	}

	return result;
}
