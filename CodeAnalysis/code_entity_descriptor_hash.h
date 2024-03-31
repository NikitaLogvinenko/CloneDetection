#pragma once
#include "code_entity_descriptor.h"
#include "code_entity_spelling_hash.h"
#include "code_entity_location_hash.h"

namespace code_analysis
{
	class code_entity_descriptor_hash final
	{
	public:
		size_t operator()(const code_entity_descriptor& descriptor) const
		{
			auto& spelling = descriptor.spelling();
			auto& location = descriptor.location();

			const size_t spelling_hash = code_entity_spelling_hash{}(spelling);
			const size_t location_hash = code_entity_location_hash{}(location);

			return utility::hash_combine(spelling_hash, location_hash);
		}
	};
}
