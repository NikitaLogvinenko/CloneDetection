#pragma once
#include "funcs_implementations_info_builder_abstract.h"
#include "func_descriptor_hash.h"
#include "var_descriptor_hash.h"
#include <unordered_map>

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_implementations_info_builder_default final : public funcs_implementations_info_builder_abstract<UsageConditionsCount>
	{
	public:
		using code_analysis::func_descriptor;
		using code_analysis::var_descriptor;

		using func_implementation_info = func_implementation_info<UsageConditionsCount>;

		using condition_index = condition_index<UsageConditionsCount>;

	private:
		using conditions_counters = std::vector<cm::counted_value>;

		using var_descriptor_conditions_counters_map = std::unordered_map<
			var_descriptor, conditions_counters, code_analysis::var_descriptor_hash>;

		using func_descriptor_vars_usage_info_map = std::unordered_map<
			func_descriptor, var_descriptor_conditions_counters_map, code_analysis::func_descriptor_hash>;

		func_descriptor_vars_usage_info_map vars_info_by_func_descriptor_{};

	public:
		void add_condition(const func_descriptor& func, const var_descriptor& var, condition_index index) override
		{
			if (vars_info_by_func_descriptor_.contains(func))
			{
				auto& conditions_counters_by_var_descr = vars_info_by_func_descriptor_[func];
				if (conditions_counters_by_var_descr.contains(var))
				{
					++conditions_counters_by_var_descr[var][index.to_size_t()];
					return;
				}

				auto [iterator_conditions_by_var, _] = conditions_counters_by_var_descr.emplace(
					std::move(var), conditions_counters(UsageConditionsCount));
				auto& conditions_counters = iterator_conditions_by_var->second;
				++conditions_counters[index.to_size_t()];
				return;
			}

			var_descriptor_conditions_counters_map new_var_with_zeroed_counters{
				{ std::move(var), conditions_counters(UsageConditionsCount) }
			};
			auto [iterator_vars_info_by_func, _] =
				vars_info_by_func_descriptor_.emplace(std::move(func), std::move(new_var_with_zeroed_counters));
			auto& counters_by_var_descr = iterator_vars_info_by_func->second;
			auto& conditions_counters = counters_by_var_descr.begin()->second;
			++conditions_counters[index.to_size_t()];
		}

		[[nodiscard]] std::vector<func_implementation_info> build_and_reset() override
		{
			std::vector<func_implementation_info> funcs_implementations_info{};
			funcs_implementations_info.reserve(vars_info_by_func_descriptor_.size());

			for (auto& [func_descriptor, counters_by_var] : vars_info_by_func_descriptor_)
			{
				func_implementation_info func_info = construct_func_info(std::move(func_descriptor), std::move(counters_by_var));
				funcs_implementations_info.emplace_back(std::move(func_info));
			}

			vars_info_by_func_descriptor_.clear();
			return funcs_implementations_info;
		}

	private:
		[[nodiscard]] func_implementation_info construct_func_info(func_descriptor func, 
			var_descriptor_conditions_counters_map condition_counters_by_var) const
		{
			std::vector<var_descriptor> vector_of_vars_descriptors{};
			vector_of_vars_descriptors.reserve(condition_counters_by_var.size());

			std::vector<cm::count_vector<UsageConditionsCount>> vector_of_count_vectors{};
			vector_of_count_vectors.reserve(condition_counters_by_var.size());

			while (!condition_counters_by_var.empty())
			{
				auto var_descr_and_counters_node = condition_counters_by_var.extract(condition_counters_by_var.cbegin());
				vector_of_vars_descriptors.emplace_back(std::move(var_descr_and_counters_node.key()));
				vector_of_count_vectors.emplace_back(std::move(var_descr_and_counters_node.mapped()));
			}

			cm::count_matrix<UsageConditionsCount> vars_usage_count_matrix(std::move(vector_of_count_vectors));

			return func_implementation_info{ std::move(func), std::move(vector_of_vars_descriptors), std::move(vars_usage_count_matrix) };
		}
	};
}
