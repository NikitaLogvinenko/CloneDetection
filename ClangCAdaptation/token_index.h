#pragma once

namespace clang_c_adaptation::internal
{
	class token_index final
	{
		size_t index_{};

	public:
		constexpr token_index() noexcept = default;

		constexpr explicit token_index(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] constexpr size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}
