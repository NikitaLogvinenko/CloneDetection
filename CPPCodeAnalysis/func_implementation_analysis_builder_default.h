#pragma once
#include "func_implementation_analysis_builder_abstract.h"
#include "cxcursor_hash.h"
#include "cxcursors_equal.h"
#include "func_entities_cursors.h"
#include "var_usage_conditions.h"
#include <unordered_set>

namespace cpp_code_analysis
{
	class func_implementation_analysis_builder_default final : public func_implementation_analysis_builder_abstract<default_conditions_total>
	{
		std::unordered_set<
			func_implementation_info<default_conditions_total>,
			func_implementation_info_hash<default_conditions_total>,
			func_implementation_info_equal_spelling_and_location<default_conditions_total>> analysed_functions_{};

	public:
		void initialize_builder() override
		{
			analysed_functions_ = {};
		}

		void analyse_translation_unit(const clang_c_adaptation::translation_unit_wrapper& translation_unit) override
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
			std::array<count_matrix::count_vector_value, default_conditions_total>,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal>
		create_unordered_map_count_arrays_by_var_cursors(
			const std::unordered_map<
				CXCursor, 
				var_linkage_usage_counter_pair, 
				clang_c_adaptation::cxcursor_hash,
				clang_c_adaptation::cxcursors_equal>& linkage_and_usage_counter_by_var);


		static void count_all_variables_inside_entities(
			const func_entities_cursors& first_traversal_data,
			const std::vector<func_entity_type>& entity_types_to_traverse,
			std::unordered_map<
				CXCursor,
				std::array<count_matrix::count_vector_value, default_conditions_total>,
				clang_c_adaptation::cxcursor_hash,
				clang_c_adaptation::cxcursors_equal>& count_arrays_by_var_cursors,
			var_usage_condition counted_condition);

		static void count_first_variable_inside_entities(
			const func_entities_cursors& first_traversal_data,
			const std::vector<func_entity_type>& entity_types_to_traverse,
			std::unordered_map<
			CXCursor,
			std::array<count_matrix::count_vector_value, default_conditions_total>,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal>& count_arrays_by_var_cursors,
			var_usage_condition counted_condition);

		static void count_from_second_variable_inside_entities(
			const func_entities_cursors& first_traversal_data,
			const std::vector<func_entity_type>& entity_types_to_traverse,
			std::unordered_map<
			CXCursor,
			std::array<count_matrix::count_vector_value, default_conditions_total>,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal>& count_arrays_by_var_cursors,
			var_usage_condition counted_condition);
		

		static CXChildVisitResult visitor_find_func_definitions(
			CXCursor cursor_in_translation_unit, CXCursor parent, CXClientData func_definitions_cursors_void_ptr);

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



		inline static const std::string array_by_var_insertion_failure_msg{ "Failure during preparing container for variables usage info." };

		inline static const std::unordered_map<clang_c_adaptation::var_linkage, var_usage_condition> var_usage_linkage_condition_by_var_linkage{
			{ clang_c_adaptation::var_linkage::func_param, var_usage_condition::is_param },
			{ clang_c_adaptation::var_linkage::local_var, var_usage_condition::is_local_var },
			{ clang_c_adaptation::var_linkage::member_field, var_usage_condition::is_member_field },
			{ clang_c_adaptation::var_linkage::static_field, var_usage_condition::is_static_filed },
			{ clang_c_adaptation::var_linkage::global_var, var_usage_condition::is_global_var }
		};

		inline static const count_matrix::count_vector_value var_usage_linkage_condition_value{1};
		inline static const count_matrix::count_vector_value var_defined_count_value{1};

		inline static const std::unordered_map<var_usage_condition, std::vector<func_entity_type>> entities_by_condition_for_all_variables_counting
		{
			{ var_usage_condition::used_inside_call_expr, std::vector{func_entity_type::any_call_expr} },

			{ var_usage_condition::used_for_sum_or_diff, std::vector{
			func_entity_type::plus_operator, func_entity_type::minus_operator,
			func_entity_type::plus_assignment_operator, func_entity_type::minus_assignment_operator,
			func_entity_type::increment_operator, func_entity_type::decrement_operator} },

			{ var_usage_condition::used_for_multiplication_or_division, std::vector{
			func_entity_type::multiplication_operator, func_entity_type::division_operator,
			func_entity_type::multiplication_assignment_operator, func_entity_type::division_assignment_operator,
			func_entity_type::unary_plus_operator, func_entity_type::unary_minus_operator} },

			{ var_usage_condition::used_for_modulus, std::vector{func_entity_type::modulus_operator, func_entity_type::modulus_assignment_operator} },

			{ var_usage_condition::used_for_comparison, std::vector{func_entity_type::comparison_operator} },

			{ var_usage_condition::used_inside_conditional_statement, std::vector{
				func_entity_type::if_stmt, func_entity_type::switch_stmt, func_entity_type::conditional_operator } },

			{ var_usage_condition::used_inside_loop, std::vector{
				func_entity_type::for_stmt, func_entity_type::for_range_stmt, func_entity_type::while_stmt } }
		};
		inline static const std::unordered_map<var_usage_condition, std::vector<func_entity_type>> entities_by_condition_for_first_variable_counting
		{
			{ var_usage_condition::changed_minimum_n_times, std::vector{
				func_entity_type::increment_operator, func_entity_type::decrement_operator,
					func_entity_type::plus_assignment_operator, func_entity_type::minus_assignment_operator,
					func_entity_type::multiplication_assignment_operator, func_entity_type::division_assignment_operator,
					func_entity_type::modulus_assignment_operator, func_entity_type::assignment_operator} },

			{ var_usage_condition::used_with_square_brackets, std::vector{
				func_entity_type::square_brackets_operator, func_entity_type::array_subscript_expr} }
		};
		inline static const std::unordered_map<var_usage_condition, std::vector<func_entity_type>> entities_by_condition_for_counting_from_second_variable
		{
			{ var_usage_condition::used_inside_square_brackets, std::vector{
				func_entity_type::square_brackets_operator, func_entity_type::array_subscript_expr} }
		};
	};
}
