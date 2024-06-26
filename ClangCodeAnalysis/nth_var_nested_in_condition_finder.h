﻿#pragma once
#include "condition_analyzer_abstract.h"
#include "funcs_analysis_traits.h"
#include "cursors_storage_threadsafe.h"
#include "cursor_classifier.h"

namespace clang_code_analysis
{
	template <size_t ConditionsCount, size_t N, class Predicate>
		requires (cm::count_vector_length<ConditionsCount> && std::predicate<Predicate, const CXCursor&> && N > 0)
	class nth_var_nested_in_condition_finder final
		: public condition_analyzer_abstract<code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>>
	{
		using funcs_analysis_traits = code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>;
		using base = condition_analyzer_abstract<funcs_analysis_traits>;
		using var_id = code_analysis::var_id;

	public:
		using condition_index = code_analysis_through_cm::condition_index<ConditionsCount>;
		using var_usage_callback = code_analysis_through_cm::nested_entity_condition_callback<funcs_analysis_traits>;
		using func_id = code_analysis::func_id;

	private:

		struct finding_nth_nested_variable_data final
		{
			const func_id func;
			const var_usage_callback& callback;
			const nth_var_nested_in_condition_finder& this_ref;

			size_t variables_passed{};
		};

		Predicate predicate_{};

	public:
		nth_var_nested_in_condition_finder() noexcept(std::is_nothrow_default_constructible_v<Predicate>) = default;

		explicit nth_var_nested_in_condition_finder(std::vector<condition_index> indices, Predicate predicate)
			noexcept(std::is_nothrow_move_constructible_v<Predicate>) : base(std::move(indices)), predicate_(std::move(predicate)) {}

		void analyse(const func_id analyzed_id, const CXCursor& nested_cursor, const var_usage_callback& callback) override
		{
			if (!predicate_(nested_cursor))
			{
				return;
			}

			finding_nth_nested_variable_data data{ analyzed_id, callback, *this };
			clang_visitChildren(nested_cursor, visitor_finding_nth_nested_variable, &data);
		}

	private:
		// ReSharper disable once CppPassValueParameterByConstReference
		static CXChildVisitResult visitor_finding_nth_nested_variable(
			const CXCursor cursor, CXCursor /*parent*/, const CXClientData data_void_ptr)
		{
			if (const auto data_ptr = static_cast<finding_nth_nested_variable_data*>(data_void_ptr); 
				cursor_classifier::is_reference_to_var_declaration(cursor))
			{
				++data_ptr->variables_passed;
				if (data_ptr->variables_passed == N)
				{
					const auto referenced_var = clang_getCursorReferenced(cursor);
					const var_id referenced_var_id = cursors_storage_threadsafe<var_id>::get_instance().insert(referenced_var);
					data_ptr->this_ref.invoke_callback_for_all_indices(data_ptr->func, referenced_var_id, data_ptr->callback);
					return CXChildVisit_Break;
				}
			}

			clang_visitChildren(cursor, visitor_finding_nth_nested_variable, data_void_ptr);
			return CXChildVisit_Continue;
		}
	};
}