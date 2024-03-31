#pragma once
#include "funcs_implementations_info_director_abstract.h"
#include "parameters_validation.h"
#include <mutex>
#include <future>

namespace funcs_analysis_through_cm
{
	template <size_t UsageConditionsCount> requires cm::count_vector_length<UsageConditionsCount>
	class funcs_implementations_info_director_multithreaded final
		: public funcs_implementations_info_director_abstract<UsageConditionsCount>
	{
		size_t traversing_threads_count_{ 1 };

		static constexpr size_t max_traversing_threads_count = 100;

	public:
		constexpr funcs_implementations_info_director_multithreaded() noexcept = default;

		explicit constexpr funcs_implementations_info_director_multithreaded(const size_t traversing_threads_count)
		: traversing_threads_count_(traversing_threads_count)
		{
			utility::throw_if_wrong_threads_count(traversing_threads_count, max_traversing_threads(), 
				"funcs_implementations_info_director_multithreaded::funcs_implementations_info_director_multithreaded",
				"traversing_threads_count");
		}

		[[nodiscard]] std::vector<func_implementation_info<UsageConditionsCount>> analyse_functions_implementations(
			std::unique_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>> builder,
			std::unique_ptr<funcs_traversers_factory_abstract<UsageConditionsCount>> functions_traversers_factory) const override
		{
			throw_if_nullptr(builder, functions_traversers_factory,
				"funcs_implementations_info_director_multithreaded::analyse_functions_implementations");

			if (traversing_threads_count_ == 1)
			{
				return single_threaded_analysis(std::move(builder), std::move(functions_traversers_factory));
			}

			return multithreaded_analysis(std::move(builder), std::move(functions_traversers_factory), traversing_threads_count_);
		}

		[[nodiscard]] static constexpr size_t max_traversing_threads() noexcept
		{
			return max_traversing_threads_count;
		}

	private:
		static void throw_if_nullptr(
			std::unique_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>>& builder,
			std::unique_ptr<funcs_traversers_factory_abstract<UsageConditionsCount>>& functions_traversers_factory,
			const std::string& method_name)
		{
			utility::throw_if_nullptr(builder.get(), method_name, "builder");
			utility::throw_if_nullptr(functions_traversers_factory.get(), method_name, "functions_traversers_factory");
		}

		[[nodiscard]] static std::vector<func_implementation_info<UsageConditionsCount>> traverse_and_return_funcs_info_single_threaded(
			std::unique_ptr<funcs_traverser_abstract<UsageConditionsCount>> traverser,
			std::unique_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>> builder)
		{
			var_usage_callback<UsageConditionsCount> callback{
				[&builder](
				const code_analysis::func_id& func, const code_analysis::var_id& var, condition_index<UsageConditionsCount> index)
				{
					builder->increment_condition(func, var, index);
				}
			};

			traverser->traverse(std::move(callback));
			
			return builder->build_and_reset();
		}

		static void traverse_and_add_conditions_multithreaded(
			std::unique_ptr<funcs_traverser_abstract<UsageConditionsCount>> traverser,
			std::shared_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>> builder,
			std::mutex& builder_mutex)
		{
			var_usage_callback<UsageConditionsCount> callback{
				[&builder, &builder_mutex](
				const code_analysis::func_id& func, const code_analysis::var_id& var, condition_index<UsageConditionsCount> index)
				{
					std::lock_guard builder_guard(builder_mutex);
					builder->increment_condition(func, var, index);
				}
			};

			traverser->traverse(std::move(callback));
		}

		[[nodiscard]] static std::vector<func_implementation_info<UsageConditionsCount>> single_threaded_analysis(
			std::unique_ptr<funcs_implementations_info_builder_abstract<UsageConditionsCount>> builder,
			std::unique_ptr<funcs_traversers_factory_abstract<UsageConditionsCount>> functions_traversers_factory)
		{
			auto traverser = functions_traversers_factory->generate();
			utility::throw_if_nullptr(traverser.get(),
				"funcs_implementations_info_director_multithreaded::single_threaded_analysis",
				"traverser");
			return traverse_and_return_funcs_info_single_threaded(std::move(traverser), std::move(builder));
		}

		[[nodiscard]] static std::vector<func_implementation_info<UsageConditionsCount>> multithreaded_analysis(
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
				utility::throw_if_nullptr(traverser.get(),
					"funcs_implementations_info_director_multithreaded::multithreaded_analysis",
					"traverser");

				auto traverser_future = std::async(std::launch::async, 
					&funcs_implementations_info_director_multithreaded::traverse_and_add_conditions_multithreaded,
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
