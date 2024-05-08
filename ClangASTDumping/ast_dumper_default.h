#pragma once
#include "ast_dumper_abstract.h"
#include "cxindex_raii.h"

namespace clang_ast_dumping
{
	class ast_dumper_default final : public ast_dumper_abstract
	{
		clang_c_adaptation::cxindex_raii cxindex_{};

	public:
		void dump(const source_to_ast_arguments& args) override;
	};
}
