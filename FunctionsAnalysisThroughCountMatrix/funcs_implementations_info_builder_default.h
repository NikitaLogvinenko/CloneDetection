#pragma once
#include "funcs_implementations_info_builder_abstract.h"
#include "func_id_hash.h"
#include "var_id_hash.h"
#include <unordered_map>

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_implementations_info_builder_default final : public funcs_implementations_info_builder_abstract<UsageConditionsCount>
	{
		using conditions_counters = std::vector<cm::counted_value>;

		using conditions_counters_by_var_id_unordered_map = std::unordered_map<
			code_analysis::var_id, conditions_counters, code_analysis::var_id_hash>;

		using vars_usage_info_by_func_id_unordered_map = std::unordered_map<
			code_analysis::func_id, conditions_counters_by_var_id_unordered_map, code_analysis::func_id_hash>;

		vars_usage_info_by_func_id_unordered_map vars_info_by_func_id_{};

	public:
		void increment_condition(const code_analysis::func_id& func, const code_analysis::var_id& var, const condition_index<UsageConditionsCount> index) override
		{
			if (vars_info_by_func_id_.contains(func))
			{
				auto& conditions_counters_by_var_id = vars_info_by_func_id_[func];
				if (conditions_counters_by_var_id.contains(var))
				{
					++conditions_counters_by_var_id[var][index.to_size_t()];
					return;
				}

				auto [conditions_by_var_iterator, _] = conditions_counters_by_var_id.emplace(var, conditions_counters(UsageConditionsCount));
				auto& conditions_counters = conditions_by_var_iterator->second;
				++conditions_counters[index.to_size_t()];
				return;
			}

			conditions_counters_by_var_id_unordered_map new_var_with_zeroed_counters{ { var, conditions_counters(UsageConditionsCount) } };
			auto [vars_info_by_func_iterator, _] = vars_info_by_func_id_.emplace(func, std::move(new_var_with_zeroed_counters));
			auto& conditions_counters_by_var = vars_info_by_func_iterator->second;
			auto& conditions_counters = conditions_counters_by_var.begin()->second;
			++conditions_counters[index.to_size_t()];
		}

		[[nodiscard]] std::vector<func_implementation_info<UsageConditionsCount>> build_and_reset() override
		{
			std::vector<func_implementation_info<UsageConditionsCount>> funcs_implementations_info{};
			funcs_implementations_info.reserve(vars_info_by_func_id_.size());

			for (auto& [func, counters_by_var] : vars_info_by_func_id_)
			{
				func_implementation_info<UsageConditionsCount> func_info = construct_func_info(func, std::move(counters_by_var));
				funcs_implementations_info.emplace_back(std::move(func_info));
			}

			vars_info_by_func_id_.clear();
			return funcs_implementations_info;
		}

	private:
		[[nodiscard]] func_implementation_info<UsageConditionsCount> construct_func_info(code_analysis::func_id func,
			conditions_counters_by_var_id_unordered_map conditions_counters_by_var) const
		{
			std::vector<code_analysis::var_id> vector_of_vars_ids{};
			vector_of_vars_ids.reserve(conditions_counters_by_var.size());

			std::vector<cm::count_vector<UsageConditionsCount>> vector_of_count_vectors{};
			vector_of_count_vectors.reserve(conditions_counters_by_var.size());

			while (!conditions_counters_by_var.empty())
			{
				auto var_id_and_counters_node = conditions_counters_by_var.extract(conditions_counters_by_var.cbegin());
				vector_of_vars_ids.emplace_back(var_id_and_counters_node.key());
				vector_of_count_vectors.emplace_back(std::move(var_id_and_counters_node.mapped()));
			}

			cm::count_matrix<UsageConditionsCount> vars_usage_count_matrix(std::move(vector_of_count_vectors));

			return func_implementation_info<UsageConditionsCount>{ func, std::move(vector_of_vars_ids), std::move(vars_usage_count_matrix) };
		}
	};
}
