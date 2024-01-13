#pragma once
#include "func_implementation_info.h"
#include "var_usage_condition_descriptor.h"
#include <unordered_map>

namespace functions_analysis
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_implementations_info_builder final
	{
	public:
		using func_info = func_implementation_info<UsageConditionsCount>;

	private:
		using usage_conditions_counters = std::vector<cm::counted_value>;
		using counters_by_var_descriptor = std::unordered_map<var_descriptor, usage_conditions_counters>;

		std::unordered_map<func_descriptor, counters_by_var_descriptor> vars_info_by_func_descriptor_{};

	public:
		void add_condition(var_usage_condition_descriptor<UsageConditionsCount> usage_condition);

		[[nodiscard]] std::vector<func_info> build_and_reset();

	private:
		[[nodiscard]] func_info construct_func_info(func_descriptor func, counters_by_var_descriptor counters_by_vars) const;
	};

	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	void funcs_implementations_info_builder<UsageConditionsCount>::add_condition(
		var_usage_condition_descriptor<UsageConditionsCount> usage_condition)
	{
		condition_index<UsageConditionsCount> condition_index = usage_condition.index();
		const auto [var_descr, func_descr] = std::move(usage_condition).var_and_func();

		if (vars_info_by_func_descriptor_.contains(func_descr))
		{
			auto& conditions_counters_by_var_descrs = vars_info_by_func_descriptor_[func_descr];
			if (conditions_counters_by_var_descrs.contains(var_descr))
			{
				++conditions_counters_by_var_descrs[var_descr][condition_index];
				return;
			}

			auto [iterator_conditions_by_var, _] = conditions_counters_by_var_descrs.emplace(
				std::move(var_descr), usage_conditions_counters(UsageConditionsCount));
			auto& conditions_counters = iterator_conditions_by_var->second;
			++conditions_counters[condition_index];
			return;
		}

		counters_by_var_descriptor new_var_with_zeroed_counters{
			{ std::move(var_descr), usage_conditions_counters(UsageConditionsCount) }
		};
		auto [iterator_vars_info_by_func, _] =
			vars_info_by_func_descriptor_.emplace(std::move(func_descr), std::move(new_var_with_zeroed_counters));
		auto& counters_by_var_descr = iterator_vars_info_by_func->second;
		auto& conditions_counters = counters_by_var_descr.begin()->second;
		++conditions_counters[condition_index];
	}

	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	std::vector<typename funcs_implementations_info_builder<UsageConditionsCount>::func_info>
	funcs_implementations_info_builder<UsageConditionsCount>::build_and_reset()
	{
		std::vector<func_info> funcs_implementations_info{};
		funcs_implementations_info.reserve(vars_info_by_func_descriptor_.size());

		for (auto& [func_descriptor, counters_by_vars] : vars_info_by_func_descriptor_)
		{
			func_info implementation_info = construct_func_info(std::move(func_descriptor), std::move(counters_by_vars));
			funcs_implementations_info.emplace_back(std::move(implementation_info));
		}

		vars_info_by_func_descriptor_.clear();
		return funcs_implementations_info;
	}

	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	typename funcs_implementations_info_builder<UsageConditionsCount>::func_info funcs_implementations_info_builder<
	UsageConditionsCount>::
	construct_func_info(func_descriptor func, counters_by_var_descriptor counters_by_vars) const
	{
		std::vector<cm::count_vector<UsageConditionsCount>> vector_of_count_vectors{};
		vector_of_count_vectors.reserve(counters_by_vars.size());
		std::vector<var_usage_info<UsageConditionsCount>> vector_of_vars_usage_info{};
		vector_of_vars_usage_info.reserve(counters_by_vars.size());

		for (auto& [var_descr, conditions_counters] : counters_by_vars)
		{
			vector_of_vars_usage_info.emplace_back(std::move(var_descr), std::move(conditions_counters));
			auto& last_created_count_vector = vector_of_vars_usage_info.back().conditions_counters();
			vector_of_count_vectors.emplace_back(last_created_count_vector);
		}

		cm::count_matrix<UsageConditionsCount> vars_usage_count_matrix(std::move(vector_of_count_vectors));

		return func_info{ std::move(func), std::move(vector_of_vars_usage_info), std::move(vars_usage_count_matrix) };
	}
}
