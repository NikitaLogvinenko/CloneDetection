#pragma once

namespace code_analysis
{
	class var_id final
	{
		size_t id_{};

	public:
		constexpr var_id() noexcept = default;

		constexpr explicit var_id(const size_t id) noexcept : id_(id) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return id_;
		}

		[[nodiscard]] constexpr bool operator==(const var_id&) const noexcept = default;
	};
}