#pragma once
#include "code_entity_descriptor.h"

namespace code_analysis
{
	class func_descriptor final
	{
		code_entity_descriptor entity_descriptor_{};

	public:
		func_descriptor() noexcept = default;

		explicit func_descriptor(code_entity_descriptor entity_descriptor) noexcept
			: entity_descriptor_(std::move(entity_descriptor)) {}

		[[nodiscard]] const code_entity_descriptor& to_code_entity_descriptor() const noexcept
		{
			return entity_descriptor_;
		}
	};
}
