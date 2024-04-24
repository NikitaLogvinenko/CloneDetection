#pragma once
#include "ast_dumping_config_parser_abstract.h"
#include <functional>
// ReSharper disable once CppUnusedIncludeDirective
#include <istream>

namespace clang_ast_dumping
{
	class ast_dumping_config_parser_default final : public ast_dumping_config_parser_abstract
	{
		using path = std::filesystem::path;

	public:
		[[nodiscard]] ast_dumping_config parse(std::istream& input) override;

	private:
		template <class DirIterator> requires (std::is_same_v<DirIterator, std::filesystem::directory_iterator> 
		|| std::is_same_v<DirIterator, std::filesystem::recursive_directory_iterator>)
		static std::vector<path> get_nested_paths(DirIterator dir_iterator,
				const std::function<bool(const std::filesystem::directory_entry&)>& predicate)
		{
			std::vector<std::filesystem::path> nested_paths{};

			for (const std::filesystem::directory_entry& nested_entry : dir_iterator)
			{
				if (predicate(nested_entry))
				{
					nested_paths.emplace_back(nested_entry.path());
				}
			}

			return nested_paths;
		}

		static void process_dumps_dir(std::vector<std::string> tokens, ast_dumping_config& config);
		static void process_include_dir(std::vector<std::string> tokens, ast_dumping_config& config);
		static void process_include_dir_recursive(std::vector<std::string> tokens, ast_dumping_config& config);
		static void process_libs_dir(std::vector<std::string> tokens, ast_dumping_config& config);
		static void process_libs_dir_recursive(std::vector<std::string> tokens, ast_dumping_config& config);
		static void process_lib(std::vector<std::string> tokens, ast_dumping_config& config);
		static void process_sources_dir(std::vector<std::string> tokens, ast_dumping_config& config);
		static void process_sources_dir_recursive(std::vector<std::string> tokens, ast_dumping_config& config);
		static void process_common_args(std::vector<std::string> tokens, ast_dumping_config& config);
		static void process_source(std::vector<std::string> tokens, ast_dumping_config& config);

		inline static const std::string dumps_dir_prefix{ "<DumpsDirectory>" };
		inline static const std::string include_dir_prefix{ "<IncludeDirectory>" };
		inline static const std::string include_dir_recursive_prefix{ "<IncludeDirectoryR>" };
		inline static const std::string libs_dir_prefix{ "<LibsDirectory>" };
		inline static const std::string libs_dir_recursive_prefix{ "<LibsDirectoryR>" };
		inline static const std::string lib_prefix{ "<Lib>" };
		inline static const std::string sources_dir_prefix{ "<SourcesDirectory>" };
		inline static const std::string sources_dir_recursive_prefix{ "<SourcesDirectoryR>" };
		inline static const std::string common_args_prefix{ "<CommonArgs>" };
		inline static const std::string source_prefix{ "<Source>" };

		inline static const std::unordered_map<
			std::string, void(*)(std::vector<std::string>, ast_dumping_config&)> tokens_processing_by_prefix
		{
			{dumps_dir_prefix, &process_dumps_dir},
			{include_dir_prefix, &process_include_dir},
			{include_dir_recursive_prefix, &process_include_dir_recursive},
			{libs_dir_prefix, &process_libs_dir},
			{libs_dir_recursive_prefix, &process_libs_dir_recursive},
			{lib_prefix, &process_lib},
			{sources_dir_prefix, &process_sources_dir},
			{sources_dir_recursive_prefix, &process_sources_dir_recursive},
			{common_args_prefix, &process_common_args},
			{source_prefix, &process_source}
		};
	};
}
