#include "functions_cmcd_console_application.h"
#include "cm_max_weighted_bipartite_matching.h"
#include "func_implementation_analysis_director.h"
#include "func_implementation_analysis_builder_default.h"
#include "cxindex_raii.h"
#include "translation_unit_raii.h"
#include "cm_clone_functions_typer.h"
#include "existed_file_rewriting_error.h"
#include "file_not_opened_error.h"
#include "types_conversion_error.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace clone_detection_io
{
	using namespace clang_c_adaptation;
	using namespace cm;
	using namespace cpp_code_analysis;

	namespace
	{
		constexpr size_t default_argv_index = 0;
		constexpr size_t funcs_similarity_threshold_index = default_argv_index + 1;
		constexpr size_t save_filename_index = funcs_similarity_threshold_index + 1;
		constexpr size_t first_ast_filename_index = save_filename_index + 1;
		constexpr size_t min_argc = first_ast_filename_index + 1;

		const std::string link_for_info{"https://github.com/NikitaLogvinenko/CloneDetection/tree/master"};

		void print_help()
		{
			std::cout << "> Console application for finding clone-functions in C++ code.\n";
			std::cout << "> Command line arguments:\n";
			std::cout <<
				"> First arg: functions_similarity_threshold - must be from 0 (absolutely different) to 1 (absolutely identical).\n";
			std::cout << "> Functions which similarities greater or equal will be declared CLONES.\n";
			std::cout << "> Second arg: path_to_save_results - filename where clone detection results will be saved.\n";
			std::cout << "> File mustn't exist before, since it will be created during application execution.\n";
			std::cout <<
				"> Other args: pathes to existing dumped clang-AST files (abstract syntax trees) of translation units that must be analysed.\n";
			std::cout << "> More information see here: " << link_for_info << "\n";
		}

		relative_similarity convert_similarity(const std::string& func_similarity_threshold)
		{
			std::istringstream iss(func_similarity_threshold);
			double similarity;
			iss >> similarity;

			if (!iss)
			{
				throw common_exceptions::types_conversion_error(
					"Can not convert functions_similarity_threshold to double.\n");
			}

			return relative_similarity{similarity};
		}
	}

	void functions_cmcd_console_application::execute(const std::vector<std::string>& argv)
	{
		std::cout << std::endl;
		std::cout << "Clone detection launched...\n";

		if (argv.size() < min_argc)
		{
			std::cout << std::format("Not enough arguments. Minimum {} are required, but {} were passed.\n",
			                         min_argc - 1, argv.size() - 1);
			print_help();
			return;
		}

		try
		{
			const relative_similarity funcs_similarity_threshold{
				convert_similarity(argv[funcs_similarity_threshold_index])
			};

			const std::filesystem::path save_filename(argv[save_filename_index]);
			if (exists(save_filename))
			{
				throw common_exceptions::existed_file_rewriting_error(
					"Filename for results saving is not empty. Try another filename.\n");
			}

			const cxindex_raii cxindex{};
			std::vector<translation_unit_raii> translation_units{};
			translation_units.reserve(argv.size() - first_ast_filename_index);
			for (size_t tu_path_index = first_ast_filename_index; tu_path_index < argv.size(); ++tu_path_index)
			{
				translation_units.emplace_back(cxindex, std::filesystem::path(argv[tu_path_index]));
			}

			std::unique_ptr<func_implementation_analysis_builder_abstract<default_conditions_total>>
				func_analysis_builder
					= std::make_unique<func_implementation_analysis_builder_default>();

			analysed_functions_info<default_conditions_total> analysed_functions_info =
				func_implementation_analysis_director::analyse_all_units(
					std::move(func_analysis_builder), translation_units);

			const auto funcs_pairwise_similarity = functions_cm_pairwise_similarity<default_conditions_total>
				::builder::build(std::move(analysed_functions_info),
				                 cm_max_weighted_bipartite_matching<default_conditions_total>{});

			std::cout << "Saving results...\n";
			std::ofstream save_file(save_filename);
			if (!save_file)
			{
				throw common_exceptions::file_not_opened_error(
					"Can not create or open file for results saving.\n");
			}

			cm_clone_functions_typer::type(save_file, funcs_pairwise_similarity, funcs_similarity_threshold);
			std::cout << "Detection performed!\n";
		}
		catch (const std::exception& ex)
		{
			std::cout << "Execution was terminated due to exception.\n" << ex.what() << "\nTry again!\n";
		}
	}
}
