#pragma once
#include "code_entities_pairwise_comparer_abstract.h"
#include "matrices_comparer_abstract.h"
#include "parameters_validation.h"
#include "code_entities_comparing_traits.h"
#include "code_implementation_info_builder_abstract.h"
#include "code_entity_implementation_info.h"
#include "code_entities_pairwise_comparing_through_cm_result.h"
#include "spinlock.h"
#include <ranges>
#include <mutex>
#include <future>

namespace code_clones_analysis_through_cm
{
	template <code_entities_comparing_traits CompTraits>
	class code_entities_pairwise_comparer_through_cm final :
		public code_clones_analysis::code_entities_pairwise_comparer_abstract<
		typename CompTraits::code_analysis_traits::analyzed_entity_id, typename CompTraits::matrices_comparing_traits::similarity_t>
	{
	public:
		using code_analysis_traits = typename CompTraits::code_analysis_traits;
		using matrices_comparing_traits = typename CompTraits::matrices_comparing_traits;

		using implementations_info_map = typename code_analysis_through_cm::code_implementation_info_builder_abstract<code_analysis_traits>::implementations_info_map;
		using implementations_info_ptr = std::shared_ptr<implementations_info_map>;

		using cm_comparing_ptr = std::unique_ptr<const cm::matrices_comparer_abstract<matrices_comparing_traits>>;

		using analyzed_entity_id = typename code_analysis_traits::analyzed_entity_id;
		using similarity_t = typename matrices_comparing_traits::similarity_t;

		using code_entities_pairwise_comparing_result = code_clones_analysis::code_entities_pairwise_comparing_result<analyzed_entity_id, similarity_t>;
		using detailed_results = code_entities_pairwise_comparing_through_cm_result<CompTraits>;

	private:
		using similarity_by_ids_map = typename code_entities_pairwise_comparing_result::similarity_by_ids_map;
		using result_by_ids_map = typename code_entities_pairwise_comparing_through_cm_result<CompTraits>::result_by_ids_map;
		using pair_of_ids = code_clones_analysis::pair_of_ids<analyzed_entity_id>;

		using code_entity_implementation_info = code_analysis_through_cm::code_entity_implementation_info<code_analysis_traits>;


		detailed_results detailed_results_{};
		implementations_info_ptr implementations_info_{};
		cm_comparing_ptr matrices_comparator_;
		size_t threads_count_;


		static constexpr size_t max_threads_count = 100;

	public:
		explicit code_entities_pairwise_comparer_through_cm(cm_comparing_ptr matrices_comparator, const size_t threads_count = 1)
		: matrices_comparator_(std::move(matrices_comparator)), threads_count_(threads_count)
		{
			const std::string method_name{ "code_entities_pairwise_comparer_through_cm::code_entities_pairwise_comparer_through_cm" };
			utility::throw_if_nullptr(matrices_comparator_.get(), method_name, "matrices_comparator");
			utility::throw_if_wrong_threads_count(threads_count_, max_threads(), method_name, "threads_count");
		}

		void set_implementations_info(std::shared_ptr<implementations_info_map> implementations_info)
		{
			const std::string method_name{ "code_entities_pairwise_comparer_through_cm::set_implementations_info" };
			utility::throw_if_nullptr(implementations_info.get(), method_name, "implementations_info");
			std::swap(implementations_info_, implementations_info);
		}

		[[nodiscard]] detailed_results&& extract_detailed_results()
		{
			return std::move(detailed_results_);
		}

		[[nodiscard]] code_entities_pairwise_comparing_result compare(
			const std::vector<analyzed_entity_id>& first_set_of_entities,
			const std::vector<analyzed_entity_id>& second_set_of_entities) override
		{
			const std::string method_name{ "code_entities_pairwise_comparer_through_cm::compare" };
			utility::throw_if_nullptr(implementations_info_.get(), method_name, "implementations_info");

			if (first_set_of_entities.empty() || second_set_of_entities.empty())
			{
				return code_entities_pairwise_comparing_result{};
			}

			if (threads_count_ == 1)
			{
				return compare_single_threaded(first_set_of_entities, second_set_of_entities);
			}

			return compare_multithreaded(first_set_of_entities, second_set_of_entities);
		}

		[[nodiscard]] static constexpr size_t max_threads() noexcept
		{
			return max_threads_count;
		}

	private:
		[[nodiscard]] code_entities_pairwise_comparing_result compare_single_threaded(
			const std::vector<analyzed_entity_id>& first_set_of_entities,
			const std::vector<analyzed_entity_id>& second_set_of_entities)
		{
			similarity_by_ids_map similarity_by_ids;
			result_by_ids_map matrices_comparing_results_by_ids;
			reserve_space(first_set_of_entities.size(), second_set_of_entities.size(), similarity_by_ids, matrices_comparing_results_by_ids);

			for (const analyzed_entity_id id_from_first_set : first_set_of_entities)
			{
				for (const analyzed_entity_id id_from_second_set : second_set_of_entities)
				{
					auto& first_entity_info = implementations_info_->at(id_from_first_set);
					auto& second_entity_info = implementations_info_->at(id_from_second_set);

					const pair_of_ids compared_entities_ids{ id_from_first_set, id_from_second_set };
					auto matrices_comparing_result = (*matrices_comparator_)(
						first_entity_info.nested_entities_conditions_cm(), second_entity_info.nested_entities_conditions_cm());

					similarity_by_ids.emplace(compared_entities_ids, matrices_comparing_result.similarity().to_similarity_t());
					matrices_comparing_results_by_ids.emplace(compared_entities_ids, std::move(matrices_comparing_result));
				}
			}

			detailed_results_ = detailed_results{ std::move(matrices_comparing_results_by_ids) };
			return code_entities_pairwise_comparing_result{ std::move(similarity_by_ids) };
		}

		[[nodiscard]] code_entities_pairwise_comparing_result compare_multithreaded(
			const std::vector<analyzed_entity_id>& first_set_of_entities,
			const std::vector<analyzed_entity_id>& second_set_of_entities)
		{
			similarity_by_ids_map similarity_by_ids;
			result_by_ids_map matrices_comparing_results_by_ids;
			reserve_space(first_set_of_entities.size(), second_set_of_entities.size(), similarity_by_ids, matrices_comparing_results_by_ids);
			utility::spinlock results_lock{};

			std::vector<std::future<void>> comparing_threads_futures{};
			comparing_threads_futures.reserve(threads_count_);

			std::size_t first_id_index{};
			std::size_t second_id_index{};
			utility::spinlock indices_lock{};

			for (size_t comparing_thread_index = 0; comparing_thread_index < threads_count_; ++comparing_thread_index)
			{
				auto comparing_thread_future = std::async(std::launch::async, 
					&code_entities_pairwise_comparer_through_cm::comparing_thread_method, this,
					std::ref(first_set_of_entities), std::ref(second_set_of_entities),
					std::ref(first_id_index), std::ref(second_id_index), 
					std::ref(similarity_by_ids), std::ref(matrices_comparing_results_by_ids),
					std::ref(indices_lock), std::ref(results_lock));

				comparing_threads_futures.emplace_back(std::move(comparing_thread_future));
			}

			for (size_t comparing_thread_index = 0; comparing_thread_index < threads_count_; ++comparing_thread_index)
			{
				comparing_threads_futures[comparing_thread_index].get();
			}

			detailed_results_ = detailed_results{ std::move(matrices_comparing_results_by_ids) };
			return code_entities_pairwise_comparing_result{ std::move(similarity_by_ids) };
		}

		void comparing_thread_method(
			const std::vector<analyzed_entity_id>& first_set_of_entities,
			const std::vector<analyzed_entity_id>& second_set_of_entities,
			size_t& first_entity_index, size_t& second_entity_index,
			similarity_by_ids_map& similarity_by_ids,
			result_by_ids_map& matrices_comparing_results_by_ids,
			utility::spinlock& indices_lock, utility::spinlock& results_lock)
		{
			if (first_set_of_entities.empty() || second_set_of_entities.empty())
			{
				return;
			}

			while(true)
			{
				const code_entity_implementation_info* first_entity_info, *second_entity_info;
				analyzed_entity_id first_entity_id_under_lock, second_entity_id_under_lock;

				{
					std::lock_guard indices_guard{ indices_lock };
					if (first_entity_index == first_set_of_entities.size())
					{
						break;
					}

					first_entity_id_under_lock = first_set_of_entities[first_entity_index];
					second_entity_id_under_lock = second_set_of_entities[second_entity_index];

					first_entity_info = &implementations_info_->at(first_entity_id_under_lock);
					second_entity_info = &implementations_info_->at(second_entity_id_under_lock);

					increase_indices(first_entity_index, second_entity_index, second_set_of_entities.size());
				}

				const pair_of_ids compared_entities_ids{ first_entity_id_under_lock, second_entity_id_under_lock };
				// ReSharper disable once CppTooWideScope
				auto matrices_comparing_result = (*matrices_comparator_)(
					first_entity_info->nested_entities_conditions_cm(), second_entity_info->nested_entities_conditions_cm());

				{
					std::lock_guard results_guard{ results_lock };
					similarity_by_ids.emplace(compared_entities_ids, matrices_comparing_result.similarity().to_similarity_t());
					matrices_comparing_results_by_ids.emplace(compared_entities_ids, std::move(matrices_comparing_result));
				}
			}
		}

		static void reserve_space(const size_t first_set_size, const size_t second_set_size, 
			similarity_by_ids_map& similarity_by_ids, result_by_ids_map& matrices_comparing_result_by_ids)
		{
			const size_t pairs_count = first_set_size * second_set_size;
			similarity_by_ids.reserve(pairs_count);
			matrices_comparing_result_by_ids.reserve(pairs_count);
		}

		static void increase_indices(size_t& first_index, size_t& second_index, const size_t max_second_index) noexcept
		{
			if (++second_index != max_second_index)
			{
				return;
			}

			second_index = 0;
			++first_index;
		}
	};
}
