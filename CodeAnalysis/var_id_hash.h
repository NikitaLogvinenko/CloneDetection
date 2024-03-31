#pragma once
#include "var_id.h"
#include <bitset>

namespace code_analysis
{
	class var_id_hash final
	{
	public:
		size_t operator()(const var_id& id) const
		{
			return std::hash<size_t>{}(id.to_size_t());
		}
	};
}