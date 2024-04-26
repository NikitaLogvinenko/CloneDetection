#pragma once
#include "cmcd_config.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"
#include "code_implementations_info_director_abstract.h"
#include "code_implementation_info_builder_abstract.h"
#include "code_traversers_factory_abstract.h"
#include "funcs_analysis_traits.h"
#include <memory>

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class funcs_analysis_through_count_matrix_factory_abstract
	{
	public:
		using code_implementations_info_director_abstract = code_analysis_through_cm::code_implementations_info_director_abstract<
			code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>>;

		using code_implementation_info_builder_abstract = code_analysis_through_cm::code_implementation_info_builder_abstract<
			code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>>;

		using code_traversers_factory_abstract = code_analysis_through_cm::code_traversers_factory_abstract<
			code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>>;

	protected:
		funcs_analysis_through_count_matrix_factory_abstract() noexcept = default;

		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(funcs_analysis_through_count_matrix_factory_abstract)

	public:
		[[nodiscard]] virtual std::unique_ptr<code_implementations_info_director_abstract> create_director(
		const cmcd_config& config) const = 0;

		[[nodiscard]] virtual std::unique_ptr<code_implementation_info_builder_abstract> create_builder(
			const cmcd_config& config) const = 0;

		[[nodiscard]] virtual std::unique_ptr<code_traversers_factory_abstract> create_traversers_factory(
			const cmcd_config& config, const std::filesystem::path& project_directory) const = 0;
	};
}