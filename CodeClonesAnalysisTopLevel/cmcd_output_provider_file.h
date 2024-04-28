#pragma once
#include "cmcd_output_provider_abstract.h"
#include "invalid_operation_error.h"
#include "parameters_validation.h"
#include <fstream>

namespace code_clones_analysis_top_level
{
	class cmcd_output_provider_file final : public cmcd_output_provider_abstract
	{
		std::ofstream output_{};

	public:
		void init(const cmcd_config& config) override
		{
			if (output_.is_open())
			{
				throw common_exceptions::invalid_operation_error{ "cmcd_output_provider_file::init: initialized already." };
			}

			const auto path = std::filesystem::absolute(config.results_output());
			output_.open(path, std::ios_base::out | std::ios_base::trunc);
			utility::throw_if_not_open(output_, std::filesystem::absolute(config.results_output()),
				"cmcd_output_provider_file::init");
		}

		[[nodiscard]] std::ostream& output() override
		{
			return output_;
		}
	};
}