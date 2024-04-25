#pragma once
#include "code_traverser_abstract.h"
#include "one_off_container.h"
#include "parameters_validation.h"
#include "spinlock.h"
#include <mutex>

namespace code_analysis_through_cm
{
	template <code_analysis_traits AnalysisTraits, std::move_constructible TranslationUnitT>
	class code_traverser_sharing_units_abstract : public code_traverser_abstract<AnalysisTraits>
	{
	public:
		using translation_units_container = utility::one_off_container<TranslationUnitT>;

	private:
		std::shared_ptr<translation_units_container> not_traversed_units_;
		std::shared_ptr<utility::spinlock> units_container_lock_;

	protected:
		code_traverser_sharing_units_abstract(std::shared_ptr<translation_units_container> not_traversed_units, 
			std::shared_ptr<utility::spinlock> units_container_lock) : not_traversed_units_(std::move(not_traversed_units)),
		units_container_lock_(std::move(units_container_lock))
		{
			const auto method_name = "code_traverser_sharing_units_abstract::code_traverser_sharing_units_abstract";
			utility::throw_if_nullptr(not_traversed_units_.get(), method_name, "not_traversed_units");
			throw_if_nullptr(units_container_lock_.get(), method_name, "units_container_lock");
		}

		virtual void traverse_unit(std::unique_ptr<TranslationUnitT> translation_unit, const nested_entity_condition_callback<AnalysisTraits>& callback) const = 0;

	public:
		void traverse(nested_entity_condition_callback<AnalysisTraits> callback) override
		{
			while (true)
			{
				std::unique_ptr<TranslationUnitT> translation_unit{};
				{
					std::lock_guard units_container_guard{ *units_container_lock_ };
					if (not_traversed_units_->empty())
					{
						break;
					}
					translation_unit = std::make_unique<TranslationUnitT>(not_traversed_units_->pop_front());
				}

				traverse_unit(std::move(translation_unit), callback);
			}
		}
	};
}
