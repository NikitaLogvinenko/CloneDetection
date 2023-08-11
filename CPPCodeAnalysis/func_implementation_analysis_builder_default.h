#pragma once
#include "func_implementation_analysis_builder_abstract.h"
#include "cxcursor_hash.h"
#include "cxcursors_equal.h"
#include "func_entities_classifier.h"
#include "var_usage_conditions.h"
#include <unordered_set>

namespace cpp_code_analysis
{
	class func_implementation_analysis_builder_default final : public func_implementation_analysis_builder_abstract<default_conditions_total>
	{
		std::unordered_set<
			func_implementation_info<default_conditions_total>,
			internal::func_implementation_info_hash<default_conditions_total>,
			func_implementation_info_equal_spelling_and_location<default_conditions_total>> analysed_functions_{};

	public:
		void initialize_builder() override
		{
			analysed_functions_ = {};
		}

		void analyse_translation_unit(const clang_c_adaptation::translation_unit_raii& translation_unit) override
		{
			for (const auto& func_def_cursor :
				find_func_definitions_cursors(clang_getTranslationUnitCursor(translation_unit.translation_unit())))
			{
				analysed_functions_.emplace(
					clang_c_adaptation::func_spelling(func_def_cursor),
					clang_c_adaptation::func_location(func_def_cursor),
					traverse_func_analyse_variables_usage(func_def_cursor));
			}
		}

		analysed_functions_info<default_conditions_total> build_analysed_functions_implementation_info() override
		{
			return analysed_functions_info{ std::move(analysed_functions_) };
		}

		void clear_builder() override
		{
			analysed_functions_.clear();
		}

	private:
		static std::unordered_set<
			CXCursor,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal>
		find_func_definitions_cursors(const CXCursor& translation_unit_cursor);

		static std::vector<var_usage_info<default_conditions_total>>
		traverse_func_analyse_variables_usage(const CXCursor& func_definition_cursor);

		static std::unordered_map<
			CXCursor,
			std::array<cm::count_vector_value, default_conditions_total>,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal>
		create_unordered_map_count_arrays_by_var_cursors(
			const std::unordered_map<
				CXCursor, 
				var_origin_and_usage_counter_pair, 
				clang_c_adaptation::cxcursor_hash,
				clang_c_adaptation::cxcursors_equal>& origin_and_usage_counter_by_var);


		static void count_all_variables_inside_entities(
			const func_entities_classifier& first_traversal_data,
			const std::vector<func_entity_type>& entity_types_to_traverse,
			std::unordered_map<
				CXCursor,
				std::array<cm::count_vector_value, default_conditions_total>,
				clang_c_adaptation::cxcursor_hash,
				clang_c_adaptation::cxcursors_equal>& count_arrays_by_var_cursors,
			var_usage_condition counted_condition);

		static void count_first_variable_inside_entities(
			const func_entities_classifier& first_traversal_data,
			const std::vector<func_entity_type>& entity_types_to_traverse,
			std::unordered_map<
			CXCursor,
			std::array<cm::count_vector_value, default_conditions_total>,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal>& count_arrays_by_var_cursors,
			var_usage_condition counted_condition);

		static void count_from_second_variable_inside_entities(
			const func_entities_classifier& first_traversal_data,
			const std::vector<func_entity_type>& entity_types_to_traverse,
			std::unordered_map<
			CXCursor,
			std::array<cm::count_vector_value, default_conditions_total>,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal>& count_arrays_by_var_cursors,
			var_usage_condition counted_condition);
		

		static CXChildVisitResult visitor_find_func_definitions(
			CXCursor cursor_inside_translation_unit, CXCursor parent, CXClientData func_definitions_cursors_void_ptr);

		static CXChildVisitResult visitor_find_func_entities(
			CXCursor cursor_in_func_definition, CXCursor parent, CXClientData func_entities_cursors_void_ptr);


		static CXChildVisitResult visitor_count_all_variables_inside_entity(
			CXCursor cursor_inside_entity, CXCursor parent, CXClientData count_all_variables_visit_data_void_ptr);

		static CXChildVisitResult visitor_count_first_variable_inside_entity(
			CXCursor cursor_inside_entity, CXCursor parent, CXClientData count_all_variables_visit_data_void_ptr);

		static CXChildVisitResult visitor_count_from_second_variable_inside_entity(
			CXCursor cursor_inside_entity, CXCursor parent, CXClientData count_from_second_variable_visit_data_void_ptr);


		static CXChildVisitResult visitor_traverse_var_definition(
			CXCursor cursor_inside_var_definition, CXCursor parent, CXClientData var_definition_visit_data_void_ptr);
	};
}
