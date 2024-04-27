#pragma once
#include "funcs_pair_comparing_result_saver_abstract.h"
#include "var_descriptor_creator_abstract.h"
#include <format>

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class funcs_pair_comparing_result_saver_default final : public funcs_pair_comparing_result_saver_abstract<ConditionsCount>
	{
		static inline const std::string start_end_line{ "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n" };
		static constexpr size_t one_hundred_percentages = 100;

	public:
		void save(std::ostream& output, const funcs_pair_comparing_result<ConditionsCount>& result,
			const std::shared_ptr<const code_analysis::var_descriptor_creator_abstract> var_descriptor_creator) const override
		{
			output << '\n';
			output << start_end_line;

			output << "\tClones:\n";
			output << std::format("\tFunctions similarity: {}%\n", to_percentages(result.comparing_result().similarity().to_similarity_t().to_double()));

			output << "\tfirst clone function:\n";
			save_descriptor(output, result.first_func_descriptor().to_code_entity_descriptor());
			output << "\tsecond clone function:\n";
			save_descriptor(output, result.second_func_descriptor().to_code_entity_descriptor());
			output << '\n';

			output << "\tMatched variables:\n";
			for (cm::matched_vectors_index matched_vectors_pair{ 0 };
				matched_vectors_pair.to_size_t() < result.comparing_result().matched_vectors_count();
				matched_vectors_pair = cm::matched_vectors_index{ matched_vectors_pair.to_size_t() + 1 })
			{
				const auto& variables_similarity = result.comparing_result().distance_between_matched_vectors(matched_vectors_pair).to_edge_weight().value();
				output << std::format("\tVariables similarity: {}%\n", to_percentages(variables_similarity));

				const auto first_var_index = result.comparing_result().vector_from_first_cm(matched_vectors_pair);
				const auto second_var_index = result.comparing_result().vector_from_second_cm(matched_vectors_pair);
				const auto& first_func_vars_ids = result.first_func_implementation().nested_entities_ids();
				const auto& second_func_vars_ids = result.second_func_implementation().nested_entities_ids();
				if (first_var_index.to_size_t() >= first_func_vars_ids.size() || second_var_index.to_size_t() >= second_func_vars_ids.size())
				{
					continue;
				}

				const auto first_var_id = first_func_vars_ids.at(first_var_index.to_size_t());
				const auto second_var_id = second_func_vars_ids.at(second_var_index.to_size_t());
				const auto& first_var_descriptor = var_descriptor_creator->create(first_var_id);
				const auto& second_var_descriptor = var_descriptor_creator->create(second_var_id);

				output << "\t\tmatched variable from first function:\n";
				save_descriptor(output, first_var_descriptor.to_code_entity_descriptor());

				output << '\n';

				output << "\t\tmatched variable from second function:\n";
				save_descriptor(output, second_var_descriptor.to_code_entity_descriptor());
			}

			output << start_end_line;
		}

	private:
		static void save_descriptor(std::ostream& output, const code_analysis::code_entity_descriptor& descriptor)
		{
			output << std::format("\t\tSpelling: {}\n", descriptor.spelling().to_string());

			const auto& location = descriptor.location();
			const auto& first_symbol_position = location.first_symbol_position();
			output << std::format("\t\tLocation:\n\t\t\tfile: {}\n\t\t\tline: {}\n\t\t\tcolumn: {}\t\t\toffset: {}\n",
				location.filename().string(), first_symbol_position.line(), first_symbol_position.column(), first_symbol_position.offset_from_file_start());
		}

		[[nodiscard]] static size_t to_percentages(const double similarity)
		{
			return static_cast<size_t>(std::round(similarity * one_hundred_percentages));
		}
	};
}
