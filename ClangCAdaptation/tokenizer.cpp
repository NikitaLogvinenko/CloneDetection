#include "tokenizer.h"
#include "cxstring_raii.h"
#include "copying_moving_delete.h"

namespace clang_c_adaptation::internal
{
	namespace
	{
		class cxtokens_raii final
		{
			CXToken* tokens_{};
			unsigned tokens_count_{};
			const CXTranslationUnit* translation_unit_ptr_{};

		public:
			cxtokens_raii(const CXTranslationUnit& translation_unit, const CXSourceRange& tokens_extent)
				: translation_unit_ptr_(&translation_unit)
			{
				clang_tokenize(translation_unit, tokens_extent, &tokens_, &tokens_count_);
			}

			COPYING_MOVING_DELETE(cxtokens_raii)

			~cxtokens_raii()
			{
				clang_disposeTokens(*translation_unit_ptr_, tokens_, tokens_count_);
			}

			[[nodiscard]] const CXToken* tokens() const noexcept
			{
				return tokens_;
			}

			[[nodiscard]] unsigned tokens_count() const noexcept
			{
				return tokens_count_;
			}
		};
	}

	tokenizer::tokenizer(const CXTranslationUnit& translation_unit, const CXSourceRange& tokens_extent)
	{
		const cxtokens_raii tokens_tmp(translation_unit, tokens_extent);
		if (tokens_tmp.tokens_count() == 0)
		{
			return;
		}

		const CXToken* tokens_array = tokens_tmp.tokens();
		if (tokens_array == nullptr)
		{
			throw common_exceptions::nullptr_error("tokenizer::tokenizer: nullptr tokens.");
		}
		tokens_.reserve(tokens_tmp.tokens_count());

		for (unsigned token_index = 0; token_index < tokens_tmp.tokens_count(); ++token_index)
		{
			const CXToken* token_ptr = tokens_array + token_index;
			const cxstring_raii token_spelling{ clang_getTokenSpelling(translation_unit, *token_ptr) };
			tokens_.emplace_back(token_spelling.string());
		}
	}
}
