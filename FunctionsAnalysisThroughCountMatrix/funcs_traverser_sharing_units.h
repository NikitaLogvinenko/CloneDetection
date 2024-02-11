#pragma once
#include "funcs_traverser_abstract.h"
#include "disposable_container.h"
#include "parameters_validation.h"
#include <mutex>

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount, std::move_constructible TranslationUnitT>
	requires cm::count_vector_length<UsageConditionsCount>
	class funcs_traverser_sharing_units : public funcs_traverser_abstract<UsageConditionsCount>
	{
	public:
		using translation_units_container = utility::disposable_container<TranslationUnitT>;
	
		using var_usage_callback = typename funcs_traverser_abstract<UsageConditionsCount>::var_usage_callback;

	private:
		std::shared_ptr<translation_units_container> not_traversed_units_;
		std::shared_ptr<std::mutex> units_container_mutex_;

	protected:
		funcs_traverser_sharing_units(std::shared_ptr<translation_units_container> not_traversed_units, 
			std::shared_ptr<std::mutex> units_container_mutex) : not_traversed_units_(std::move(not_traversed_units)),
		units_container_mutex_(std::move(units_container_mutex))
		{
			const auto method_name = "funcs_traverser_sharing_units::funcs_traverser_sharing_units";
			utility::throw_if_nullptr(not_traversed_units_.get(), method_name, "not_traversed_units");
			utility::throw_if_nullptr(units_container_mutex_.get(), method_name, "units_container_mutex");
		}

		virtual void traverse_unit(std::unique_ptr<TranslationUnitT> translation_unit, 
			const var_usage_callback& callback) = 0;

	public:
		void traverse(const var_usage_callback& callback) override
		{
			while (true)
			{
				std::unique_ptr<TranslationUnitT> translation_unit{};

				{
					std::lock_guard units_container_guard{ *units_container_mutex_ };
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
