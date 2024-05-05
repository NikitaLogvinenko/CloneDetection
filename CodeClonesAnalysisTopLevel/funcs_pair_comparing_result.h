#pragma once
#include "func_descriptor.h"
#include "code_entity_implementation_info.h"
#include "funcs_pairwise_comparing_traits.h"
#include "matrices_comparing_result.h"

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class funcs_pair_comparing_result final
	{
	public:
		using func_descriptor = code_analysis::func_descriptor;
		using traits = funcs_pairwise_comparing_traits<ConditionsCount>;
		using func_implementation_info = code_analysis_through_cm::code_entity_implementation_info<typename traits::code_analysis_traits>;
		using matrices_comparing_result = cm::matrices_comparing_result<typename traits::matrices_comparing_traits>;

	private:
		func_descriptor first_func_descriptor_{};
		func_descriptor second_func_descriptor_{};
		func_implementation_info first_func_implementation_{};
		func_implementation_info second_func_implementation_{};
		matrices_comparing_result comparing_result_{};

	public:
		funcs_pair_comparing_result() noexcept = default;

		funcs_pair_comparing_result(func_descriptor first_func_descriptor, func_descriptor second_func_descriptor,
			func_implementation_info first_func_implementation, func_implementation_info second_func_implementation,
			matrices_comparing_result comparing_result) noexcept
		: first_func_descriptor_(std::move(first_func_descriptor)), second_func_descriptor_(std::move(second_func_descriptor)),
		first_func_implementation_(std::move(first_func_implementation)), second_func_implementation_(std::move(second_func_implementation)),
		comparing_result_(std::move(comparing_result)) {}

		[[nodiscard]] const func_descriptor& first_func_descriptor() const  noexcept
		{
			return first_func_descriptor_;
		}

		[[nodiscard]] const func_descriptor& second_func_descriptor() const  noexcept
		{
			return second_func_descriptor_;
		}

		[[nodiscard]] const func_implementation_info& first_func_implementation() const noexcept
		{
			return first_func_implementation_;
		}

		[[nodiscard]] const func_implementation_info& second_func_implementation() const noexcept
		{
			return second_func_implementation_;
		}

		[[nodiscard]] const matrices_comparing_result& comparing_result() const noexcept
		{
			return comparing_result_;
		}
	};
}