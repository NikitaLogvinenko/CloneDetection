#pragma once
#include "clang-c/Index.h"
#include "cxindex_raii.h"
#include "create_translation_unit_exception.h"
#include <filesystem>

namespace clang_c_adaptation
{
	class translation_unit_raii final
	{
		CXTranslationUnit translation_unit_{};

		static constexpr auto invalid_tu = nullptr;

	public:
		explicit translation_unit_raii(const cxindex_raii& cxindex_wrapper, const std::filesystem::path& ast_filename)
			: translation_unit_{ clang_createTranslationUnit(cxindex_wrapper.index(), 
				ast_filename.string().c_str()) }
		{
			if (translation_unit_ == invalid_tu)
			{
				throw create_translation_unit_exception(
					"Failure during translation unit creation. AST file: " + ast_filename.string());
			}
		}

		translation_unit_raii(const translation_unit_raii& other) = delete;
		translation_unit_raii& operator=(const translation_unit_raii& other) = delete;
		translation_unit_raii(translation_unit_raii&& other) noexcept : translation_unit_(other.translation_unit_)
		{
			other.translation_unit_ = nullptr;
		}
		translation_unit_raii& operator=(translation_unit_raii&& other) noexcept
		{
			if (&other == this)
			{
				return *this;
			}
			translation_unit_ = other.translation_unit_;
			other.translation_unit_ = nullptr;
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