#pragma once
#include "var_origin.h"

namespace cpp_code_analysis
{
	class var_origin_and_usage_counter_pair final
	{
		clang_c_adaptation::var_origin origin_{};
		size_t used_n_times_{};

		static constexpr size_t default_used_n_times = 0;

	public:
		var_origin_and_usage_counter_pair() noexcept = default;
		explicit var_origin_and_usage_counter_pair(const clang_c_adaptation::var_origin origin,
			const size_t used_n_times = default_used_n_times) noexcept
			: origin_(origin), used_n_times_(used_n_times) {}

		[[nodiscard]] clang_c_adaptation::var_origin origin() const noexcept
		{
			return origin_;
		}

		[[nodiscard]] size_t& used_n_times() noexcept
		{
			return used_n_times_;
		}

		[[nodiscard]] size_t used_n_times() const noexcept
		{
			return used_n_times_;
		}
	};
}