#pragma once
#include "func_descriptor.h"
#include "code_entity_descriptor_hash.h"

namespace code_analysis
{
	class func_descriptor_hash final
	{
	public:
		size_t operator()(const func_descriptor& func_descr) const
		{
			return code_entity_descriptor_hash{}(func_descr.to_code_entity_descriptor());
		}
	};
}
