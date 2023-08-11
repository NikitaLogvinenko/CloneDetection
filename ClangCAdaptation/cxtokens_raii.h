﻿#pragma once
#include "clang-c/Index.h"
#include "token_index.h"
#include "invalid_index_exception.h"
#include "cxstring_raii.h"

namespace clang_c_adaptation::internal
{
	class cxtokens_raii final
	{
		CXToken* tokens_{};
		unsigned tokens_n_{};
		CXTranslationUnit translation_unit_{};

	public:
		explicit cxtokens_raii(const CXTranslationUnit translation_unit, const CXSourceRange& tokens_extent)
			: translation_unit_(translation_unit)
		{
			clang_tokenize(translation_unit, tokens_extent, &tokens_, &tokens_n_);
		}

		cxtokens_raii(const cxtokens_raii& other) = delete;
		cxtokens_raii& operator=(const cxtokens_raii& other) = delete;
		cxtokens_raii(cxtokens_raii&& other) noexcept
		{
			std::swap(*this, other);
		}
		cxtokens_raii& operator=(cxtokens_raii&& other) noexcept
		{
			if (&other == this)
			{
				return *this;
			}
			std::swap(*this, other);
			return *this;
		}
		~cxtokens_raii()
		{
			clang_disposeTokens(translation_unit_, tokens_, tokens_n_);
		}

		[[nodiscard]] unsigned tokens_n() const noexcept
		{
			return tokens_n_;
		}

		[[nodiscard]] std::string at(const token_index index) const
		{
			if (index.to_size_t() >= tokens_n_)
			{
				throw common_exceptions::invalid_index_exception(
					"cxtokens_raii.at: invalid index was passed");
			}
			return this->operator[](index);
		}

		[[nodiscard]] std::string operator[](const token_index index) const
		{
			const CXToken token = tokens_[index.to_size_t()];
			const cxstring_raii token_spelling{ clang_getTokenSpelling(translation_unit_, token) };
			return token_spelling.string();
		}
	};
}