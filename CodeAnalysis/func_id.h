#pragma once

namespace code_analysis
{
	class func_id final
	{
		size_t id_{};

	public:
		constexpr func_id() noexcept = default;

		constexpr explicit func_id(const size_t id) noexcept : id_(id) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return id_;
		}

		[[nodiscard]] constexpr bool operator==(const func_id&) const noexcept = default;
	};
}