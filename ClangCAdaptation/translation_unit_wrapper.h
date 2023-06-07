#pragma once
#include "clang-c/Index.h"
#include "cxindex_wrapper.h"
#include <filesystem>
#include <stdexcept>

namespace clang_c_adaptation
{
	class translation_unit_wrapper final
	{
		CXTranslationUnit translation_unit_{};

		inline static const std::string ctor_error_msg{ "Failure during translation unit creation." };

	public:
		explicit translation_unit_wrapper(const cxindex_wrapper& cxindex_wrapper, const std::filesystem::path& ast_filename)
			: translation_unit_{ clang_createTranslationUnit(cxindex_wrapper.index(), ast_filename.string().c_str()) }
		{
			if (translation_unit_ == nullptr)
			{
				throw std::runtime_error(ctor_error_msg + " AST file: " + ast_filename.string());
			}
		}

		translation_unit_wrapper(const translation_unit_wrapper& other) = default;
		translation_unit_wrapper(translation_unit_wrapper&& other) = default;
		translation_unit_wrapper& operator=(const translation_unit_wrapper& other) = default;
		translation_unit_wrapper& operator=(translation_unit_wrapper&& other) = default;
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