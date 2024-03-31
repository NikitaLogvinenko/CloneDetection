#pragma once

namespace code_clones_analysis_through_cm
{
	class func_index final
	{
		size_t index_{};

	public:
		constexpr func_index() noexcept = default;

		constexpr explicit func_index(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}

		[[nodiscard]] constexpr bool operator==(const func_index&) const noexcept = default;

		[[nodiscard]] constexpr auto operator<=>(const func_index&) const noexcept = default;
	};
}
