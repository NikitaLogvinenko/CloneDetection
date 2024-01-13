#pragma once
#include <string>

namespace code_analysis
{
	class code_entity_spelling final
	{
		std::string spelling_{};

	public:
		constexpr code_entity_spelling() noexcept = default;

		constexpr explicit code_entity_spelling(std::string spelling) noexcept : spelling_(std::move(spelling)) {}

		[[nodiscard]] const std::string& to_string() const noexcept
		{
			return spelling_;
		}

		[[nodiscard]] constexpr bool operator==(const code_entity_spelling&) const noexcept = default;
	};
}
