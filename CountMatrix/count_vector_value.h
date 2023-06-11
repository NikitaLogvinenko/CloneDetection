#pragma once

namespace count_matrix
{
	class count_vector_value final
	{
		size_t value_{};

	public:
		count_vector_value() noexcept = default;
		explicit count_vector_value(const size_t value) noexcept : value_(value) {}

		[[nodiscard]] size_t value() const noexcept
		{
			return value_;
		}

		count_vector_value& operator++() noexcept
		{
			++value_;
			return *this;
		}
	};
}
