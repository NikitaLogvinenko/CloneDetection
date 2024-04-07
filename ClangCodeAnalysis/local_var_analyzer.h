#pragma once
#include "condition_analyzer_abstract.h"
#include "funcs_analysis_traits.h"
#include "cursors_storage_threadsafe.h"
#include "cursor_classifier.h"

namespace clang_code_analysis
{
	template <size_t ConditionsCount> requires cm::count_vector_length<ConditionsCount>
	class local_var_analyzer final
		: public condition_analyzer_abstract<code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>>
	{
		using funcs_analysis_traits = code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>;
		using base = condition_analyzer_abstract<funcs_analysis_traits>;
		using condition_index = code_analysis_through_cm::condition_index<ConditionsCount>;
		using var_id = code_analysis::var_id;

	public:
		using var_usage_callback = code_analysis_through_cm::nested_entity_condition_callback<funcs_analysis_traits>;
		using func_id = code_analysis::func_id;

		local_var_analyzer() noexcept = default;

		explicit local_var_analyzer(std::vector<condition_index> indices) noexcept : base(std::move(indices)) {}

		void analyse(const func_id analyzed_id, const CXCursor& nested_cursor, const var_usage_callback& callback) override
		{
			if (!cursor_classifier::is_local_var_declaration(nested_cursor))
			{
				return;
			}

			const auto local_var_id = cursors_storage_threadsafe<var_id>::get_instance().insert(nested_cursor);
			base::invoke_callback_for_all_indices(analyzed_id, local_var_id, callback);
		}
	};
}