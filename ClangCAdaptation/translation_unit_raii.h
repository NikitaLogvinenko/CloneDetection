#pragma once
#include "clang-c/Index.h"
#include "cxindex_raii.h"
#include "translation_unit_error.h"
#include "copying_delete.h"
#include "command_line_args_raii.h"
#include <filesystem>
#include <format>

namespace clang_c_adaptation
{
	class translation_unit_raii final
	{
		CXTranslationUnit translation_unit_{};

		static constexpr auto invalid_translation_unit = nullptr;

	public:
		explicit translation_unit_raii(const cxindex_raii& cxindex, 
			const std::filesystem::path& source_filename,
			const std::vector<std::string>& command_line_args)
			: translation_unit_{create_from_source(cxindex, source_filename, command_line_args)}
		{
			throw_if_invalid_translation_unit(std::format(
				"translation_unit_raii::translation_unit_raii: failure during translation unit creating. "
				"Source file: {}.", source_filename.string()));
		}

		explicit translation_unit_raii(const cxindex_raii& cxindex, const std::filesystem::path& ast_filename)
			: translation_unit_{
				clang_createTranslationUnit(cxindex.index(),
				                            ast_filename.string().c_str())
			}
		{
			throw_if_invalid_translation_unit(std::format(
				"translation_unit_raii::translation_unit_raii: failure during translation unit creating. "
				"AST file: {}.", ast_filename.string()));
		}

		COPYING_DELETE(translation_unit_raii)

		translation_unit_raii(translation_unit_raii&& other) noexcept
		{
			std::swap(translation_unit_, other.translation_unit_);
		}

		translation_unit_raii& operator=(translation_unit_raii&& other) noexcept
		{
			std::swap(translation_unit_, other.translation_unit_);
			return *this;
		}

		~translation_unit_raii()
		{
			clang_disposeTranslationUnit(translation_unit_);
		}

		[[nodiscard]] const CXTranslationUnit& translation_unit() const noexcept
		{
			return translation_unit_;
		}

	private:
		[[nodiscard]] static CXTranslationUnit create_from_source(const cxindex_raii& cxindex,
			const std::filesystem::path& source_filename, const std::vector<std::string>& command_line_args)
		{
			const command_line_args_raii args_raii{ command_line_args };

			return clang_createTranslationUnitFromSourceFile(
				cxindex.index(), source_filename.string().c_str(),
				static_cast<int>(args_raii.argc()), args_raii.argv(), 
				0, nullptr);
		}

		void throw_if_invalid_translation_unit(const std::string& description) const
		{
			if (translation_unit_ == invalid_translation_unit)
			{
				throw translation_unit_error{ description };
			}
		}
	};
}
