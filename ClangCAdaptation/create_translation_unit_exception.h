#pragma once
#include <string>
#include <stdexcept>
#include <filesystem>

namespace clang_c_adaptation
{
	class create_translation_unit_exception final : std::runtime_error
	{
		inline static const std::string ctor_error_msg{ "Failure during translation unit creation. AST file: " };
	public:
		explicit create_translation_unit_exception(const std::filesystem::path& ast_filename)
		: std::runtime_error(ctor_error_msg + ast_filename.string()) {}
	};
}