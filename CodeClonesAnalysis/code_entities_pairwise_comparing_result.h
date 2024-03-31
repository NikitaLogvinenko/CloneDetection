#pragma once
#include "pair_of_ids.h"
#include "continuous_similarity_bounded_below.h"
#include "pair_of_ids_hash.h"
#include <unordered_map>

namespace code_clones_analysis
{
	template <utility::id_concept IdT, utility::continuous_similarity_bounded_below SimilarityT>
	class code_entities_pairwise_comparing_result final
	{
	public:
		using similarity_by_ids_map = std::unordered_map<pair_of_ids<IdT>, SimilarityT, pair_of_ids_hash<IdT>>;

	private:
		similarity_by_ids_map similarity_by_ids_{};

	public:
		code_entities_pairwise_comparing_result() = default;

		explicit code_entities_pairwise_comparing_result(similarity_by_ids_map similarity_by_ids)
			: similarity_by_ids_(std::move(similarity_by_ids)) {}

		[[nodiscard]] SimilarityT get_similarity(const IdT first_id, const IdT second_id) const
		{
			return similarity_by_ids_.at(pair_of_ids{ first_id, second_id });
		}
	};
}
