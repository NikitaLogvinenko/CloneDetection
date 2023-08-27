#pragma once

namespace cpp_code_analysis::internal
{
	class analysed_func_info_index final
	{
		size_t index_{};

	public:
		analysed_func_info_index() noexcept = default;

		explicit analysed_func_info_index(const size_t index) noexcept : index_(index) {}

		[[nodiscard]] size_t to_size_t() const noexcept
		{
			return index_;
		}

		[[nodiscard]] auto operator<=>(const analysed_func_info_index& other) const noexcept = default;
	};
}
