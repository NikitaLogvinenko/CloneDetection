﻿#include "func_implementation_analysis_builder_default.h"

namespace cpp_code_analysis
{
	namespace
	{
		using unordered_map_count_arrays_by_var_cursors =
			std::unordered_map<
			CXCursor,
			std::array<count_matrix::count_vector_value, default_conditions_total>,
			clang_c_adaptation::cxcursor_hash,
			clang_c_adaptation::cxcursors_equal>;

		using unordered_set_cursors =
			std::unordered_set<
				CXCursor,
				clang_c_adaptation::cxcursor_hash,
				clang_c_adaptation::cxcursors_equal>;

		using unordered_map_linkage_usage_counter_pair_by_var_cursor =
			const std::unordered_map<
				CXCursor,
				var_linkage_usage_counter_pair,
				clang_c_adaptation::cxcursor_hash,
				clang_c_adaptation::cxcursors_equal>;


		class count_all_variables_visit_data
		{
			var_usage_condition incremented_condition_;
			unordered_map_count_arrays_by_var_cursors& count_arrays_by_var_cursors_;
		public:
			explicit count_all_variables_visit_data(const var_usage_condition incremented_condition, 
				unordered_map_count_arrays_by_var_cursors& count_arrays_by_var_cursors) noexcept
				: incremented_condition_(incremented_condition), count_arrays_by_var_cursors_(count_arrays_by_var_cursors) {}

			virtual ~count_all_variables_visit_data() = default;

			bool increment_condition(const CXCursor& cursor_for_incrementation)
			{
				if (count_arrays_by_var_cursors_.contains(cursor_for_incrementation))
				{
					auto& var_count_array = count_arrays_by_var_cursors_[cursor_for_incrementation];
					++var_count_array[static_cast<size_t>(incremented_condition_)];
					return true;
				}
				return false;
			}
		};

		class count_from_second_variable_visit_data final : public count_all_variables_visit_data
		{
			bool first_variable_passed_{ false };
		public:
			explicit count_from_second_variable_visit_data(const var_usage_condition incremented_condition,
				unordered_map_count_arrays_by_var_cursors& count_arrays_by_var_cursors) noexcept
				: count_all_variables_visit_data(incremented_condition, count_arrays_by_var_cursors) {}

			[[nodiscard]] bool first_variable_passed() const noexcept
			{
				return first_variable_passed_;
			}

			void set_true_first_variable_passed() noexcept
			{
				first_variable_passed_ = true;
			}
		};

		class var_definition_visit_data final
		{
			std::array<count_matrix::count_vector_value, default_conditions_total>& count_array_;
			const func_entities_cursors& first_traversal_data_;
		public:
			explicit var_definition_visit_data(
				std::array<count_matrix::count_vector_value, default_conditions_total>& count_array,
				const func_entities_cursors& first_traversal_data) noexcept
				: count_array_(count_array), first_traversal_data_(first_traversal_data) {}

			void increment_condition(const var_usage_condition incremented_condition) const noexcept
			{
				++count_array_[static_cast<size_t>(incremented_condition)];
			}

			[[nodiscard]] const func_entities_cursors& first_traversal_data() const noexcept
			{
				return first_traversal_data_;
			}
		};
	}
	

	unordered_set_cursors func_implementation_analysis_builder_default::find_func_definitions_cursors(
			const CXCursor& translation_unit_cursor)
	{
		unordered_set_cursors func_def_cursors{};
		clang_visitChildren(translation_unit_cursor, visitor_find_func_definitions, &func_def_cursors);
		return func_def_cursors;
	}

