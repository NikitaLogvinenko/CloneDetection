#pragma once
#include "condition_analyzer_abstract.h"
#include "funcs_analysis_traits.h"
#include "cursors_storage_threadsafe.h"
#include "cursor_classifier.h"
#include "id_hash.h"

namespace clang_code_analysis
{
	template <size_t ConditionsCount> requires cm::count_vector_length<ConditionsCount>
	class global_var_analyzer final
		: public condition_analyzer_abstract<code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>>
	{
		using funcs_analysis_traits = code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>;
		using base = condition_analyzer_abstract<funcs_analysis_traits>;
		using condition_index = code_analysis_through_cm::condition_index<ConditionsCount>;
		using var_id = code_analysis::var_id;
		using ids_set = std::unordered_set<var_id, utility::id_hash<var_id>>;

	public:
		using var_usage_callback = code_analysis_through_cm::nested_entity_condition_callback<funcs_analysis_traits>;
		using func_id = code_analysis::func_id;

	private:
		ids_set vars_processed_already_{};

	public:
		global_var_analyzer() noexcept = default;

		explicit global_var_analyzer(std::vector<condition_index> indices) noexcept : base(std::move(indices)) {}

		void analyse(const func_id analyzed_id, const CXCursor& nested_cursor, const var_usage_callback& callback) override
		{
			if (!cursor_classifier::is_reference_to_var_declaration(nested_cursor))
			{
				return;
			}

			const auto declaration_cursor = clang_getCursorReferenced(nested_cursor);

			if (cursor_classifier::is_local_var_declaration(declaration_cursor) ||
				cursor_classifier::is_param_declaration(declaration_cursor) ||
				cursor_classifier::is_reference_to_field_declaration(declaration_cursor))
			{
				return;
			}

			const auto global_var_id = cursors_storage_threadsafe<var_id>::get_instance().insert(declaration_cursor);
			if (vars_processed_already_.contains(global_var_id))
			{
				return;
			}

			base::invoke_callback_for_all_indices(analyzed_id, global_var_id, callback);
		}
	};
}