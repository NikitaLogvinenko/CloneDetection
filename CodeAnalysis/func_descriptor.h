#pragma once
#include "func_id.h"
#include "unique_id_generator_threadsafe.h"
#include "code_entity_descriptor.h"

namespace code_analysis
{
	class func_descriptor final
	{
		func_id id_{ utility::unique_id_generator_threadsafe<func_id>::generate() };
		code_entity_descriptor entity_descriptor_{};

	public:
		func_descriptor() noexcept = default;

		explicit func_descriptor(code_entity_descriptor entity_descriptor) noexcept
			: entity_descriptor_(std::move(entity_descriptor)) {}

		[[nodiscard]] func_id id() const noexcept
		{
			return id_;
		}

		[[nodiscard]] const code_entity_descriptor& to_code_entity_descriptor() const noexcept
		{
			return entity_descriptor_;
		}
	};
}
