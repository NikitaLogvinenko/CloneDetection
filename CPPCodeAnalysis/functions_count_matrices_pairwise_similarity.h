#pragma once
#include "analysed_functions_info.h"
#include "cm_similarity_abstract_algorithm.h"
#include <map>

namespace cpp_code_analysis
{
	template <size_t VarUsageConditionsN>
	class functions_count_matrices_pairwise_similarity final
	{
		class func_indices_pair final
		{
			analysed_func_info_index first_func_index_;
			analysed_func_info_index second_func_index_;

		public:
			explicit func_indices_pair(const size_t first_func_index, const size_t second_func_index) noexcept
				: first_func_index_(first_func_index), second_func_index_(second_func_index) {}

			[[nodiscard]] analysed_func_info_index first_func_index() const noexcept
			{
				return first_func_index_;
			}

			[[nodiscard]] analysed_func_info_index second_func_index() const noexcept
			{
				return second_func_index_;
			}

			[[nodiscard]] auto operator<=>(const func_indices_pair& other) const noexcept = default;
		};

		class const_func_indices_pair_by_similarity_iterator final
			: public std::map<func_indices_pair, count_matrix::count_matrices_similarity_data>::const_values_iterator
		{
		public:
			explicit const_func_indices_pair_by_similarity_iterator(
				const typename std::multimap<double, func_indices_pair>::const_values_iterator& multimap_iterator) noexcept
				: std::multimap<double, func_indices_pair>::const_values_iterator(multimap_iterator) {}
		};

		analysed_functions_info<VarUsageConditionsN> analysed_functions_info_{};
		std::map<func_indices_pair, count_matrix::count_matrices_similarity_data> similarity_data_by_func_indices_pair_{};
		std::multimap<double, func_indices_pair> func_indices_pair_by_similarity_{};

	public:
		class builder final
		{
		public:
			builder() = delete;
			[[nodiscard]] static functions_count_matrices_pairwise_similarity build(
				analysed_functions_info<VarUsageConditionsN> functions_info,
				const count_matrix::cm_similarity_abstract_algorithm<VarUsageConditionsN>& similarity_abstract_algorithm);
		};

		[[nodiscard]] const analysed_functions_info<VarUsageConditionsN>& get_analysed_functions_info() const noexcept
		{
			return analysed_functions_info_;
		}

		[[nodiscard]] const_func_indices_pair_by_similarity_iterator lower_similarity_func_indices_pair(const double lower_similarity_bound) const noexcept
		{
			return const_func_indices_pair_by_similarity_iterator(func_indices_pair_by_similarity_.lower_bound(lower_similarity_bound));
		}

		[[nodiscard]] const_func_indices_pair_by_similarity_iterator upper_similarity_func_indices_pair(const double upper_similarity_bound) const noexcept
		{
			return const_func_indices_pair_by_similarity_iterator(func_indices_pair_by_similarity_.upper_bound(upper_similarity_bound));
		}

		[[nodiscard]] const count_matrix::count_matrices_similarity_data& get_functions_similarity_data(func_indices_pair func_indices_pair) const
		{
			return similarity_data_by_func_indices_pair_.at(func_indices_pair);
		}

		[[nodiscard]] const count_matrix::count_matrices_similarity_data& get_functions_similarity_data_dont_check_indices(func_indices_pair func_indices_pair) const noexcept
		{
			return similarity_data_by_func_indices_pair_[func_indices_pair];
		}

	private:
		explicit functions_count_matrices_pairwise_similarity(
			analysed_functions_info<VarUsageConditionsN> analysed_functions_info = {},
			std::map<func_indices_pair, count_matrix::count_matrices_similarity_data> similarity_data_by_func_indices_pair = {},
			std::multimap<double, func_indices_pair> func_indices_pair_by_similarity = {}) noexcept :
			analysed_functions_info_(std::move(analysed_functions_info)),
			similarity_data_by_func_indices_pair_(std::move(similarity_data_by_func_indices_pair)),
			func_indices_pair_by_similarity_(std::move(func_indices_pair_by_similarity)) {}

	};

	template <size_t VarUsageConditionsN>
	functions_count_matrices_pairwise_similarity<VarUsageConditionsN>
	functions_count_matrices_pairwise_similarity<VarUsageConditionsN>::builder::build(
		analysed_functions_info<VarUsageConditionsN> functions_info,
		const count_matrix::cm_similarity_abstract_algorithm<VarUsageConditionsN>& similarity_abstract_algorithm)
	{
		std::map<func_indices_pair, count_matrix::count_matrices_similarity_data> similarity_data_by_func_indices_pair{};
		std::multimap<double, func_indices_pair> func_indices_pair_by_similarity{};

		auto similarity_by_indices_iterator = similarity_data_by_func_indices_pair.cbegin();
		for (size_t first_func_index = 0; first_func_index < functions_info.functions_count(); ++first_func_index)
		{
			for (size_t second_func_index = first_func_index; second_func_index < functions_info.functions_count(); ++second_func_index)
			{
				similarity_by_indices_iterator = similarity_data_by_func_indices_pair_.emplace_hint(
					similarity_by_indices_iterator, func_indices_pair{ first_func_index, second_func_index },
					similarity_abstract_algorithm.determine_similarity(
						analysed_functions_info_[analysed_func_info_index(first_func_index)].variables_usage_count_matrix(),
						analysed_functions_info_[analysed_func_info_index(second_func_index)].variables_usage_count_matrix()));
				const auto& [current_indices_pair, current_similarity_data] = *similarity_by_indices_iterator;
				func_indices_pair_by_similarity.emplace(current_similarity_data.matrices_similarity(), current_indices_pair);
			}
		}

		return functions_count_matrices_pairwise_similarity{
			std::move(functions_info), std::move(similarity_data_by_func_indices_pair), std::move(func_indices_pair_by_similarity) };
	}
}
