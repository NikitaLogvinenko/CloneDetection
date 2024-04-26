#pragma once
#include "code_traversers_factory_abstract.h"
#include "code_traverser_sharing_units_abstract.h"
#include "traverser_sharing_units_concept.h"

namespace code_analysis_through_cm
{
	template <code_analysis_traits AnalysisTraits, class TranslationUnitT, class TraverserT>
	requires traverser_sharing_units_concept<TraverserT, AnalysisTraits, TranslationUnitT>
	class code_traversers_sharing_units_factory final : public code_traversers_factory_abstract<AnalysisTraits>
	{
		using translation_units_container = utility::one_off_container<TranslationUnitT>;
		
		std::shared_ptr<translation_units_container> not_traversed_units_{};
		std::shared_ptr<utility::spinlock> units_container_lock_{ std::make_shared<utility::spinlock>() };

		std::function<void(TraverserT&)> traverser_setting_{};

	public:
		code_traversers_sharing_units_factory() = default;

		explicit code_traversers_sharing_units_factory(std::vector<TranslationUnitT> units_for_traversing, 
			std::function<void(TraverserT&)> traverser_setting = {})
			: not_traversed_units_{ std::make_shared<translation_units_container>(std::move(units_for_traversing)) },
		traverser_setting_(std::move(traverser_setting)) {}

		[[nodiscard]] std::unique_ptr<code_traverser_abstract<AnalysisTraits>> generate() override
		{
			auto ptr = std::make_unique<TraverserT>(not_traversed_units_, units_container_lock_);

			if (!traverser_setting_)
			{
				return ptr;
			}

			traverser_setting_(*ptr);

			return ptr;
		}
	};
}