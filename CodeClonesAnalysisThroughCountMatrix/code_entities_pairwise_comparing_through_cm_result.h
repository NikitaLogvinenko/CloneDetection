#pragma once
#include "pair_of_ids.h"
#include "matrices_comparing_result.h"
#include "code_entities_comparing_traits.h"
#include "pair_of_ids_hash.h"
#include <unordered_map>

namespace code_clones_analysis_through_cm
{
	template <code_entities_comparing_traits CompTraits>
	class code_entities_pairwise_comparing_through_cm_result final
	{
	public:
		using code_analysis_traits = typename CompTraits::code_analysis_traits;
		using matrices_comparing_traits = typename CompTraits::matrices_comparing_traits;

		using analyzed_entity_id = typename code_analysis_traits::analyzed_entity_id;

		using pair_of_ids = code_clones_analysis::pair_of_ids<analyzed_entity_id>;
		using matrices_comparing_result = cm::matrices_comparing_result<matrices_comparing_traits>;
		using result_by_ids_map = std::unordered_map<pair_of_ids, matrices_comparing_result, code_clones_analysis::pair_of_ids_hash<analyzed_entity_id>>;

	private:
		result_by_ids_map result_by_ids_{};

	public:
		code_entities_pairwise_comparing_through_cm_result() = default;

		explicit code_entities_pairwise_comparing_through_cm_result(result_by_ids_map result_by_ids)
			: result_by_ids_(std::move(result_by_ids)) {}

		[[nodiscard]] matrices_comparing_result get_result(const analyzed_entity_id first_id, const analyzed_entity_id second_id) const
		{
			return result_by_ids_.at(pair_of_ids{ first_id, second_id });
		}
	};
}