#pragma once
#include "func_descriptor.h"
#include "code_entity_descriptor_hash.h"

namespace code_analysis
{
	class func_descriptor_hash final
	{
	public:
		[[nodiscard]] size_t operator()(const func_descriptor& descriptor) const
		{
			return code_entity_descriptor_hash{}(descriptor.to_code_entity_descriptor());
		}
	};
}
