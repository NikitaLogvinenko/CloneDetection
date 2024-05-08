#pragma once
#include "translation_unit_raii.h"
#include <unordered_set>

namespace clang_c_adaptation
{
	class translation_units_builder final
	{
	public:
		[[nodiscard]] std::vector<translation_unit_raii> build(const std::unordered_set<std::filesystem::path>& directories) const;
	};
}
