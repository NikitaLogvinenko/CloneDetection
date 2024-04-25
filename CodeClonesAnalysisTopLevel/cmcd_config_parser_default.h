#pragma once
#include "cmcd_config_parser_abstract.h"

namespace code_clones_analysis_top_level
{
	class cmcd_config_parser_default final : public cmcd_config_parser_abstract
	{
	public:
		[[nodiscard]] count_matrix_clones_detection_config parse(std::istream& input) override;

	private:
		static void process_output(std::string token, count_matrix_clones_detection_config& config);
		static void process_framework(std::string token, count_matrix_clones_detection_config& config);
		static void process_first_project_dir(std::string token, count_matrix_clones_detection_config& config);
		static void process_second_project_dir(std::string token, count_matrix_clones_detection_config& config);
		static void process_min_similarity(std::string token, count_matrix_clones_detection_config& config);
		static void process_min_variables(std::string token, count_matrix_clones_detection_config& config);
		static void process_code_analysis_threads(std::string token, count_matrix_clones_detection_config& config);
		static void process_count_matrices_comparing_threads(std::string token, count_matrix_clones_detection_config& config);
		static void process_matrices_comparing_algorithm(std::string token, count_matrix_clones_detection_config& config);
		static void process_additional_argument(const std::string& argument_name, const std::string& token, count_matrix_clones_detection_config& config);

		inline static const std::string output_prefix{ "<Output>" };
		inline static const std::string framework_prefix{ "<Framework>" };
		inline static const std::string first_project_dir_prefix{ "<FirstProjectDirectory>" };
		inline static const std::string second_project_dir_prefix{ "<SecondProjectDirectory>" };
		inline static const std::string min_similarity_prefix{ "<MinSimilarity>" };
		inline static const std::string min_variables_prefix{ "<MinVariables>" };
		inline static const std::string code_analysis_threads_prefix{ "<CodeAnalysisThreads>" };
		inline static const std::string count_matrices_comparing_threads_prefix{ "<CountMatricesComparingThreads>" };
		inline static const std::string matrices_comparing_algorithm_prefix{ "<MatricesComparingAlgorithm>" };


		inline static const std::unordered_map<
			std::string, void(*)(std::string, count_matrix_clones_detection_config&)> arg_processing_by_prefix
		{
			{output_prefix, &process_output},
			{framework_prefix, &process_framework},
			{first_project_dir_prefix, &process_first_project_dir},
			{second_project_dir_prefix, &process_second_project_dir},
			{min_similarity_prefix, &process_min_similarity},
			{min_variables_prefix, &process_min_variables},
			{code_analysis_threads_prefix, &process_code_analysis_threads},
			{count_matrices_comparing_threads_prefix, &process_matrices_comparing_algorithm},
			{matrices_comparing_algorithm_prefix, &process_matrices_comparing_algorithm}
		};
	};
}
