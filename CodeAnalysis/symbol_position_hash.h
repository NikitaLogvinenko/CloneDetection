#pragma once
#include "symbol_position.h"
#include "hash_combine.h"

namespace code_analysis
{
	class symbol_position_hash final
	{
	public:
		size_t operator()(const symbol_position& position) const
		{
			size_t hash = std::hash<size_t>{}(position.line());
			hash = utility::hash_combine(hash, position.column());
			hash = utility::hash_combine(hash, position.offset_from_file_start());

			return hash;
		}
	};
}
