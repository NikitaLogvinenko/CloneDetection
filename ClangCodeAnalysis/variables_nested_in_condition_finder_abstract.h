#pragma once
#include "condition_analyzer_abstract.h"
#include "funcs_analysis_traits.h"
#include "cursors_storage_threadsafe.h"
#include "common_checks.h"

namespace clang_code_analysis
{
	template <size_t ConditionsCount> requires cm::count_vector_length<ConditionsCount>
	class variables_nested_in_condition_finder_abstract
		: public condition_analyzer_abstract<code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>>
	{
		using funcs_analysis_traits = code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>;
		using condition_index = code_analysis_through_cm::condition_index<ConditionsCount>;
		using var_id = code_analysis::var_id;

	public:
		using var_usage_callback = code_analysis_through_cm::nested_entity_condition_callback<funcs_analysis_traits>;
		using func_id = code_analysis::func_id;

	private:

		struct finding_all_nested_variables_data final
		{
			const func_id func;
			const std::reference_wrapper<const var_usage_callback> callback;
			const condition_index index;
		};

		condition_index index_{};

	public:
		void analyse(const func_id analyzed_id, const CXCursor& nested_cursor, const var_usage_callback& callback) const override
		{
			if (!cursor_satisfy_condition(nested_cursor))
			{
				return;
			}

			const finding_all_nested_variables_data data{ analyzed_id, std::ref(callback), index_ };
			clang_visitChildren(nested_cursor, visitor_finding_all_nested_variables, &data);
		}

	protected:
		variables_nested_in_condition_finder_abstract() noexcept = default;

		explicit variables_nested_in_condition_finder_abstract(const condition_index index) noexcept : index_(index) {}

		[[nodiscard]] virtual bool cursor_satisfy_condition(const CXCursor& parent) const = 0;

	private:
		static CXChildVisitResult visitor_finding_all_nested_variables(
			CXCursor cursor, CXCursor /*parent*/, const CXClientData data_void_ptr)
		{
			const auto data_ptr = static_cast<const finding_all_nested_variables_data*>(data_void_ptr);
			if (clang_c_adaptation::common_checks::is_cursor_referring_to_var_decl(cursor))
			{
				const auto referenced_var = clang_getCursorReferenced(cursor);
				const var_id var = cursors_storage_threadsafe<var_id>::get_instance().insert(referenced_var);
				data_ptr->callback(data_ptr->func, var, data_ptr->index);
			}

			clang_visitChildren(cursor, visitor_finding_all_nested_variables, data_void_ptr);
			return CXChildVisit_Continue;
		}
	};
}