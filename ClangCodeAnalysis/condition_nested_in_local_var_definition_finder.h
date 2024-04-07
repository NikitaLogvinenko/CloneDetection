#pragma once
#include "condition_analyzer_abstract.h"
#include "funcs_analysis_traits.h"
#include "cursors_storage_threadsafe.h"
#include "cursor_classifier.h"

namespace clang_code_analysis
{
	template <size_t ConditionsCount, class Predicate>
		requires (cm::count_vector_length<ConditionsCount> && std::predicate<Predicate, CXCursor>)
	class condition_nested_in_local_var_definition_finder final
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

		struct finding_nested_condition_data final
		{
			const func_id func;
			const var_id var;
			const std::reference_wrapper<Predicate> predicate;
			const std::reference_wrapper<const var_usage_callback> callback;
			const std::reference_wrapper<const base> this_as_base;
		};

		Predicate predicate_{};

	public:
		condition_nested_in_local_var_definition_finder() noexcept(std::is_nothrow_default_constructible_v<Predicate>) = default;

		explicit condition_nested_in_local_var_definition_finder(std::vector<condition_index> indices, Predicate predicate)
			noexcept(std::is_nothrow_move_constructible_v<Predicate>) : base(std::move(indices)), predicate_(std::move(predicate)) {}

		void analyse(const func_id analyzed_id, const CXCursor& nested_cursor, const var_usage_callback& callback) override
		{
			if (!cursor_classifier::is_local_var_declaration(nested_cursor))
			{
				return;
			}

			const var_id local_var_id = cursors_storage_threadsafe<var_id>::get_instance().insert(nested_cursor);

			const finding_nested_condition_data data{ analyzed_id, local_var_id, std::ref(predicate_), std::ref(callback), std::ref(*this) };
			clang_visitChildren(nested_cursor, visitor_finding_nested_condition, &data);
		}

	private:
		// ReSharper disable once CppPassValueParameterByConstReference
		static CXChildVisitResult visitor_finding_nested_condition(
			CXCursor cursor, CXCursor /*parent*/, const CXClientData data_void_ptr)
		{
			if (const auto data_ptr = static_cast<const finding_nested_condition_data*>(data_void_ptr); 
				data_ptr->predicate(cursor))
			{
				data_ptr->this_as_base.invoke_callback_for_all_indices(data_ptr->func, data_ptr->var, data_ptr->callback);
			}

			clang_visitChildren(cursor, visitor_finding_nested_condition, data_void_ptr);
			return CXChildVisit_Continue;
		}
	};
}