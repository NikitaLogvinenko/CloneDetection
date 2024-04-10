#pragma once
#include "id_concept.h"

namespace utility
{
	template <id_concept IdT>
	class id_hash final
	{
	public:
		[[nodiscard]] size_t operator()(const IdT& id) const
		{
			return std::hash<size_t>{}(id.to_size_t());
		}
	};
}
