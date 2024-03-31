#pragma once
#include "symbol_position.h"
#include "hash_combine.h"

namespace code_analysis
{
	class symbol_position_hash final
	{
	public:
		[[nodiscard]] size_t operator()(const symbol_position& position) const
		{
			constexpr std::hash<size_t> hasher{};
			const size_t line_hash = hasher(position.line());
			const size_t column_hash = hasher(position.column());
			const size_t offset_hash = hasher(position.offset_from_file_start());

			size_t hash = utility::hash_combine(line_hash, column_hash);
			hash = utility::hash_combine(hash, offset_hash);
			return hash;
		}
	};
}
