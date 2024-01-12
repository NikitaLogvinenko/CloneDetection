#pragma once
#include "symbol_position.h"
#include <filesystem>

namespace functions_analysis
{
	class code_entity_location final
	{
		std::filesystem::path filename_{};
		symbol_position first_symbol_position_{};

	public:
		code_entity_location() noexcept = default;

		explicit code_entity_location(std::filesystem::path filename, const symbol_position& first_symbol_position) noexcept
			: filename_(std::move(filename)), first_symbol_position_(first_symbol_position) {}

		[[nodiscard]] const std::filesystem::path& filename() const noexcept
		{
			return filename_;
		}

		[[nodiscard]] const symbol_position& first_symbol_position() const noexcept
		{
			return first_symbol_position_;
		}

		[[nodiscard]] bool operator==(const code_entity_location&) const noexcept = default;
	};
}
