#pragma once
#include "cmcd_results_saver_abstract.h"
#include "func_descriptor_creator_abstract.h"
#include "var_descriptor_creator_abstract.h"
#include "clones_filter_abstract.h"
#include "id_hash.h"
#include "parameters_validation.h"
#include "funcs_pair_comparing_result_saver_abstract.h"
#include <memory>

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class cmcd_results_saver_default final : public cmcd_results_saver_abstract<ConditionsCount>
	{
		using func_id = code_analysis::func_id;
		using var_id = code_analysis::var_id;
		using func_descriptor = code_analysis::func_descriptor;
		using var_descriptor = code_analysis::var_descriptor;
		using funcs_descriptors_map = std::unordered_map<func_id, func_descriptor, utility::id_hash<func_id>>;
		using vars_descriptors_map = std::unordered_map<var_id, var_descriptor, utility::id_hash<var_id>>;

		std::unique_ptr<const code_analysis::func_descriptor_creator_abstract> func_descriptor_creator_{};
		std::shared_ptr<const code_analysis::var_descriptor_creator_abstract> var_descriptor_creator_{};
		std::unique_ptr<const clones_filter_abstract<ConditionsCount>> clones_filter_{};
		std::unique_ptr<const funcs_pair_comparing_result_saver_abstract<ConditionsCount>> funcs_pair_saver_{};

	public:
		cmcd_results_saver_default(
			std::unique_ptr<const code_analysis::func_descriptor_creator_abstract> func_descriptor_creator,
			std::unique_ptr<const code_analysis::var_descriptor_creator_abstract> var_descriptor_creator,
			std::unique_ptr<const clones_filter_abstract<ConditionsCount>> clones_filter,
			std::unique_ptr<const funcs_pair_comparing_result_saver_abstract<ConditionsCount>> funcs_pair_saver)
			: func_descriptor_creator_{ std::move(func_descriptor_creator) },
			var_descriptor_creator_{ std::move(var_descriptor_creator) },
			clones_filter_(std::move(clones_filter)),
			funcs_pair_saver_(std::move(funcs_pair_saver))
		{
			utility::throw_if_nullptr(func_descriptor_creator_.get(), "cmcd_results_saver_default::cmcd_results_saver_default", "func_descriptor_creator");
			utility::throw_if_nullptr(var_descriptor_creator_.get(), "cmcd_results_saver_default::cmcd_results_saver_default", "var_descriptor_creator");
			utility::throw_if_nullptr(clones_filter_.get(), "cmcd_results_saver_default::cmcd_results_saver_default", "clones_filter");
			utility::throw_if_nullptr(funcs_pair_saver_.get(), "cmcd_results_saver_default::cmcd_results_saver_default", "funcs_pair_saver");
		}

		void save(std::ostream& output, const cmcd_result<ConditionsCount>& result,
		          const cmcd_results_saver_config& config) const override
		{
			output << std::format("\n~~~ Count matrix based clone-functions detection ~~~\n<MinSimilarity> {}\n<MinVariables> {}\n\n", 
				config.min_similarity(), config.min_variables());

			funcs_descriptors_map func_descriptor_by_id{};
			std::map<double, funcs_pair_comparing_result<ConditionsCount>, std::greater<double>> result_by_similarity{};

			for (const auto first_func_id : result.first_set_of_entities())
			{
				const auto& first_func_implementation = result.first_project_implementations_info().at(first_func_id);
				const auto& first_func_descriptor = get_descriptor(func_descriptor_by_id, first_func_id, *func_descriptor_creator_);

				for (const auto second_func_id : result.second_set_of_entities())
				{
					const auto& second_func_implementation = result.second_project_implementations_info().at(second_func_id);
					const auto& second_func_descriptor = get_descriptor(func_descriptor_by_id, second_func_id, *func_descriptor_creator_);

					const auto& comparing_result = result.comparing_result().get_result(first_func_id, second_func_id);
					const double similarity = comparing_result.similarity().to_similarity_t().to_double();

					funcs_pair_comparing_result<ConditionsCount> funcs_pair_result{ first_func_descriptor, second_func_descriptor,
						first_func_implementation, second_func_implementation, comparing_result };

					if (!clones_filter_->clones(funcs_pair_result, config))
					{
						continue;
					}

					if (config.exclude(first_func_descriptor.to_code_entity_descriptor().location().filename())
					|| config.exclude(second_func_descriptor.to_code_entity_descriptor().location().filename()))
					{
						continue;
					}

					result_by_similarity.emplace(similarity, std::move(funcs_pair_result));
				}
			}

			for (const auto& [_, funcs_pair_result] : result_by_similarity)
			{
				funcs_pair_saver_->save(output, funcs_pair_result, var_descriptor_creator_);
			}
		}

	private:
		template <class IdT, class DescriptorT, class DescriptorCreatorT>
		[[nodiscard]] const DescriptorT& get_descriptor(
			std::unordered_map<IdT, DescriptorT, utility::id_hash<IdT>>& descriptors_map, 
			const IdT id, const DescriptorCreatorT& creator) const
		{
			if (!descriptors_map.contains(id))
			{
				descriptors_map[id] = creator.create(id);
				
			}

			return descriptors_map[id];
		}
	};
}
