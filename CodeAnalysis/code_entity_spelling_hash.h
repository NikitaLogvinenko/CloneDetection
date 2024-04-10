#pragma once
#include "code_entity_spelling.h"

namespace code_analysis
{
	class code_entity_spelling_hash final
	{
	public:
		[[nodiscard]] size_t operator()(const code_entity_spelling& spelling) const
		{
			return std::hash<std::string>{}(spelling.to_string());
		}
	};
}