#pragma once
#include "condition_analyzer_abstract.h"
#include "funcs_analysis_traits.h"
#include "cursors_storage_threadsafe.h"
#include "cursor_classifier.h"

namespace clang_code_analysis
{
	template <size_t ConditionsCount, class Predicate>
	requires (cm::count_vector_length<ConditionsCount> && std::predicate<Predicate, CXCursor>)
	class vars_nested_in_condition_finder_abstract final
		: public condition_analyzer_abstract<code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>>
	{
		using funcs_analysis_traits = code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>;
		using var_id = code_analysis::var_id;

	public:
		using condition_index = code_analysis_through_cm::condition_index<ConditionsCount>;
		using var_usage_callback = code_analysis_through_cm::nested_entity_condition_callback<funcs_analysis_traits>;
		using func_id = code_analysis::func_id;

	private:

		struct finding_all_nested_variables_data final
		{
			const func_id func;
			const std::reference_wrapper<const var_usage_callback> callback;
			std::reference_wrapper<const std::vector<condition_index>> indices;
		};

		std::vector<condition_index> indices_{};
		Predicate predicate_{};

	public:
		vars_nested_in_condition_finder_abstract() noexcept(std::is_nothrow_default_constructible_v<Predicate>) = default;

		explicit vars_nested_in_condition_finder_abstract(std::vector<condition_index> indices, Predicate predicate)
		noexcept(std::is_nothrow_move_constructible_v<Predicate>) : indices_(std::move(indices)), predicate_(std::move(predicate)) {}

		void analyse(const func_id analyzed_id, const CXCursor& nested_cursor, const var_usage_callback& callback) override
		{
			if (!predicate_(nested_cursor))
			{
				return;
			}

			const finding_all_nested_variables_data data{ analyzed_id, std::ref(callback), std::ref(indices_) };
			clang_visitChildren(nested_cursor, visitor_finding_all_nested_variables, &data);
		}

	private:
		// ReSharper disable once CppPassValueParameterByConstReference
		static CXChildVisitResult visitor_finding_all_nested_variables(
			CXCursor cursor, CXCursor /*parent*/, const CXClientData data_void_ptr)
		{
			const auto data_ptr = static_cast<const finding_all_nested_variables_data*>(data_void_ptr);
			if (cursor_classifier::is_reference_to_var_declaration(cursor))
			{
				const auto referenced_var = clang_getCursorReferenced(cursor);
				const var_id var = cursors_storage_threadsafe<var_id>::get_instance().insert(referenced_var);
				for (const auto index : data_ptr->indices)
				{
					data_ptr->callback(data_ptr->func, var, index);
				}
			}

			clang_visitChildren(cursor, visitor_finding_all_nested_variables, data_void_ptr);
			return CXChildVisit_Continue;
		}
	};
}