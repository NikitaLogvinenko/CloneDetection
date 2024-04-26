#pragma once
#include "translation_unit_raii.h"

namespace clang_c_adaptation
{
	class translation_units_builder final
	{
	public:
		[[nodiscard]] std::vector<translation_unit_raii> build(const std::filesystem::path& directory) const;
	};
}
