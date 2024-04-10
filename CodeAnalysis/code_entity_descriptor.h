#pragma once
#include "code_entity_spelling.h"
#include "code_entity_location.h"

namespace code_analysis
{
	class code_entity_descriptor final
	{
		code_entity_spelling spelling_{};
		code_entity_location location_{};

	public:
		code_entity_descriptor() noexcept = default;

		explicit code_entity_descriptor(code_entity_spelling spelling, code_entity_location location) noexcept
			: spelling_(std::move(spelling)), location_(std::move(location)) {}

		[[nodiscard]] const code_entity_spelling& spelling() const noexcept
		{
			return spelling_;
		}

		[[nodiscard]] const code_entity_location& location() const noexcept
		{
			return location_;
		}
	};
}
