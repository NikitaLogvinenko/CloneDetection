#pragma once
#include "var_spelling.h"
#include "var_location.h"
#include "count_vector.h"
#include "var_linkage.h"

namespace cpp_code_analysis
{
	template <size_t VarUsageConditionsN>
	class var_usage_info final
	{
		clang_c_adaptation::var_spelling spelling_{};
		clang_c_adaptation::var_location location_{};
		clang_c_adaptation::var_linkage linkage_{ clang_c_adaptation::var_linkage::unknown };
		count_matrix_ns::count_vector<VarUsageConditionsN> usage_count_vector_{};

	public:
		var_usage_info() noexcept = default;
		explicit var_usage_info(
			clang_c_adaptation::var_spelling spelling, clang_c_adaptation::var_location location,
		    const clang_c_adaptation::var_linkage linkage, 
			const std::array<count_matrix_ns::count_vector_value, VarUsageConditionsN>& usage_values_array) noexcept
			: spelling_(std::move(spelling)), location_(std::move(location)), linkage_(linkage), usage_count_vector_(usage_values_array) {}

		[[nodiscard]] const clang_c_adaptation::var_spelling& spelling() const noexcept
		{
			return spelling_;
		}

		[[nodiscard]] const clang_c_adaptation::var_location& location() const noexcept
		{
			return location_;
		}

		[[nodiscard]] clang_c_adaptation::var_linkage linkage() const noexcept
		{
			return linkage_;
		}

		[[nodiscard]] const count_matrix_ns::count_vector<VarUsageConditionsN>& usage_count_vector() const noexcept
		{
			return  usage_count_vector_;
		}
	};
}