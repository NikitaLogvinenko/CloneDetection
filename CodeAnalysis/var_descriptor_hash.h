#pragma once
#include "var_descriptor.h"
#include "code_entity_descriptor_hash.h"

namespace code_analysis
{
	class var_descriptor_hash final
	{
	public:
		size_t operator()(const var_descriptor& var_descr) const
		{
			return code_entity_descriptor_hash{}(var_descr.to_code_entity_descriptor());
		}
	};
}