	std::vector<var_usage_info<default_conditions_total>>
		func_implementation_analysis_builder_default::traverse_func_analyse_variables_usage(
			const CXCursor& func_definition_cursor)
	{
		func_entities_cursors first_traversal_data{};
		clang_visitChildren(func_definition_cursor, visitor_find_func_entities, &first_traversal_data);
		auto count_arrays_by_var_cursors = 
			create_unordered_map_count_arrays_by_var_cursors(first_traversal_data.var_linkage_and_usage_counter_by_decl_cursors());

		for (const auto& [usage_condition, traversed_entity_types] : entities_by_condition_for_all_variables_counting)
		{
			count_all_variables_inside_entities(first_traversal_data, traversed_entity_types,
				count_arrays_by_var_cursors, usage_condition);
		}
		for (const auto& [usage_condition, traversed_entity_types] : entities_by_condition_for_first_variable_counting)
		{
			count_first_variable_inside_entities(first_traversal_data, traversed_entity_types,
				count_arrays_by_var_cursors, usage_condition);
		}
		for (const auto& [usage_condition, traversed_entity_types] : entities_by_condition_for_counting_from_second_variable)
		{
			count_from_second_variable_inside_entities(first_traversal_data, traversed_entity_types,
				count_arrays_by_var_cursors, usage_condition);
		}
		for (const auto& local_var_decl_cursor : first_traversal_data.cursors_to_entities(func_entity_type::local_var))
		{
			var_definition_visit_data visit_data{ count_arrays_by_var_cursors.at(local_var_decl_cursor), first_traversal_data };
			clang_visitChildren(local_var_decl_cursor, visitor_traverse_var_definition, &visit_data);
		}


		std::vector<var_usage_info<default_conditions_total>> variables_usage_info{};
		variables_usage_info.reserve(count_arrays_by_var_cursors.size());
		for (const auto& [used_var_cursor, linkage_usage_counter_pair] : first_traversal_data.var_linkage_and_usage_counter_by_decl_cursors())
		{
			variables_usage_info.emplace_back(clang_c_adaptation::var_spelling(used_var_cursor), clang_c_adaptation::var_location(used_var_cursor),
				linkage_usage_counter_pair.linkage(), count_arrays_by_var_cursors[used_var_cursor]);
		}

		return variables_usage_info;
	}

	unordered_map_count_arrays_by_var_cursors func_implementation_analysis_builder_default::create_unordered_map_count_arrays_by_var_cursors(
			unordered_map_linkage_usage_counter_pair_by_var_cursor& linkage_and_usage_counter_by_var)
	{
		unordered_map_count_arrays_by_var_cursors count_arrays_by_var_cursors{};
		count_arrays_by_var_cursors.reserve(linkage_and_usage_counter_by_var.size());
		for (const auto& [var_cursor, linkage_used_counter_pair] : linkage_and_usage_counter_by_var)
		{
			const auto [iterator, inserted] = count_arrays_by_var_cursors.try_emplace(
				var_cursor, std::array<count_matrix::count_vector_value, default_conditions_total>{});
			if (!inserted)
			{
				throw std::runtime_error(array_by_var_insertion_failure_msg);
			}

			auto& count_array = iterator->second;
			count_array[static_cast<size_t>(var_usage_condition::used_n_times)] = count_matrix::count_vector_value(linkage_used_counter_pair.used_n_times());
			if (var_usage_linkage_condition_by_var_linkage.contains(linkage_used_counter_pair.linkage()))
			{
				count_array[static_cast<size_t>(var_usage_linkage_condition_by_var_linkage.at(linkage_used_counter_pair.linkage()))] = var_usage_linkage_condition_value;
			}
			if (linkage_used_counter_pair.linkage() == clang_c_adaptation::var_linkage::func_param ||
				linkage_used_counter_pair.linkage() == clang_c_adaptation::var_linkage::local_var)
			{
				count_array[static_cast<size_t>(var_usage_condition::defined)] = var_defined_count_value;
			}
		}
		return count_arrays_by_var_cursors;
	}

	void func_implementation_analysis_builder_default::count_all_variables_inside_entities(
		const func_entities_cursors& first_traversal_data, 
		const std::vector<func_entity_type>& entity_types_to_traverse,
		unordered_map_count_arrays_by_var_cursors& count_arrays_by_var_cursors,
		const var_usage_condition counted_condition)
	{
		for (const auto& entity_type_to_traverse : entity_types_to_traverse)
		{
			for (const auto& entity_cursor : first_traversal_data.cursors_to_entities(entity_type_to_traverse))
			{
				count_all_variables_visit_data visit_data{ counted_condition, count_arrays_by_var_cursors };
				clang_visitChildren(entity_cursor, visitor_count_all_variables_inside_entity, &visit_data);
			}
		}
	}

