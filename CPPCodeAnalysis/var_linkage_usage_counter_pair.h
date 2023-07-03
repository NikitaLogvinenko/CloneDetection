#pragma once
#include "var_linkage.h"

namespace cpp_code_analysis
{
	class var_linkage_usage_counter_pair final
	{
		clang_c_adaptation::var_linkage linkage_{};
		size_t used_n_times_{};

		inline static constexpr size_t default_used_n_times = 0;

	public:
		var_linkage_usage_counter_pair() noexcept = default;
		explicit var_linkage_usage_counter_pair(const clang_c_adaptation::var_linkage linkage, const size_t used_n_times = default_used_n_times) noexcept
			: linkage_(linkage), used_n_times_(used_n_times) {}

		[[nodiscard]] clang_c_adaptation::var_linkage linkage() const noexcept
		{
			return linkage_;
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