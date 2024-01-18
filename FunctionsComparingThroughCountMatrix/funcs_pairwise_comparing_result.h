#pragma once
#include "funcs_comparing_result.h"
#include "pair_of_funcs_indices.h"
#include "funcs_pair_index.h"
#include <map>
#include <ranges>

namespace funcs_comparing_through_cm
{
	template <funcs_comparing_traits CompTraits>
	class funcs_pairwise_comparing_result final
	{
		using similarity_t = typename CompTraits::similarity_t;
		using similarity_greater = typename CompTraits::similarity_greater;

		std::map<pair_of_funcs_indices, funcs_comparing_result<CompTraits>> comparing_results_by_funcs_indices_{};
		std::vector<pair_of_funcs_indices> funcs_pairs_decreasing_similarity_order_{};

	public:
		funcs_pairwise_comparing_result() = default;

		explicit funcs_pairwise_comparing_result(
			std::map<pair_of_funcs_indices, funcs_comparing_result<CompTraits>> comparing_results_by_funcs_indices)
				: comparing_results_by_funcs_indices_(std::move(comparing_results_by_funcs_indices))
		{
			std::multimap<similarity_t, pair_of_funcs_indices, similarity_greater> funcs_indices_by_decreasing_similarity{};

			for (const auto& [funcs_indices, comparing_result] : comparing_results_by_funcs_indices_)
			{
				const funcs_similarity<similarity_t> funcs_similarity = comparing_result.similarity();
				const similarity_t similarity = funcs_similarity.to_matrices_similarity().to_similarity_t();
				funcs_indices_by_decreasing_similarity.emplace(funcs_indices, similarity);
			}

			funcs_pairs_decreasing_similarity_order_.reserve(comparing_results_by_funcs_indices_.size());
			for (auto funcs_indices : funcs_indices_by_decreasing_similarity | std::views::values)
			{
				funcs_pairs_decreasing_similarity_order_.emplace_back(funcs_indices);
			}
		}

		[[nodiscard]] const funcs_comparing_result<CompTraits>& get_comparing_result(
			const pair_of_funcs_indices& compared_funcs_indices) const
		{
			return comparing_results_by_funcs_indices_.at(compared_funcs_indices);
		}

		[[nodiscard]] pair_of_funcs_indices get_funcs_indices(
			const funcs_pair_index pair_index_in_decreasing_similarity_order) const
		{
			return funcs_pairs_decreasing_similarity_order_.at(pair_index_in_decreasing_similarity_order.to_size_t());
		}

		[[nodiscard]] size_t compared_pairs_count() const noexcept
		{
			return funcs_pairs_decreasing_similarity_order_.size();
		}
	};
}
