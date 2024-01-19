#pragma once
#include "clang-c/Index.h"
#include "token_index.h"
#include <string>
#include <vector>

namespace clang_c_adaptation::internal
{
	class tokenizer final
	{
		std::vector<std::string> tokens_{};

	public:
		explicit tokenizer(const CXTranslationUnit& translation_unit, const CXSourceRange& tokens_extent);

		[[nodiscard]] size_t tokens_count() const noexcept
		{
			return tokens_.size();
		}

		[[nodiscard]] std::string at(const token_index index) const
		{
			return tokens_.at(index.to_size_t());
		}
	};
}