	void func_implementation_analysis_builder_default::count_first_variable_inside_entities(
		const func_entities_cursors& first_traversal_data,
		const std::vector<func_entity_type>& entity_types_to_traverse,
		unordered_map_count_arrays_by_var_cursors&count_arrays_by_var_cursors,
		const var_usage_condition counted_condition)
	{
		for (const auto& entity_type_to_traverse : entity_types_to_traverse)
		{
			for (const auto& entity_cursor : first_traversal_data.cursors_to_entities(entity_type_to_traverse))
			{
				count_all_variables_visit_data visit_data{ counted_condition, count_arrays_by_var_cursors };
				clang_visitChildren(entity_cursor, visitor_count_first_variable_inside_entity, &visit_data);
			}
		}
	}

	void func_implementation_analysis_builder_default::count_from_second_variable_inside_entities(
		const func_entities_cursors& first_traversal_data,
		const std::vector<func_entity_type>& entity_types_to_traverse,
		unordered_map_count_arrays_by_var_cursors& count_arrays_by_var_cursors,
		const var_usage_condition counted_condition)
	{
		for (const auto& entity_type_to_traverse : entity_types_to_traverse)
		{
			for (const auto& entity_cursor : first_traversal_data.cursors_to_entities(entity_type_to_traverse))
			{
				count_from_second_variable_visit_data visit_data{ counted_condition, count_arrays_by_var_cursors };
				clang_visitChildren(entity_cursor, visitor_count_from_second_variable_inside_entity, &visit_data);
			}
		}
	}


	CXChildVisitResult func_implementation_analysis_builder_default::visitor_find_func_definitions(
		const CXCursor cursor_in_translation_unit, const CXCursor /*parent*/, const CXClientData func_definitions_cursors_void_ptr)
	{
		if (clang_Location_isInSystemHeader(clang_getCursorLocation(cursor_in_translation_unit)))
		{
			return CXChildVisit_Continue;
		}
		if (clang_c_adaptation::clang_c_types_handling::is_cursor_to_func_definition(cursor_in_translation_unit))
		{
			static_cast<unordered_set_cursors* const>(func_definitions_cursors_void_ptr)->insert(cursor_in_translation_unit);
		}
		else
		{
			clang_visitChildren(cursor_in_translation_unit, visitor_find_func_definitions, func_definitions_cursors_void_ptr);
		}
		return CXChildVisit_Continue;
	}

	CXChildVisitResult func_implementation_analysis_builder_default::visitor_find_func_entities(
		const CXCursor cursor_in_func_definition, const CXCursor /*parent*/, const CXClientData func_entities_cursors_void_ptr)
	{
		static_cast<func_entities_cursors* const>(func_entities_cursors_void_ptr)->try_insert(cursor_in_func_definition);
		clang_visitChildren(cursor_in_func_definition, visitor_find_func_entities, func_entities_cursors_void_ptr);
		return CXChildVisit_Continue;
	}

	CXChildVisitResult func_implementation_analysis_builder_default::visitor_count_all_variables_inside_entity(
		const CXCursor cursor_inside_entity, const CXCursor /*parent*/, const CXClientData count_all_variables_visit_data_void_ptr)
	{
		if (clang_c_adaptation::clang_c_types_handling::is_decl_ref_expr_to_var_decl(cursor_inside_entity))
		{
			static_cast<count_all_variables_visit_data* const>(
				count_all_variables_visit_data_void_ptr)->increment_condition(clang_getCursorReferenced(cursor_inside_entity));
		}
		clang_visitChildren(cursor_inside_entity, visitor_count_all_variables_inside_entity, count_all_variables_visit_data_void_ptr);
		return CXChildVisit_Continue;
	}

	CXChildVisitResult func_implementation_analysis_builder_default::visitor_count_first_variable_inside_entity(
		const CXCursor cursor_inside_entity, const CXCursor /*parent*/, const CXClientData count_all_variables_visit_data_void_ptr)
	{
		if (clang_c_adaptation::clang_c_types_handling::is_decl_ref_expr_to_var_decl(cursor_inside_entity))
				
		{
			if (static_cast<count_all_variables_visit_data* const>(
				count_all_variables_visit_data_void_ptr)->increment_condition(clang_getCursorReferenced(cursor_inside_entity)))
			{
				return CXChildVisit_Break;
			}
		}
		clang_visitChildren(cursor_inside_entity, visitor_count_first_variable_inside_entity, count_all_variables_visit_data_void_ptr);
		return CXChildVisit_Continue;
	}

