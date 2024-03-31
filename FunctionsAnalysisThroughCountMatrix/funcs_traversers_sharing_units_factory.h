#pragma once
#include "funcs_traversers_factory_abstract.h"
#include "funcs_traverser_sharing_units.h"

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount, class TranslationUnitT>
	requires cm::count_vector_length<UsageConditionsCount>
	class funcs_traversers_sharing_units_factory final : public funcs_traversers_factory_abstract<UsageConditionsCount>
	{
		using translation_units_container = utility::one_off_container<TranslationUnitT>;

	public:
		using funcs_traverser_ptr = std::unique_ptr<funcs_traverser_sharing_units<UsageConditionsCount, TranslationUnitT>>;

	private:
		std::shared_ptr<translation_units_container> not_traversed_units_{};
		std::shared_ptr<utility::spinlock> units_container_lock_{ std::make_shared<utility::spinlock>() };

	public:
		funcs_traversers_sharing_units_factory() = default;

		explicit funcs_traversers_sharing_units_factory(std::vector<TranslationUnitT> units_for_traversing)
			: not_traversed_units_{ std::make_shared<translation_units_container>(std::move(units_for_traversing)) } {}

		[[nodiscard]] funcs_traverser_ptr generate() override
		{
			return funcs_traverser_ptr{ not_traversed_units_, units_container_lock_ };
		}
	};
}