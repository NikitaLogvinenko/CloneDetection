#include "translation_units_builder.h"
#include "input_format_error.h"
#include "parameters_validation.h"
#include "cxindex_raii.h"

std::vector<clang_c_adaptation::translation_unit_raii> code_clones_analysis_top_level::translation_units_builder::build(
	const std::filesystem::path& directory) const
{
	static clang_c_adaptation::cxindex_raii cxindex{};

	utility::throw_if_nonexistent_directory<common_exceptions::input_format_error>(
		directory, "funcs_analysis_through_count_matrix_factory_default::create_traversers_factory");

	std::vector<clang_c_adaptation::translation_unit_raii> result;

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
