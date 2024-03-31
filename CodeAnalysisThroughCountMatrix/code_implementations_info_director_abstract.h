#pragma once
#include "code_implementation_info_builder_abstract.h"
#include "code_traversers_factory_abstract.h"

namespace code_analysis_through_cm
{
	template <code_analysis_traits AnalysisTraits>
	class code_implementations_info_director_abstract
	{
	protected:
		constexpr code_implementations_info_director_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(code_implementations_info_director_abstract)

		[[nodiscard]] virtual std::vector<code_entity_implementation_info<AnalysisTraits>> analyse_implementations(
			std::unique_ptr<code_implementation_info_builder_abstract<AnalysisTraits>> builder,
			std::unique_ptr<code_traversers_factory_abstract<AnalysisTraits>> traversers_factory) const = 0;
	};
}
