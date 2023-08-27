#pragma once
#include "clang-c/Index.h"
#include "cxindex_raii.h"
#include "create_translation_unit_exception.h"
#include "copying_delete.h"
#include <filesystem>

namespace clang_c_adaptation
{
	class translation_unit_raii final
	{
		CXTranslationUnit translation_unit_{};

	public:
		explicit translation_unit_raii(const cxindex_raii& cxindex, const std::filesystem::path& ast_filename)
			: translation_unit_{
				clang_createTranslationUnit(cxindex.index(),
				                            ast_filename.string().c_str())
			}
		{
			if (translation_unit_ == invalid_tu)
			{
				throw create_translation_unit_exception(
					"Failure during translation unit creating. AST file: " + ast_filename.string());
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

	private:
		static constexpr auto invalid_tu = nullptr;
	};
}
