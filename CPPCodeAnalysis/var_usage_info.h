#pragma once
#include "var_spelling.h"
#include "var_location.h"
#include "count_vector.h"
#include "var_origin.h"

namespace cpp_code_analysis
{
	template <size_t VarUsageConditionsN>
	class var_usage_info final
	{
		clang_c_adaptation::var_spelling spelling_{};
		clang_c_adaptation::var_location location_{};
		clang_c_adaptation::var_origin origin_{ clang_c_adaptation::var_origin::unknown };
		cm::count_vector<VarUsageConditionsN> usage_count_vector_{};

	public:
		var_usage_info() noexcept = default;
		explicit var_usage_info(
			clang_c_adaptation::var_spelling spelling, clang_c_adaptation::var_location location,
		    const clang_c_adaptation::var_origin origin, 
			const std::array<cm::count_vector_value, VarUsageConditionsN>& usage_values_array) noexcept
			: spelling_(std::move(spelling)), location_(std::move(location)), origin_(origin), usage_count_vector_(usage_values_array) {}

		[[nodiscard]] const clang_c_adaptation::var_spelling& spelling() const noexcept
		{
			return spelling_;
		}

		[[nodiscard]] const clang_c_adaptation::var_location& location() const noexcept
		{
			return location_;
		}

		[[nodiscard]] clang_c_adaptation::var_origin origin() const noexcept
		{
			return origin_;
		}

		[[nodiscard]] const cm::count_vector<VarUsageConditionsN>& usage_count_vector() const noexcept
		{
			return  usage_count_vector_;
		}
	};
}