#pragma once
#include "func_spelling.h"
#include "func_location.h"
#include "var_usage_info.h"
#include "count_matrix.h"
#include <vector>

namespace cpp_code_analysis
{
	class index_of_var_usage_info final : public count_matrix::typed_index
	{
	public:
		index_of_var_usage_info() noexcept = default;
		explicit index_of_var_usage_info(const size_t index) noexcept : typed_index(index) {}
	};

	template <size_t VarUsageConditionsN>
	class func_implementation_info final
	{
		clang_c_adaptation::func_spelling spelling_{};
		clang_c_adaptation::func_location location_{};
		std::vector<var_usage_info<VarUsageConditionsN>> variables_usage_info_{};
		count_matrix::count_matrix<VarUsageConditionsN> variables_usage_count_matrix_{};

	public:
		func_implementation_info() noexcept = default;
		explicit func_implementation_info(
			clang_c_adaptation::func_spelling spelling, clang_c_adaptation::func_location location,
			std::vector<var_usage_info<VarUsageConditionsN>> variables_usage_info);

		[[nodiscard]] const clang_c_adaptation::func_spelling& spelling() const noexcept
		{
			return spelling_;
		}

		[[nodiscard]] const clang_c_adaptation::func_location& location() const noexcept
		{
			return location_;
		}

		[[nodiscard]] const count_matrix::count_matrix<VarUsageConditionsN>& variables_usage_count_matrix() const noexcept
		{
			return variables_usage_count_matrix_;
		}

		[[nodiscard]] size_t variables_count() const noexcept
		{
			return variables_usage_info_.size();
		}

		[[nodiscard]] const var_usage_info<VarUsageConditionsN>& operator[](const index_of_var_usage_info variable_index) const noexcept
		{
			return variables_usage_info_[variable_index];
		}

		[[nodiscard]] const var_usage_info<VarUsageConditionsN>& at(const index_of_var_usage_info variable_index) const
		{
			return variables_usage_info_.at(variable_index); 
		}
	};

	template <size_t VarUsageConditionsN>
	func_implementation_info<VarUsageConditionsN>::func_implementation_info(
		clang_c_adaptation::func_spelling spelling,	clang_c_adaptation::func_location location,
		std::vector<var_usage_info<VarUsageConditionsN>> variables_usage_info)
			: spelling_(std::move(spelling)), location_(std::move(location)), variables_usage_info_(std::move(variables_usage_info))
	{
		std::vector<count_matrix::count_vector<VarUsageConditionsN>> variables_count_vectors{};
		variables_count_vectors.reserve(variables_usage_info_.size());
		for (const auto& var_info : variables_usage_info_)
		{
			variables_count_vectors.emplace_back(var_info.usage_count_vector());
		}
		variables_usage_count_matrix_ = count_matrix::count_matrix<VarUsageConditionsN>(std::move(variables_count_vectors));
	}
}
