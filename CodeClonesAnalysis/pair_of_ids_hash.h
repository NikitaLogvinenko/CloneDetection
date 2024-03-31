#pragma once
#include "pair_of_ids.h"
#include "id_hash.h"
#include "hash_combine.h"

namespace code_clones_analysis
{
	template <utility::id_concept IdT>
	class pair_of_ids_hash final
	{
	public:
		[[nodiscard]] size_t operator()(const pair_of_ids<IdT>& ids) const
		{
			const utility::id_hash<IdT> hasher{};
			const size_t first_id_hash = hasher(ids.first_id());
			const size_t second_id_hash = hasher(ids.second_id());
			return utility::hash_combine(first_id_hash, second_id_hash);
		}
	};
}
