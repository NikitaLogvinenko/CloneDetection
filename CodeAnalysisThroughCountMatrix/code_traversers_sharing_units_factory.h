#pragma once
#include "code_traversers_factory_abstract.h"
#include "code_traverser_sharing_units.h"

namespace code_analysis_through_cm
{
	template <code_analysis_traits AnalysisTraits, class TranslationUnitT>
	class code_traversers_sharing_units_factory final : public code_traversers_factory_abstract<AnalysisTraits>
	{
		using translation_units_container = utility::one_off_container<TranslationUnitT>;

	public:
		using funcs_traverser_ptr = std::unique_ptr<code_traverser_sharing_units<AnalysisTraits, TranslationUnitT>>;

	private:
		std::shared_ptr<translation_units_container> not_traversed_units_{};
		std::shared_ptr<utility::spinlock> units_container_lock_{ std::make_shared<utility::spinlock>() };

	public:
		code_traversers_sharing_units_factory() = default;

		explicit code_traversers_sharing_units_factory(std::vector<TranslationUnitT> units_for_traversing)
			: not_traversed_units_{ std::make_shared<translation_units_container>(std::move(units_for_traversing)) } {}

		[[nodiscard]] funcs_traverser_ptr generate() override
		{
			return funcs_traverser_ptr{ not_traversed_units_, units_container_lock_ };
		}
	};
}