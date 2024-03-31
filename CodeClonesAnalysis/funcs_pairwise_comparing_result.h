#pragma once
#include "pair_of_funcs_ids.h"
#include "funcs_similarity.h"
#include <unordered_map>

namespace code_clones_analysis
{
	template <utility::continuous_similarity_bounded_below SimilarityT>
	class funcs_pairwise_comparing_result final
	{
		std::unordered_map<pair_of_funcs_ids, funcs_similarity<SimilarityT>> similarity_by_ids_{};

	public:
		funcs_pairwise_comparing_result() = default;

		explicit funcs_pairwise_comparing_result(std::unordered_map<pair_of_funcs_ids, funcs_similarity<SimilarityT>> similarity_by_ids)
			: similarity_by_ids_(std::move(similarity_by_ids)) {}

		[[nodiscard]] funcs_similarity<SimilarityT> get_similarity(const code_analysis::func_id first_id, const code_analysis::func_id second_id) const
		{
			return similarity_by_ids_.at(pair_of_funcs_ids{ first_id, second_id });
		}
	};
}
