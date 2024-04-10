#pragma once
#include "func_id.h"
#include <bitset>

namespace code_analysis
{
	class func_id_hash final
	{
	public:
		[[nodiscard]] size_t operator()(const func_id& id) const
		{
			return std::hash<size_t>{}(id.to_size_t());
		}
	};
}