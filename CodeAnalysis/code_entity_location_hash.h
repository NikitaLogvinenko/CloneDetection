#pragma once
#include "code_entity_location.h"
#include "symbol_position_hash.h"

namespace code_analysis
{
	class code_entity_location_hash final
	{
	public:
		[[nodiscard]] size_t operator()(const code_entity_location& location) const
		{
			const size_t first_symbol_position_hash = symbol_position_hash{}(location.first_symbol_position());
			return utility::hash_combine(first_symbol_position_hash, std::hash<std::string>{}(location.filename().string()));
		}
	};
}
