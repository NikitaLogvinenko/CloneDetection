#pragma once
#include "var_descriptor.h"
#include "code_entity_descriptor_hash.h"

namespace code_analysis
{
	class var_descriptor_hash final
	{
	public:
		[[nodiscard]] size_t operator()(const var_descriptor& descriptor) const
		{
			const size_t code_entity_hash = code_entity_descriptor_hash{}(descriptor.to_code_entity_descriptor());
			const size_t id_hash = std::hash<size_t>{}(descriptor.id().to_size_t());
			return utility::hash_combine(code_entity_hash, id_hash);
		}
	};
}
