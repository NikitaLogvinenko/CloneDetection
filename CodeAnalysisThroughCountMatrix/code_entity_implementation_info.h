﻿#pragma once
#include "code_analysis_traits.h"
#include "count_matrix.h"

namespace code_analysis_through_cm
{
	template <code_analysis_traits AnalysisTraits>
	class code_entity_implementation_info final
	{
	public:
		using nested_entity_id = typename AnalysisTraits::nested_entity_id;

	private:
		std::vector<nested_entity_id> nested_entities_ids_{};
		cm::count_matrix<AnalysisTraits::conditions_count> nested_entities_conditions_cm_{};

	public:
		code_entity_implementation_info() noexcept = default;

		code_entity_implementation_info(std::vector<nested_entity_id> nested_entities_ids, 
			cm::count_matrix<AnalysisTraits::conditions_count> nested_entities_conditions_cm) :
			nested_entities_ids_(std::move(nested_entities_ids)),
			nested_entities_conditions_cm_(std::move(nested_entities_conditions_cm))
		{
			if (nested_entities_ids_.size() != nested_entities_conditions_cm_.vectors_count())
			{
				throw common_exceptions::incorrect_size_error("code_entity_implementation_info::code_entity_implementation_info:"
												  "nested_entities_ids and nested_entities_conditions_cm must have equal size.");
			}
		}

		[[nodiscard]] const std::vector<nested_entity_id>& nested_entities_ids() const noexcept
		{
			return nested_entities_ids_;
		}

		[[nodiscard]] const cm::count_matrix<AnalysisTraits::conditions_count>& nested_entities_conditions_cm() const noexcept
		{
			return nested_entities_conditions_cm_;
		}
	};
}
