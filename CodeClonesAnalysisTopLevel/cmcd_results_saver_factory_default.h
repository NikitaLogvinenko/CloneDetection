#pragma once
#include "cmcd_results_saver_factory_abstract.h"
#include "input_format_error.h"
#include "func_descriptor_creator_clang.h"
#include "var_descriptor_creator_clang.h"
#include "clones_filter_default.h"
#include "funcs_pair_comparing_result_saver_default.h"
#include "cmcd_results_saver_default.h"

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class cmcd_results_saver_factory_default final : public cmcd_results_saver_factory_abstract<ConditionsCount>
	{
	public:
		[[nodiscard]] std::unique_ptr<cmcd_results_saver_abstract<ConditionsCount>> create_saver(
			const cmcd_config& config) const override
		{
			if (config.framework() != "clang")
			{
				throw common_exceptions::input_format_error{ "cmcd_results_saver_factory_default::create_saver: "
												"only clang framework supported now." };
			}

			auto func_descriptor_creator = std::make_unique<clang_code_analysis::func_descriptor_creator_clang>();
			auto var_descriptor_creator = std::make_unique<clang_code_analysis::var_descriptor_creator_clang>();
			auto clones_filter = std::make_unique<clones_filter_default<ConditionsCount>>();
			auto funcs_pair_saver = std::make_unique<funcs_pair_comparing_result_saver_default<ConditionsCount>>();

			return std::make_unique<cmcd_results_saver_default>(std::move(func_descriptor_creator), std::move(var_descriptor_creator),
				std::move(clones_filter), std::move(funcs_pair_saver));
		}
	};
}