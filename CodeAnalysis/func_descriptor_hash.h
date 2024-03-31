#pragma once
#include "func_descriptor.h"
#include "code_entity_descriptor_hash.h"

namespace code_analysis
{
	class func_descriptor_hash final
	{
	public:
		size_t operator()(const func_descriptor& descriptor) const
		{
			const size_t code_entity_hash = code_entity_descriptor_hash{}(descriptor.to_code_entity_descriptor());
			return utility::hash_combine(code_entity_hash, descriptor.id().to_size_t());
		}
	};
}
