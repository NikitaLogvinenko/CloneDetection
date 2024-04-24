#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <istream>

namespace clang_ast_dumping
{
	class ast_dumping_config_parser final
	{
		using path = std::filesystem::path;

		class ast_dumping_config final
		{
			path dumps_dir_{};

			std::unordered_map<path, std::vector<std::string>> args_by_source_{};

			std::unordered_set<path> include_dirs_{};
			std::unordered_set<path> libs_dirs_{};
			std::unordered_set<path> libs_{};

			std::optional<std::vector<std::string>> common_args_{};

		public:
			[[nodiscard]] const path& dumps_dir() const noexcept
			{
				return dumps_dir_;
			}

			[[nodiscard]] const std::unordered_map<path, std::vector<std::string>>& args_by_source() const noexcept
			{
				return args_by_source_;
			}

			[[nodiscard]] const std::unordered_set<path>& include_dirs() const noexcept
			{
				return include_dirs_;
			}

			[[nodiscard]] const std::unordered_set<path>& libs_dirs() const noexcept
			{
				return libs_dirs_;
			}

			[[nodiscard]] const std::unordered_set<path>& libs() const noexcept
			{
				return libs_;
			}

			[[nodiscard]] const std::optional<std::vector<std::string>>& common_args() const noexcept
			{
				return common_args_;
			}

			void set_dumps_dir(path dir);
			void add_source(path source, std::vector<std::string> args);
			void add_include_dir(path dir);
			void add_lib_dir(path dir);
			void add_lib(path lib);
			void add_args(std::vector<std::string> args);
		};

	public:
		ast_dumping_config_parser() = delete;

		static ast_dumping_config parse(std::istream& input);

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
