#pragma once
#include "translation_unit_raii.h"
#include "code_traverser_sharing_units_container_abstract.h"
#include "funcs_analysis_traits.h"
#include "funcs_definitions_finder.h"
#include "cursors_storage_threadsafe.h"
#include "condition_analyzer_abstract.h"
#include "common_checks.h"

namespace clang_code_analysis
{
	template <size_t ConditionsCount> requires cm::count_vector_length<ConditionsCount>
	class clang_funcs_traverser final :
		public code_analysis_through_cm::code_traverser_sharing_units_container_abstract<
		code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>, clang_c_adaptation::translation_unit_raii>
	{
	public:
		using translation_unit_raii = clang_c_adaptation::translation_unit_raii;
		using translation_units_container = utility::one_off_container<translation_unit_raii>;
		using funcs_analysis_traits = code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>;
		using conditions_analyzers_vector = std::vector<std::unique_ptr<condition_analyzer_abstract<funcs_analysis_traits>>>;

	private:
		using var_usage_callback = code_analysis_through_cm::nested_entity_condition_callback<funcs_analysis_traits>;
		using base = code_analysis_through_cm::code_traverser_sharing_units_container_abstract<funcs_analysis_traits, clang_c_adaptation::translation_unit_raii>;
		using func_id = code_analysis::func_id;
		using var_id = code_analysis::var_id;

		struct traversing_all_cursors_inside_function_data final
		{
			const func_id func;
			const std::reference_wrapper<const conditions_analyzers_vector> analyzers;
			const std::reference_wrapper<const var_usage_callback> callback;
		};

		conditions_analyzers_vector analyzers_{};

	public:
		clang_funcs_traverser(std::shared_ptr<translation_units_container> not_traversed_units,
			std::shared_ptr<utility::spinlock> units_container_lock, conditions_analyzers_vector analyzers)
			: base(std::move(not_traversed_units), std::move(units_container_lock)), analyzers_(std::move(analyzers)) {}

	private:
		void traverse_unit(const std::unique_ptr<translation_unit_raii> translation_unit, const var_usage_callback& callback) const override
		{
			const auto& clang_translation_unit = translation_unit->translation_unit();
			const auto translation_unit_cursor = clang_getTranslationUnitCursor(clang_translation_unit);

			for (const auto funcs_ids = funcs_definitions_finder::find_nested(translation_unit_cursor); 
				const auto func_id : funcs_ids)
			{
				const auto& func_cursor = cursors_storage_threadsafe<code_analysis::func_id>::get_instance().at(func_id);

				const traversing_all_cursors_inside_function_data data{ func_id, std::ref(analyzers_), std::ref(callback) };
				clang_visitChildren(func_cursor, visitor_traversing_all_cursors_inside_function, &data);
			}
		}

		static CXChildVisitResult visitor_traversing_all_cursors_inside_function(
			CXCursor cursor, CXCursor /*parent*/, const CXClientData data_void_ptr)
		{
			const auto data_ptr = static_cast<const traversing_all_cursors_inside_function_data*>(data_void_ptr);
			save_if_var_declaration_or_reference_to_declaration(cursor);
			for (const auto& analyzer : data_ptr->analyzers)
			{
				analyzer->analyse(data_ptr->func, cursor, data_ptr->callback);
			}

			clang_visitChildren(cursor, visitor_traversing_all_cursors_inside_function, data_void_ptr);
			return CXChildVisit_Continue;
		}

		static bool save_if_var_declaration_or_reference_to_declaration(const CXCursor& cursor)
		{
			if (save_if_var_declaration(cursor))
			{
				return true;
			}

			return save_if_reference_to_var_declaration(cursor);
		}

		static bool save_if_var_declaration(const CXCursor& cursor)
		{
			if (!clang_c_adaptation::common_checks::is_cursor_to_var_decl(cursor))
			{
				return false;
			}

			save_var_declaration(cursor);
			return true;
		}

		static bool save_if_reference_to_var_declaration(const CXCursor& cursor)
		{
			if (!clang_c_adaptation::common_checks::is_cursor_referring_to_var_decl(cursor))
			{
				return false;
			}

			const CXCursor referenced_cursor = clang_getCursorReferenced(cursor);
			save_var_declaration(referenced_cursor);
			return true;
		}

		static void save_var_declaration(const CXCursor& cursor)
		{
			cursors_storage_threadsafe<var_id>::get_instance().insert(cursor);
		}
	};
}
