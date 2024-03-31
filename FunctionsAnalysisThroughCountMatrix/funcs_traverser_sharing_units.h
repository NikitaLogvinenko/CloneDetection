#pragma once
#include "funcs_traverser_abstract.h"
#include "one_off_container.h"
#include "parameters_validation.h"
#include "spinlock.h"
#include <mutex>

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount, std::move_constructible TranslationUnitT>
	requires cm::count_vector_length<UsageConditionsCount>
	class funcs_traverser_sharing_units : public funcs_traverser_abstract<UsageConditionsCount>
	{
	public:
		using translation_units_container = utility::one_off_container<TranslationUnitT>;

	private:
		std::shared_ptr<translation_units_container> not_traversed_units_;
		std::shared_ptr<utility::spinlock> units_container_lock_;

	protected:
		funcs_traverser_sharing_units(std::shared_ptr<translation_units_container> not_traversed_units, 
			std::shared_ptr<utility::spinlock> units_container_lock) : not_traversed_units_(std::move(not_traversed_units)),
		units_container_lock_(std::move(units_container_lock))
		{
			const auto method_name = "funcs_traverser_sharing_units::funcs_traverser_sharing_units";
			utility::throw_if_nullptr(not_traversed_units_.get(), method_name, "not_traversed_units");
			throw_if_nullptr(units_container_lock_.get(), method_name, "units_container_lock");
		}

		virtual void traverse_unit(std::unique_ptr<TranslationUnitT> translation_unit, const var_usage_callback<UsageConditionsCount>& callback) = 0;

	public:
		void traverse(var_usage_callback<UsageConditionsCount> callback) override
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