	CXChildVisitResult func_implementation_analysis_builder_default::visitor_count_from_second_variable_inside_entity(
		const CXCursor cursor_inside_entity, const CXCursor /*parent*/, const CXClientData count_from_second_variable_visit_data_void_ptr)
	{
		if (clang_c_adaptation::clang_c_types_handling::is_decl_ref_expr_to_var_decl(cursor_inside_entity))
		{
			if (auto const count_from_second_variable_visit_data_ptr = 
				static_cast<count_from_second_variable_visit_data* const>(count_from_second_variable_visit_data_void_ptr);
				count_from_second_variable_visit_data_ptr->first_variable_passed())
			{
				count_from_second_variable_visit_data_ptr->increment_condition(clang_getCursorReferenced(cursor_inside_entity));
			}
			else
			{
				count_from_second_variable_visit_data_ptr->set_true_first_variable_passed();
			}
		}

		clang_visitChildren(cursor_inside_entity, visitor_count_from_second_variable_inside_entity, count_from_second_variable_visit_data_void_ptr);
		return CXChildVisit_Continue;
	}

	CXChildVisitResult func_implementation_analysis_builder_default::visitor_traverse_var_definition(
		const CXCursor cursor_inside_var_definition, const CXCursor /*parent*/, const CXClientData var_definition_visit_data_void_ptr)
	{
		auto const var_definition_visit_data_ptr = 
			static_cast<class var_definition_visit_data* const>(var_definition_visit_data_void_ptr);
		const auto& first_traversal_data = var_definition_visit_data_ptr->first_traversal_data();

		if (clang_c_adaptation::clang_c_types_handling::is_cursor_to_literal(cursor_inside_var_definition))
		{
			var_definition_visit_data_ptr->increment_condition(var_usage_condition::defined_with_literals);
			clang_visitChildren(cursor_inside_var_definition, visitor_traverse_var_definition, var_definition_visit_data_void_ptr);
			return CXChildVisit_Continue;
		}

		if (clang_getCursorKind(cursor_inside_var_definition) == CXCursor_CallExpr)
		{
			var_definition_visit_data_ptr->increment_condition(var_usage_condition::defined_with_call_expr);
		}
		if (
			first_traversal_data.cursors_to_entities(func_entity_type::plus_operator).contains(cursor_inside_var_definition) || 
			first_traversal_data.cursors_to_entities(func_entity_type::minus_operator).contains(cursor_inside_var_definition) || 
			first_traversal_data.cursors_to_entities(func_entity_type::increment_operator).contains(cursor_inside_var_definition) || 
			first_traversal_data.cursors_to_entities(func_entity_type::decrement_operator).contains(cursor_inside_var_definition))
		{
			var_definition_visit_data_ptr->increment_condition(var_usage_condition::defined_with_sum_or_difference);
		}
		else if (
			first_traversal_data.cursors_to_entities(func_entity_type::multiplication_operator).contains(cursor_inside_var_definition) ||
			first_traversal_data.cursors_to_entities(func_entity_type::division_operator).contains(cursor_inside_var_definition) ||
			first_traversal_data.cursors_to_entities(func_entity_type::unary_plus_operator).contains(cursor_inside_var_definition) ||
			first_traversal_data.cursors_to_entities(func_entity_type::unary_minus_operator).contains(cursor_inside_var_definition))
		{
			var_definition_visit_data_ptr->increment_condition(var_usage_condition::defined_with_multiplication_or_division);
		}
		else if (
			first_traversal_data.cursors_to_entities(func_entity_type::modulus_operator).contains(cursor_inside_var_definition))
		{
			var_definition_visit_data_ptr->increment_condition(var_usage_condition::defined_with_modulus);
		}
		else if (
			first_traversal_data.cursors_to_entities(func_entity_type::comparison_operator).contains(cursor_inside_var_definition))
		{
			var_definition_visit_data_ptr->increment_condition(var_usage_condition::defined_with_comparison);
		}

		clang_visitChildren(cursor_inside_var_definition, visitor_traverse_var_definition, var_definition_visit_data_void_ptr);
		return CXChildVisit_Continue;
	}
}
