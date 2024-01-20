#pragma once
#include "clang-c/Index.h"
#include "cxindex_raii.h"
#include "create_translation_unit_error.h"
#include "copying_delete.h"
#include <filesystem>
#include <format>

namespace clang_c_adaptation
{
	class translation_unit_raii final
	{
		CXTranslationUnit translation_unit_{};

		static constexpr auto invalid_translation_unit = nullptr;

	public:
		explicit translation_unit_raii(const cxindex_raii& cxindex, const std::filesystem::path& ast_filename)
			: translation_unit_{
				clang_createTranslationUnit(cxindex.index(),
				                            ast_filename.string().c_str())
			}
		{
			if (translation_unit_ == invalid_translation_unit)
			{
				throw create_translation_unit_error(std::format(
					"translation_unit_raii::translation_unit_raii: failure during translation unit creating. "
					"AST file: {}.", ast_filename.string()));
			}
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
	};
}
