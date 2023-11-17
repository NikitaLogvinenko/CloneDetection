#pragma once

namespace cm
{
	class matched_variables_index final
	{
		size_t index_{};

	public:
		constexpr matched_variables_index() noexcept = default;

		constexpr explicit matched_variables_index(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}