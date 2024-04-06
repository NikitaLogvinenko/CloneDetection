#pragma once
#include "nested_entity_condition_callback.h"
#include "code_analysis_traits.h"
#include "clang-c/Index.h"
#include "constexpr_default_copy_move_constructor_assignment_virtual_destructor_move_noexcept.h"

namespace clang_code_analysis
{
	template <code_analysis_through_cm::code_analysis_traits AnalysisTraits>
	class condition_analyzer_abstract
	{
		using condition_callback = code_analysis_through_cm::nested_entity_condition_callback<AnalysisTraits>;
		using analyzed_entity_id = typename AnalysisTraits::analyzed_entity_id;

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(condition_analyzer_abstract)

		virtual void analyse(analyzed_entity_id analyzed_id, const CXCursor& nested_cursor, const condition_callback& callback) = 0;

	protected:
		condition_analyzer_abstract() noexcept = default;
	};
}
