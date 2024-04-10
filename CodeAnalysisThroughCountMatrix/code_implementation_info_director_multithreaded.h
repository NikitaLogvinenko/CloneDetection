#pragma once
#include "code_implementations_info_director_abstract.h"
#include "parameters_validation.h"
#include <mutex>
#include <future>

namespace code_analysis_through_cm
{
	template <code_analysis_traits AnalysisTraits>
	class code_implementation_info_director_multithreaded final
		: public code_implementations_info_director_abstract<AnalysisTraits>
	{
	public:
		using implementations_info_map = typename code_implementations_info_director_abstract<AnalysisTraits>::implementations_info_map;

	private:

		using analyzed_entity_id = typename AnalysisTraits::analyzed_entity_id;
		using nested_entity_id = typename AnalysisTraits::nested_entity_id;

		size_t traversing_threads_count_{ 1 };

		static constexpr size_t max_traversing_threads_count = 100;

	public:
		constexpr code_implementation_info_director_multithreaded() noexcept = default;

		explicit constexpr code_implementation_info_director_multithreaded(const size_t traversing_threads_count)
		: traversing_threads_count_(traversing_threads_count)
		{
			utility::throw_if_wrong_threads_count(traversing_threads_count, max_traversing_threads(), 
				"code_implementation_info_director_multithreaded::code_implementation_info_director_multithreaded",
				"traversing_threads_count");
		}

		[[nodiscard]] implementations_info_map analyse_implementations(
			std::unique_ptr<code_implementation_info_builder_abstract<AnalysisTraits>> builder,
			std::unique_ptr<code_traversers_factory_abstract<AnalysisTraits>> traversers_factory) const override
		{
			throw_if_nullptr(builder, traversers_factory,
				"code_implementation_info_director_multithreaded::analyse_implementations");

			if (traversing_threads_count_ == 1)
			{
				return single_threaded_analysis(std::move(builder), std::move(traversers_factory));
			}

			return multithreaded_analysis(std::move(builder), std::move(traversers_factory), traversing_threads_count_);
		}

		[[nodiscard]] static constexpr size_t max_traversing_threads() noexcept
		{
			return max_traversing_threads_count;
		}

	private:
		static void throw_if_nullptr(
			std::unique_ptr<code_implementation_info_builder_abstract<AnalysisTraits>>& builder,
			std::unique_ptr<code_traversers_factory_abstract<AnalysisTraits>>& traversers_factory,
			const std::string& method_name)
		{
			utility::throw_if_nullptr(builder.get(), method_name, "builder");
			utility::throw_if_nullptr(traversers_factory.get(), method_name, "traversers_factory");
		}

		[[nodiscard]] static implementations_info_map traverse_and_return_funcs_info_single_threaded(
			std::unique_ptr<code_traverser_abstract<AnalysisTraits>> traverser,
			std::unique_ptr<code_implementation_info_builder_abstract<AnalysisTraits>> builder)
		{
			nested_entity_condition_callback<AnalysisTraits> callback{
				[&builder](analyzed_entity_id analyzed_id, nested_entity_id nested_id, condition_index<AnalysisTraits::conditions_count> condition)
				{
					builder->increment_condition(analyzed_id, nested_id, condition);
				}
			};

			traverser->traverse(std::move(callback));
			
			return builder->build_and_reset();
		}

		static void traverse_and_add_conditions_multithreaded(
			std::unique_ptr<code_traverser_abstract<AnalysisTraits>> traverser,
			std::shared_ptr<code_implementation_info_builder_abstract<AnalysisTraits>> builder,
			std::mutex& builder_mutex)
		{
			nested_entity_condition_callback<AnalysisTraits> callback{
				[&builder, &builder_mutex](
					analyzed_entity_id analyzed_id, nested_entity_id nested_id, condition_index<AnalysisTraits::conditions_count> condition)
				{
					std::lock_guard builder_guard(builder_mutex);
					builder->increment_condition(analyzed_id, nested_id, condition);
				}
			};

			traverser->traverse(std::move(callback));
		}

		[[nodiscard]] static implementations_info_map single_threaded_analysis(
			std::unique_ptr<code_implementation_info_builder_abstract<AnalysisTraits>> builder,
			std::unique_ptr<code_traversers_factory_abstract<AnalysisTraits>> traversers_factory)
		{
			auto traverser = traversers_factory->generate();
			utility::throw_if_nullptr(traverser.get(),
				"code_implementation_info_director_multithreaded::single_threaded_analysis",
				"traverser");
			return traverse_and_return_funcs_info_single_threaded(std::move(traverser), std::move(builder));
		}

		[[nodiscard]] static implementations_info_map multithreaded_analysis(
			std::unique_ptr<code_implementation_info_builder_abstract<AnalysisTraits>> builder,
			std::unique_ptr<code_traversers_factory_abstract<AnalysisTraits>> traversers_factory,
			const size_t traversing_threads_count)
		{
			std::shared_ptr<code_implementation_info_builder_abstract<AnalysisTraits>> shared_builder(std::move(builder));

			std::mutex builder_mutex{};
			std::vector<std::future<void>> traversers_futures{};
			traversers_futures.reserve(traversing_threads_count);

			for (size_t traverser_index = 0; traverser_index < traversing_threads_count; ++traverser_index)
			{
				auto traverser = traversers_factory->generate();
				utility::throw_if_nullptr(traverser.get(),
					"code_implementation_info_director_multithreaded::multithreaded_analysis",
					"traverser");

				auto traverser_future = std::async(std::launch::async, 
					&code_implementation_info_director_multithreaded::traverse_and_add_conditions_multithreaded,
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
