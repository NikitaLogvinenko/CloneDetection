#pragma once
#include "condition_analyzer_abstract.h"
#include <memory>

namespace clang_code_analysis
{
	template <code_analysis_through_cm::code_analysis_traits AnalysisTraits>
	class conditions_analyzers_factory_abstract
	{
	public:
		using conditions_analyzers_vector = std::vector<std::unique_ptr<condition_analyzer_abstract<AnalysisTraits>>>;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(conditions_analyzers_factory_abstract)

		[[nodiscard]] virtual conditions_analyzers_vector create_analyzers() = 0;

	protected:
		conditions_analyzers_factory_abstract() noexcept = default;
	};
}