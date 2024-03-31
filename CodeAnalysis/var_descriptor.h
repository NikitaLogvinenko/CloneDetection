#pragma once
#include "var_id.h"
#include "unique_id_generator_threadsafe.h"
#include "code_entity_descriptor.h"

namespace code_analysis
{
	class var_descriptor final
	{
		var_id id_{ utility::unique_id_generator_threadsafe<var_id>::generate() };
		code_entity_descriptor entity_descriptor_{};

	public:
		var_descriptor() noexcept = default;

		explicit var_descriptor(code_entity_descriptor entity_descriptor) noexcept
			: entity_descriptor_(std::move(entity_descriptor)) {}

		[[nodiscard]] var_id id() const noexcept
		{
			return id_;
		}

		[[nodiscard]] const code_entity_descriptor& to_code_entity_descriptor() const noexcept
		{
			return entity_descriptor_;
		}
	};
}
