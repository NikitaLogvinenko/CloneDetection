#pragma once

namespace code_clones_analysis_through_cm
{
	class var_index final
	{
		size_t index_{};

	public:
		constexpr var_index() noexcept = default;

		constexpr explicit var_index(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}
