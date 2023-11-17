#pragma once

namespace cm
{
	class vertex_index final
	{
		size_t index_{};

	public:
		constexpr vertex_index() noexcept = default;

		constexpr explicit vertex_index(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}