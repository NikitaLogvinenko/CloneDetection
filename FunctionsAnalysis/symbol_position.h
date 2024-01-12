#pragma once

namespace functions_analysis
{
	class symbol_position final
	{
		size_t line_{};
		size_t column_{};
		size_t offset_from_file_start_{};

	public:
		constexpr symbol_position() noexcept = default;

		constexpr explicit symbol_position(const size_t line, const size_t column, 
			const size_t offset_from_file_start) noexcept
		: line_(line), column_(column), offset_from_file_start_(offset_from_file_start) {}

		[[nodiscard]] constexpr size_t line() const noexcept
		{
			return line_;
		}

		[[nodiscard]] constexpr size_t column() const noexcept
		{
			return column_;
		}

		[[nodiscard]] constexpr size_t offset_from_file_start() const noexcept
		{
			return offset_from_file_start_;
		}

		[[nodiscard]] constexpr bool operator==(const symbol_position&) const noexcept = default;
	};
}
