#pragma once
#include "operator_kind.h"
#include "clang-c/Index.h"

namespace clang_code_analysis
{
	class operator_classifier final
	{
	public:
		operator_classifier() = delete;

		[[nodiscard]] static operator_kind classify(const CXCursor& cursor);
	};
}