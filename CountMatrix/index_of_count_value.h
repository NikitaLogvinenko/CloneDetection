#pragma once

namespace count_matrix
{
	class index_of_count_value final
	{
		size_t index_{};
	public:
		index_of_count_value() noexcept = default;
		explicit index_of_count_value(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}