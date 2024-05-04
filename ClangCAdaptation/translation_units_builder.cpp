#include "translation_units_builder.h"
#include "input_format_error.h"
#include "parameters_validation.h"
#include "cxindex_raii.h"
#include <unordered_set>

std::vector<clang_c_adaptation::translation_unit_raii> clang_c_adaptation::translation_units_builder::build(
	const std::unordered_set<std::filesystem::path>& directories) const
{
	static cxindex_raii cxindex{};

	std::vector<translation_unit_raii> result;

	for (const auto& dir : directories)
	{
		utility::throw_if_nonexistent_directory<common_exceptions::input_format_error>(
			dir, "translation_units_builder::build");

		for (const auto& entry : std::filesystem::directory_iterator{ dir })
		{
			if (entry.path().extension() != ".ast")
			{
				continue;
			}

			result.emplace_back(cxindex, entry.path());
		}
	}

	return result;
}
