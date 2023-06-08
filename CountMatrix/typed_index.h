#pragma once

namespace count_matrix
{
	class typed_index
	{
		size_t index_{};
	public:
		typed_index() noexcept = default;
		explicit typed_index(const size_t index) noexcept : index_(index) {}

		virtual ~typed_index();

		[[nodiscard]] size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}