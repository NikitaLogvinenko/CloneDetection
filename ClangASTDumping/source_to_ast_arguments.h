#pragma once
#include <string>
#include <vector>
#include <filesystem>

namespace clang_ast_dumping
{
	class source_to_ast_arguments final
	{
		std::filesystem::path source_path_{};
		std::vector<std::string> command_line_args_{};

	public:
		source_to_ast_arguments() noexcept = default;

		source_to_ast_arguments(std::filesystem::path source_path, std::vector<std::string> command_line_args) noexcept
			: source_path_(std::move(source_path)), command_line_args_(std::move(command_line_args)) {}

		[[nodiscard]] const std::filesystem::path& source_path() const noexcept
		{
			return source_path_;
		}

		[[nodiscard]] const std::vector<std::string>& command_line_args() const noexcept
		{
			return command_line_args_;
		}
	};
}