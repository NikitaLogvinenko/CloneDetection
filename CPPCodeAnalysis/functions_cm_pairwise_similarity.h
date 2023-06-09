#pragma once
#include "analysed_functions_info.h"
#include "cm_similarity_abstract_algorithm.h"
#include <map>

namespace cpp_code_analysis
{
	template <size_t VarUsageConditionsN>
	class functions_cm_pairwise_similarity final
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
			: public std::multimap<count_matrix_ns::relative_similarity, func_indices_pair>::const_iterator
		{
		public:
			explicit const_func_indices_pair_by_similarity_iterator(
				const typename std::multimap<count_matrix_ns::relative_similarity, func_indices_pair>::const_iterator& multimap_iterator) noexcept
				: std::multimap<count_matrix_ns::relative_similarity, func_indices_pair>::const_iterator(multimap_iterator) {}
		};

		analysed_functions_info<VarUsageConditionsN> analysed_functions_info_{};
		std::map<func_indices_pair, count_matrix_ns::count_matrices_similarity_data> similarity_data_by_func_indices_pair_{};
		std::multimap<count_matrix_ns::relative_similarity, func_indices_pair> func_indices_pair_by_similarity_{};

	public:
		class builder final
		{
		public:
			builder() = delete;
			[[nodiscard]] static functions_cm_pairwise_similarity build(
				analysed_functions_info<VarUsageConditionsN> functions_info,
				const count_matrix_ns::cm_similarity_abstract_algorithm<VarUsageConditionsN>& similarity_abstract_algorithm);
		};

		[[nodiscard]] const analysed_functions_info<VarUsageConditionsN>& get_analysed_functions_info() const noexcept
		{
			return analysed_functions_info_;
		}

		[[nodiscard]] const_func_indices_pair_by_similarity_iterator lower_bound_similarity_func_indices_pair(
			const count_matrix_ns::relative_similarity lower_similarity_bound) const noexcept
		{
			return const_func_indices_pair_by_similarity_iterator{ func_indices_pair_by_similarity_.lower_bound(lower_similarity_bound) };
		}

		[[nodiscard]] const_func_indices_pair_by_similarity_iterator upper_bound_similarity_func_indices_pair(
			const count_matrix_ns::relative_similarity upper_similarity_bound) const noexcept
		{
			return const_func_indices_pair_by_similarity_iterator{ func_indices_pair_by_similarity_.upper_bound(upper_similarity_bound) };
		}

		[[nodiscard]] const count_matrix_ns::count_matrices_similarity_data& get_functions_similarity_data(
			const func_indices_pair func_indices) const
		{
			return similarity_data_by_func_indices_pair_.at(func_indices);
		}

		[[nodiscard]] const_func_indices_pair_by_similarity_iterator begin() const noexcept
		{
			return const_func_indices_pair_by_similarity_iterator{ func_indices_pair_by_similarity_.cbegin() };
		}

		[[nodiscard]] const_func_indices_pair_by_similarity_iterator end() const noexcept
		{
			return const_func_indices_pair_by_similarity_iterator{ func_indices_pair_by_similarity_.cend() };
		}

	private:
		explicit functions_cm_pairwise_similarity(
			analysed_functions_info<VarUsageConditionsN> analysed_functions_info = {},
			std::map<func_indices_pair, count_matrix_ns::count_matrices_similarity_data> similarity_data_by_func_indices_pair = {},
			std::multimap<count_matrix_ns::relative_similarity, func_indices_pair> func_indices_pair_by_similarity = {}) noexcept :
				analysed_functions_info_(std::move(analysed_functions_info)),
				similarity_data_by_func_indices_pair_(std::move(similarity_data_by_func_indices_pair)),
				func_indices_pair_by_similarity_(std::move(func_indices_pair_by_similarity)) {}

	};

	template <size_t VarUsageConditionsN>
	functions_cm_pairwise_similarity<VarUsageConditionsN>
	functions_cm_pairwise_similarity<VarUsageConditionsN>::builder::build(
		analysed_functions_info<VarUsageConditionsN> functions_info,
		const count_matrix_ns::cm_similarity_abstract_algorithm<VarUsageConditionsN>& similarity_abstract_algorithm)
	{
		std::map<func_indices_pair, count_matrix_ns::count_matrices_similarity_data> similarity_data_by_func_indices_pair{};
		std::multimap<count_matrix_ns::relative_similarity, func_indices_pair> func_indices_pair_by_similarity{};

		auto similarity_by_indices_iterator = similarity_data_by_func_indices_pair.begin();
		for (size_t first_func_index = 0; first_func_index < functions_info.functions_count(); ++first_func_index)
		{
			for (size_t second_func_index = first_func_index; second_func_index < functions_info.functions_count(); ++second_func_index)
			{
				similarity_by_indices_iterator = similarity_data_by_func_indices_pair.emplace_hint(
					similarity_by_indices_iterator, func_indices_pair{ first_func_index, second_func_index },
					similarity_abstract_algorithm.determine_similarity(
						functions_info[analysed_func_info_index(first_func_index)].variables_usage_count_matrix(),
						functions_info[analysed_func_info_index(second_func_index)].variables_usage_count_matrix()));
				const auto& [current_indices_pair, current_similarity_data] = *similarity_by_indices_iterator;
				func_indices_pair_by_similarity.emplace(current_similarity_data.matrices_relative_similarity(), current_indices_pair);
			}
		}

		return functions_cm_pairwise_similarity{
			std::move(functions_info), std::move(similarity_data_by_func_indices_pair), std::move(func_indices_pair_by_similarity) };
	}
}
