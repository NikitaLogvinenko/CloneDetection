#pragma once

namespace cpp_code_analysis
{
	class index_of_var_usage_info final
	{
		size_t index_{};
	public:
		index_of_var_usage_info() noexcept = default;
		explicit index_of_var_usage_info(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] size_t to_size_t() const noexcept
		{
			return index_;
		}
	};
}