#pragma once
#include "clang-c/Index.h"
#include "cxindex_wrapper.h"
#include "create_translation_unit_exception.h"

namespace clang_c_adaptation
{
	class translation_unit_wrapper final
	{
		CXTranslationUnit translation_unit_{};

		static constexpr auto invalid_tu = nullptr;

	public:
		explicit translation_unit_wrapper(const cxindex_wrapper& cxindex_wrapper, const std::filesystem::path& ast_filename)
			: translation_unit_{ clang_createTranslationUnit(cxindex_wrapper.index(), ast_filename.string().c_str()) }
		{
			if (translation_unit_ == invalid_tu)
			{
				throw create_translation_unit_exception(ast_filename);
			}
		}

		translation_unit_wrapper(const translation_unit_wrapper& other) = delete;
		translation_unit_wrapper& operator=(const translation_unit_wrapper& other) = delete;
		translation_unit_wrapper(translation_unit_wrapper&& other) noexcept : translation_unit_(other.translation_unit_)
		{
			other.translation_unit_ = nullptr;
		}
		translation_unit_wrapper& operator=(translation_unit_wrapper&& other) noexcept
		{
			if (&other == this)
			{
				return *this;
			}
			translation_unit_ = other.translation_unit_;
			other.translation_unit_ = nullptr;
			return *this;
		}
		~translation_unit_wrapper()
		{
			clang_disposeTranslationUnit(translation_unit_);
		}

		[[nodiscard]] const CXTranslationUnit& translation_unit() const noexcept
		{
			return translation_unit_;
		}
	};
}