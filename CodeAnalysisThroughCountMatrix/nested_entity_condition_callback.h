#pragma once
#include "code_analysis_traits.h"
#include "condition_index.h"
#include "empty_argument_error.h"
#include <functional>

namespace code_analysis_through_cm
{
	template <code_analysis_traits AnalysisTraits>
	class nested_entity_condition_callback final
	{
	public:
		using analyzed_entity_id = typename AnalysisTraits::analyzed_entity_id;
		using nested_entity_id = typename AnalysisTraits::nested_entity_id;

		using callback_t = std::function<void(analyzed_entity_id, nested_entity_id, condition_index<AnalysisTraits::conditions_count>)>;

	private:
		callback_t callback_ =
			[](analyzed_entity_id, nested_entity_id, condition_index<AnalysisTraits::conditions_count>) {};

	public:
		nested_entity_condition_callback() = default;

		explicit nested_entity_condition_callback(callback_t callback) : callback_(std::move(callback))
		{
			if (!callback_)
			{
				throw common_exceptions::empty_argument_error{"nested_entity_condition_callback::nested_entity_condition_callback: callback is empty."};
			}
		}

		void operator()(const analyzed_entity_id analyzed_id, const nested_entity_id nested_id,
			const condition_index<AnalysisTraits::conditions_count> condition) const
		{
			callback_(analyzed_id, nested_id, condition);
		}
	};
}