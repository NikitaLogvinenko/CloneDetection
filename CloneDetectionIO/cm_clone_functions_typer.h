#pragma once
#include "functions_cm_pairwise_similarity.h"
#include "func_implementation_info_equal_spelling_and_location.h"
#include <ostream>

namespace clone_detection_io
{
	class cm_clone_functions_typer final
	{
		inline static const std::string header{ "\n~~~ Count matrix based clone-functions detection ~~~\nfunctions_similarity_threshold=" };

		inline static const std::string clones_str{ "clones: " };
		inline static const std::string functions_similarity_str{ "functions similarity: " };

		inline static const std::string first_clone_function_str{ "first clone function: " };
		inline static const std::string second_clone_function_str{ "second clone function: " };
		inline static const std::string function_spelling_str{ "function spelling: " };
		inline static const std::string function_location_str{ "function location: " };
		inline static const std::string file_str{ "file: " };
		inline static const std::string line_str{ " line: " };
		inline static const std::string column_str{ " column: " };

		inline static const std::string matched_variables{ "matched variables: " };

		inline static const std::string matched_variable_from_first_function_str{ " matched variable from first function: " };
		inline static const std::string matched_variable_from_second_function_str{ "matched variable from second function: " };
		inline static const std::string variable_spelling_str{ "variable spelling: " };
		inline static const std::string variable_location_str{ "variable location: " };
		inline static const std::string variables_similarity_str{ "variables similarity: " };

		inline static const std::string tab{ "\t" };
		inline static const std::string right_arrow{ ">" };
		inline static const std::string newline{ "\n" };
		inline static const std::string percentage{ "%" };

		inline static constexpr size_t one_hundred_percent = 100;

	public:
		cm_clone_functions_typer() = delete;

		template <size_t VarUsageConditionsN>
		static void type(
			std::ostream& output, 
			const cpp_code_analysis::functions_cm_pairwise_similarity<VarUsageConditionsN>& pairwise_similarity,
			count_matrix::relative_similarity similarity_threshold);

	private:
		template <size_t VarUsageConditionsN>
		static void type_functions_pair(
			std::ostream& output,
			const cpp_code_analysis::func_implementation_info<VarUsageConditionsN>& first_func_info,
			const cpp_code_analysis::func_implementation_info<VarUsageConditionsN>& second_func_info,
			const count_matrix::count_matrices_similarity_data& cm_similarity_data);

		static std::string convert_similarity(count_matrix::relative_similarity relative_similarity);

		template <size_t VarUsageConditionsN>
		static void type_func_info(
			std::ostream& output,
			const cpp_code_analysis::func_implementation_info<VarUsageConditionsN>& func_info);

		template <size_t VarUsageConditionsN>
		static void type_var_info(
			std::ostream& output,
			const cpp_code_analysis::var_usage_info<VarUsageConditionsN>& var_usage_info);
	};

	inline std::string cm_clone_functions_typer::convert_similarity(
		const count_matrix::relative_similarity relative_similarity)
	{
		return std::to_string(std::lround(relative_similarity.to_double() * one_hundred_percent)) + percentage;
	}

	template <size_t VarUsageConditionsN>
	void cm_clone_functions_typer::type(std::ostream& output,
		const cpp_code_analysis::functions_cm_pairwise_similarity<VarUsageConditionsN>& pairwise_similarity,
		count_matrix::relative_similarity similarity_threshold)
	{
		output << header << convert_similarity(similarity_threshold) << newline;
		for (auto iterator = pairwise_similarity.lower_bound_similarity_func_indices_pair(similarity_threshold);
			iterator != pairwise_similarity.end(); ++iterator)
		{
			const auto& [funcs_similarity, funcs_indices] = *iterator;
			const count_matrix::count_matrices_similarity_data& cm_similarity_data = pairwise_similarity.get_functions_similarity_data(funcs_indices);
			const cpp_code_analysis::func_implementation_info<VarUsageConditionsN>& first_func_info =
				pairwise_similarity.get_analysed_functions_info()[funcs_indices.first_func_index()];
			const cpp_code_analysis::func_implementation_info<VarUsageConditionsN>& second_func_info =
				pairwise_similarity.get_analysed_functions_info()[funcs_indices.second_func_index()];
			if (!cpp_code_analysis::func_implementation_info_equal_spelling_and_location<VarUsageConditionsN>{}(first_func_info, second_func_info) &&
				first_func_info.variables_count() != 0 && second_func_info.variables_count() != 0)
			{
				type_functions_pair<VarUsageConditionsN>(output, first_func_info, second_func_info, cm_similarity_data);
			}
		}
	}

	template <size_t VarUsageConditionsN>
	void cm_clone_functions_typer::type_functions_pair(std::ostream& output,
		const cpp_code_analysis::func_implementation_info<VarUsageConditionsN>& first_func_info,
		const cpp_code_analysis::func_implementation_info<VarUsageConditionsN>& second_func_info,
		const count_matrix::count_matrices_similarity_data& cm_similarity_data)
	{
		output << newline;
		output << right_arrow << right_arrow << tab << clones_str << newline;
		output << tab << tab << functions_similarity_str << convert_similarity(cm_similarity_data.matrices_relative_similarity()) << newline;

		output << right_arrow << tab << tab << first_clone_function_str << newline;
		type_func_info(output, first_func_info);
		output << right_arrow << tab << tab << second_clone_function_str << newline;
		type_func_info(output, second_func_info);

		output << tab << tab << matched_variables << newline;
		for (const auto& similar_vectors_data : cm_similarity_data)
		{
			output << tab << tab << tab << variables_similarity_str << convert_similarity(similar_vectors_data.relative_similarity()) << newline;
			output << tab << tab << tab << matched_variable_from_first_function_str << newline;
			type_var_info(output, first_func_info[cpp_code_analysis::index_of_var_usage_info(similar_vectors_data.index_of_first_vector().to_size_t())]);
			output << tab << tab << tab << matched_variable_from_second_function_str << newline;
			type_var_info(output, second_func_info[cpp_code_analysis::index_of_var_usage_info(similar_vectors_data.index_of_second_vector().to_size_t())]);
			output << newline;
		}
	}

	template <size_t VarUsageConditionsN>
	void cm_clone_functions_typer::type_func_info(std::ostream& output,
		const cpp_code_analysis::func_implementation_info<VarUsageConditionsN>& func_info)
	{
		output << tab << tab << tab << function_spelling_str << func_info.spelling().to_string() << newline;
		output << tab << tab << tab << function_location_str;
		output << file_str << func_info.location().filename().string() << line_str << func_info.location().line() << column_str << func_info.location().column() << newline;
	}

	template <size_t VarUsageConditionsN>
	void cm_clone_functions_typer::type_var_info(std::ostream& output,
		const cpp_code_analysis::var_usage_info<VarUsageConditionsN>& var_usage_info)
	{
		output << tab << tab << tab << variable_spelling_str << var_usage_info.spelling().to_string() << newline;
		output << tab << tab << tab << variable_location_str;
		output << file_str << var_usage_info.location().filename().string();
		output << line_str << var_usage_info.location().line() << column_str << var_usage_info.location().column() << newline;
	}
}
