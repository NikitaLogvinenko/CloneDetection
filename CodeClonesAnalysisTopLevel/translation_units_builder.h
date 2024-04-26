#pragma once
#include "translation_unit_raii.h"

namespace code_clones_analysis_top_level
{
	class translation_units_builder final
	{
	public:
		[[nodiscard]] std::vector<clang_c_adaptation::translation_unit_raii> build(const std::filesystem::path& directory) const;
	};
}
