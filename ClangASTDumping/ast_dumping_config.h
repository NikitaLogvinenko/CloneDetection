#pragma once
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <optional>

namespace clang_ast_dumping
{
	class ast_dumping_config final
	{
		using path = std::filesystem::path;

		path dumps_dir_{};

		std::unordered_map<path, std::vector<std::string>> args_by_source_{};

		std::unordered_set<path> include_dirs_{};
		std::unordered_set<path> libs_dirs_{};
		std::unordered_set<std::string> libs_{};

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

		[[nodiscard]] const std::unordered_set<std::string>& libs() const noexcept
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
		void add_lib(std::string lib);
		void add_args(std::vector<std::string> args);
	};
}

