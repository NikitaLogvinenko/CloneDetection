#pragma once
#include "clang-c/Index.h"
#include "token_index.h"
#include "translation_unit_raii.h"
#include "unsuccessfull_tokenization_exception.h"

//namespace clang_c_adaptation::internal
//{
//	class cxtokens_raii final
//	{
//		translation_unit_raii translation_unit_;
//		CXToken* tokens_{};
//		size_t tokens_n_{};
//
//	public:
//		cxtokens_raii(const cxtokens_raii& other) = delete;
//		cxtokens_raii& operator=(const cxtokens_raii& other) = delete;
//		cxtokens_raii(cxtokens_raii&& other) noexcept
//		{
//			std::swap(*this, other);
//		}
//		cxtokens_raii& operator=(cxtokens_raii&& other) noexcept
//		{
//			if (&other == this)
//			{
//				return *this;
//			}
//			std::swap(*this, other);
//			return *this;
//		}
//		~cxtokens_raii()
//		{
//			clang_disposeTokens(translation_unit_.translation_unit(), tokens_, tokens_n_);
//		}
//
//		[[nodiscard]] const CXTranslationUnit& translation_unit() const noexcept
//		{
//			return translation_unit_;
//		}
//	};
//}