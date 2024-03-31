#pragma once
#include "pair_of_funcs_ids.h"
#include "hash_combine.h"

namespace code_clones_analysis
{
	class pair_of_funcs_ids_hash final
	{
	public:
		size_t operator()(const pair_of_funcs_ids& ids) const
		{
			const size_t first_id_hash = std::hash<size_t>{}(ids.first_id().to_size_t());
			return utility::hash_combine(first_id_hash, ids.second_id().to_size_t());
		}
	};
}
