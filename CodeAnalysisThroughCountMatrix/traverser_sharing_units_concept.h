#pragma once
#include <concepts>
#include "code_analysis_traits.h"
#include "code_traverser_sharing_units_abstract.h"

namespace code_analysis_through_cm
{
	template <class TraverserT, class AnalysisTraits, class TranslationUnitT>
	concept traverser_sharing_units_concept = code_analysis_traits<AnalysisTraits>
	&& std::derived_from<TraverserT, code_traverser_sharing_units_abstract<AnalysisTraits, TranslationUnitT>>
	&& std::is_constructible_v<TraverserT, std::shared_ptr<utility::one_off_container<TranslationUnitT>>, std::shared_ptr<utility::spinlock>>;
}
