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
	protected:
		using condition_callback = code_analysis_through_cm::nested_entity_condition_callback<AnalysisTraits>;
		using analyzed_entity_id = typename AnalysisTraits::analyzed_entity_id;
		using nested_entity_id = typename AnalysisTraits::nested_entity_id;
		using condition_index = code_analysis_through_cm::condition_index<AnalysisTraits::conditions_count>;

	private:
		std::vector<condition_index> indices_{};

	protected:
		constexpr condition_analyzer_abstract() noexcept = default;

		constexpr explicit condition_analyzer_abstract(std::vector<condition_index> indices) noexcept : indices_(std::move(indices)) {}

		void invoke_callback_for_all_indices(const analyzed_entity_id analyzed_id,
			const nested_entity_id nested_id, const condition_callback& callback) const
		{
			for (const auto index : indices_)
			{
				callback(analyzed_id, nested_id, index);
			}
		}

	public:
		CONSTEXPR_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_VIRTUAL_DESTRUCTOR_NOEXCEPT_MOVE(condition_analyzer_abstract)

		virtual void analyse(analyzed_entity_id analyzed_id, const CXCursor& nested_cursor, const condition_callback& callback) = 0;
	};
}
