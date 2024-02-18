#pragma once
#include "functions_cm_pairwise_similarity.h"
#include "func_implementation_info_equal_spelling_and_location.h"
#include <ostream>

namespace clone_detection_io
{
	class cm_clone_functions_typer final
	{
		inline static const std::string header{
			"\n~~~ Count matrix based clone-functions detection ~~~\nfunctions_similarity_threshold="
		};

		static constexpr size_t one_hundred_percent = 100;

	public:
		cm_clone_functions_typer() = delete;

		template <size_t VarUsageConditionsN>
		static void type(
			std::ostream& output,
			const cpp_code_analysis::functions_cm_pairwise_similarity<VarUsageConditionsN>& pairwise_similarity,
			cm::relative_similarity similarity_threshold);

	private:
		template <size_t VarUsageConditionsN>
		static void type_functions_pair(
			std::ostream& output,
			const cpp_code_analysis::func_implementation_info<VarUsageConditionsN>& first_func_info,
			const cpp_code_analysis::func_implementation_info<VarUsageConditionsN>& second_func_info,
			const cm::count_matrices_similarity_data& cm_similarity_data);

		static std::string convert_similarity(cm::relative_similarity relative_similarity);

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
		const cm::relative_similarity relative_similarity)
	{
		return std::to_string(std::lround(relative_similarity.to_double() * one_hundred_percent)) + "%";
	}

	template <size_t VarUsageConditionsN>
	void cm_clone_functions_typer::type(std::ostream& output,
	                                    const cpp_code_analysis::functions_cm_pairwise_similarity<VarUsageConditionsN>&
	                                    pairwise_similarity,
	                                    cm::relative_similarity similarity_threshold)
	{
		output << header << convert_similarity(similarity_threshold) << "\n";
		for (auto iterator = pairwise_similarity.lower_bound_similarity_func_indices_pair(similarity_threshold);
		     iterator != pairwise_similarity.end(); ++iterator)
		{
			const auto& [funcs_similarity, funcs_indices] = *iterator;
			const cm::count_matrices_similarity_data& cm_similarity_data = pairwise_similarity.
				get_functions_similarity_data(funcs_indices);
			const cpp_code_analysis::func_implementation_info<VarUsageConditionsN>& first_func_info =
				pairwise_similarity.get_analysed_functions_info()[funcs_indices.first_func_index()];
			const cpp_code_analysis::func_implementation_info<VarUsageConditionsN>& second_func_info =
				pairwise_similarity.get_analysed_functions_info()[funcs_indices.second_func_index()];

			if (!cpp_code_analysis::func_implementation_info_equal_spelling_and_location<VarUsageConditionsN>{}(
					first_func_info, second_func_info) &&
				first_func_info.variables_count() != 0 && second_func_info.variables_count() != 0)
			{
				type_functions_pair<VarUsageConditionsN>(output, first_func_info, second_func_info, cm_similarity_data);
			}
		}
	}

	template <size_t VarUsageConditionsN>
	void cm_clone_functions_typer::type_functions_pair(std::ostream& output,
	                                                   const cpp_code_analysis::func_implementation_info<
		                                                   VarUsageConditionsN>& first_func_info,
	                                                   const cpp_code_analysis::func_implementation_info<
		                                                   VarUsageConditionsN>& second_func_info,
	                                                   const cm::count_matrices_similarity_data& cm_similarity_data)
	{
		output << "\n";
		output << ">>\tclones:\n";
		output << "\t\tfunctions similarity: " << convert_similarity(cm_similarity_data.matrices_relative_similarity())
			<< "\n";

		output << ">>\t\tfirst clone function:\n";
		type_func_info(output, first_func_info);
		output << ">>\t\tsecond clone function:\n";
		type_func_info(output, second_func_info);

		output << "\t\tmatched_variables:\n";
		for (const auto& similar_vectors_data : cm_similarity_data)
		{
			output << "\t\t\t" << "variables_similarity: " << convert_similarity(
				similar_vectors_data.relative_similarity()) << "\n";

			output << "\t\t\t" << "matched_variable_from_first_function:\n";
			type_var_info(
				output, first_func_info[cpp_code_analysis::index_of_var_usage_info(
					similar_vectors_data.index_of_first_vector().to_size_t())]);
			output << "\t\t\t" << "matched_variable_from_second_function:\n";
			type_var_info(
				output, second_func_info[cpp_code_analysis::index_of_var_usage_info(
					similar_vectors_data.index_of_second_vector().to_size_t())]);

			output << "\n";
		}
	}

	template <size_t VarUsageConditionsN>
	void cm_clone_functions_typer::type_func_info(std::ostream& output,
	                                              const cpp_code_analysis::func_implementation_info<VarUsageConditionsN>
	                                              & func_info)
	{
		output << "\t\t\tfunction spelling: " << func_info.spelling().to_code_entity_spelling().to_string() << "\n";
		output << "\t\t\tfunction location: ";
		output << "\n\t\t\t\tfile: " << func_info.location().to_code_entity_location().filename().string() << "\n\t\t\t\tline: " << func_info.location().to_code_entity_location().line() <<
			"\n\t\t\t\tcolumn: " << func_info.location().to_code_entity_location().column() << "\n";
	}

	template <size_t VarUsageConditionsN>
	void cm_clone_functions_typer::type_var_info(std::ostream& output,
	                                             const cpp_code_analysis::var_usage_info<VarUsageConditionsN>&
	                                             var_usage_info)
	{
		output << "\t\t\t" << "variable_spelling: " << var_usage_info.spelling().to_code_entity_spelling().to_string() << "\n";
		output << "\t\t\t" << "variable_location: ";
		output << "\n\t\t\t\tfile: " << var_usage_info.location().to_code_entity_location().filename().string();
		output << "\n\t\t\t\tline: " << var_usage_info.location().to_code_entity_location().line() << "\n\t\t\t\tcolumn: " <<
			var_usage_info.location().to_code_entity_location().column() << "\n";
	}
}
