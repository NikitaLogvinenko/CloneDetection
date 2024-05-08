#pragma once
#include "code_traverser_abstract.h"
#include <memory>

namespace code_analysis_through_cm
{
	template <code_analysis_traits AnalysisTraits>
	class code_traversers_factory_abstract
	{
	protected:
		constexpr code_traversers_factory_abstract() noexcept = default;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(code_traversers_factory_abstract)

		[[nodiscard]] virtual std::unique_ptr<code_traverser_abstract<AnalysisTraits>> generate() = 0;
	};
}