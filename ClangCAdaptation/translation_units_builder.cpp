#include "translation_units_builder.h"
#include "input_format_error.h"
#include "parameters_validation.h"
#include "cxindex_raii.h"

std::vector<clang_c_adaptation::translation_unit_raii> clang_c_adaptation::translation_units_builder::build(
	const std::filesystem::path& directory) const
{
	static cxindex_raii cxindex{};

	utility::throw_if_nonexistent_directory<common_exceptions::input_format_error>(
		directory, "translation_units_builder::build");

	std::vector<translation_unit_raii> result;

	for (const auto& entry : std::filesystem::directory_iterator{directory})
	{
		if (entry.path().extension() != ".ast")
		{
			continue;
		}

		result.emplace_back(cxindex, entry.path());
	}

	return result;
}
