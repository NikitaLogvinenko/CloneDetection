#pragma once
#include "funcs_implementations_info_director_abstract.h"
#include "parameters_validation.h"
#include <mutex>
#include <future>

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_implementations_info_director_multithreaded final : public funcs_implementations_info_director_abstract<UsageConditionsCount>
	{
		using this_class = funcs_implementations_info_director_multithreaded;

		size_t traversing_threads_count_{ 1 };

		static constexpr size_t max_traversing_threads_count = 1000;

	public:
		constexpr funcs_implementations_info_director_multithreaded() noexcept = default;

		explicit constexpr funcs_implementations_info_director_multithreaded(const size_t traversing_threads_count)
			: funcs_implementations_info_director_abstract<UsageConditionsCount>(), traversing_threads_count_(traversing_threads_count)
		{
			utility::throw_if_wrong_threads_count(traversing_threads_count, max_traversing_threads(), 
				"funcs_implementations_info_director_multithreaded::funcs_implementations_info_director_multithreaded",
				"traversing_threads_count");
		}

		[[nodiscard]] std::vector<func_implementation_info<UsageConditionsCount>> analyse_functions_implementations(
			std::unique_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>> builder,
			std::unique_ptr<funcs_traversers_factory_abstract<UsageConditionsCount>> functions_traversers_factory) override
		{
			this_class::throw_if_nullptr(builder, functions_traversers_factory,
				"funcs_implementations_info_director_multithreaded::analyse_functions_implementations");

			if (traversing_threads_count_ == 1)
			{
				return this_class::one_threaded_analysis(
					std::move(builder), std::move(functions_traversers_factory));
			}

			return this_class::multi_threaded_analysis(std::move(builder), 
				std::move(functions_traversers_factory), traversing_threads_count_);
		}

		[[nodiscard]] static constexpr size_t max_traversing_threads() noexcept
		{
			return max_traversing_threads_count;
		}

	private:

		static void throw_if_nullptr(
			std::unique_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>> builder,
			std::unique_ptr<funcs_traversers_factory_abstract<UsageConditionsCount>> functions_traversers_factory,
			const std::string& method_name)
		{
			utility::throw_if_nullptr(builder, method_name, "builder");
			utility::throw_if_nullptr(functions_traversers_factory, method_name, "functions_traversers_factory");
		}

		[[nodiscard]] static std::vector<var_usage_condition_descriptor<UsageConditionsCount>>
			throw_if_nullptr_traverse_otherwise(std::unique_ptr<funcs_traverser_abstract<UsageConditionsCount>> traverser)
		{
			utility::throw_if_nullptr(traverser,
				"funcs_implementations_info_director_multithreaded::throw_if_nullptr_traverse_otherwise",
				"traverser");
			return traverser->traverse();
		}

		static void add_conditions(
			std::vector<var_usage_condition_descriptor<UsageConditionsCount>> vars_usage_conditions,
			std::shared_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>> builder)
		{
			for (auto& usage_condition : vars_usage_conditions)
			{
				builder->add_condition(std::move(usage_condition));
			}
		}

		static void traverse_and_add_conditions(
			std::unique_ptr<funcs_traverser_abstract<UsageConditionsCount>> traverser,
			std::shared_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>> builder)
		{
			auto vars_usage_conditions = this_class::throw_if_nullptr_traverse_otherwise(std::move(traverser));

			this_class::add_conditions(std::move(vars_usage_conditions, builder));
		}

		static void traverse_and_add_conditions_threadsafe(
			std::unique_ptr<funcs_traverser_abstract<UsageConditionsCount>> traverser,
			std::shared_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>> builder,
			std::mutex& builder_mutex)
		{
			auto vars_usage_conditions = this_class::throw_if_nullptr_traverse_otherwise(std::move(traverser));

			std::lock_guard builder_guard(builder_mutex);
			this_class::add_conditions(std::move(vars_usage_conditions, builder));
		}

		[[nodiscard]] static std::vector<func_implementation_info<UsageConditionsCount>> one_threaded_analysis(
			std::unique_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>> builder,
			std::unique_ptr<funcs_traversers_factory_abstract<UsageConditionsCount>> functions_traversers_factory)
		{
			std::shared_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>> shared_builder(std::move(builder));

			this_class::traverse_and_add_conditions(functions_traversers_factory->generate(), shared_builder);
			return shared_builder->build_and_reset();
		}

		[[nodiscard]] static std::vector<func_implementation_info<UsageConditionsCount>> multi_threaded_analysis(
			std::unique_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>> builder,
			std::unique_ptr<funcs_traversers_factory_abstract<UsageConditionsCount>> functions_traversers_factory,
			const size_t traversing_threads_count)
		{
			std::shared_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>> shared_builder(std::move(builder));

			std::mutex builder_mutex{};
			std::vector<std::future<void>> traversers_futures{};
			traversers_futures.reserve(traversing_threads_count);

			for (size_t traverser_index = 0; traverser_index < traversing_threads_count; ++traverser_index)
			{
				auto traverser = functions_traversers_factory->generate();
				auto traverser_future = std::async(std::launch::async, &this_class::traverse_and_add_conditions_threadsafe, 
					std::move(traverser), shared_builder, std::ref(builder_mutex));
				traversers_futures.emplace_back(std::move(traverser_future));
			}

			for (size_t traverser_index = 0; traverser_index < traversing_threads_count; ++traverser_index)
			{
				traversers_futures[traverser_index].get();
			}

			return shared_builder->build_and_reset();
		}
	};
}
