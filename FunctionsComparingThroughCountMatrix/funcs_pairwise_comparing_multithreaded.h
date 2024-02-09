#pragma once
#include "funcs_pairwise_comparing_abstract.h"
#include "matrices_comparing_abstract.h"
#include "parameters_validation.h"
#include <mutex>
#include <future>

namespace funcs_comparing_through_cm
{
	template <funcs_comparing_traits CompTraits>
	class funcs_pairwise_comparing_multithreaded final : public funcs_pairwise_comparing_abstract<CompTraits>
	{
		using func_implementation_info = funcs_analysis_through_cm::func_implementation_info<CompTraits::count_vector_length>;

	public:
		using cm_comparing_ptr = std::unique_ptr<const cm::matrices_comparing_abstract<CompTraits>>;

	private:
		cm_comparing_ptr matrices_comparator_;
		size_t threads_count_;

		static constexpr size_t max_threads_count = 100;

	public:
		explicit funcs_pairwise_comparing_multithreaded(cm_comparing_ptr matrices_comparator, const size_t threads_count = 1)
		: matrices_comparator_(std::move(matrices_comparator)), threads_count_(threads_count)
		{
			const std::string method_name{ "funcs_pairwise_comparing_multithreaded::funcs_pairwise_comparing_multithreaded" };
			utility::throw_if_nullptr(matrices_comparator_.get(), method_name, "matrices_comparator");
			utility::throw_if_wrong_threads_count(threads_count_, max_threads(), method_name, "threads_count");
		}

		[[nodiscard]] funcs_pairwise_comparing_result<CompTraits> compare(
			const std::vector<func_implementation_info>& first_set_of_functions,
			const std::vector<func_implementation_info>& second_set_of_functions) const override
		{
			if (first_set_of_functions.empty() || second_set_of_functions.empty())
			{
				return funcs_pairwise_comparing_result<CompTraits>{};
			}

			if (threads_count_ == 1)
			{
				return compare_single_threaded(first_set_of_functions, second_set_of_functions);
			}

			return compare_multithreaded(first_set_of_functions, second_set_of_functions);
		}

		[[nodiscard]] static constexpr size_t max_threads() noexcept
		{
			return max_threads_count;
		}

	private:
		[[nodiscard]] funcs_pairwise_comparing_result<CompTraits> compare_single_threaded(
			const std::vector<func_implementation_info>& first_set_of_functions,
			const std::vector<func_implementation_info>& second_set_of_functions) const
		{
			std::map<pair_of_funcs_indices, funcs_comparing_result<CompTraits>> comparing_results_by_funcs_indices;

			for (size_t first_func_index = 0; first_func_index < first_set_of_functions.size(); ++first_func_index)
			{
				for (size_t second_func_index = 0; second_func_index < second_set_of_functions.size(); ++second_func_index)
				{
					auto& first_func_info = first_set_of_functions[first_func_index];
					auto& second_func_info = second_set_of_functions[second_func_index];

					const pair_of_funcs_indices funcs_indices{ func_index{first_func_index}, func_index{second_func_index} };
					auto matrices_comparing_result = (*matrices_comparator_)(
						first_func_info.vars_usage_count_matrix(), second_func_info.vars_usage_count_matrix());

					comparing_results_by_funcs_indices.emplace(funcs_indices, std::move(matrices_comparing_result));
				}
			}

			return funcs_pairwise_comparing_result<CompTraits>{ std::move(comparing_results_by_funcs_indices) };
		}

		[[nodiscard]] funcs_pairwise_comparing_result<CompTraits> compare_multithreaded(
			const std::vector<func_implementation_info>& first_set_of_functions,
			const std::vector<func_implementation_info>& second_set_of_functions) const
		{
			std::size_t first_func_index{};
			std::size_t second_func_index{};
			std::mutex indices_mutex{};

			std::map<pair_of_funcs_indices, funcs_comparing_result<CompTraits>> comparing_results_by_funcs_indices;
			std::mutex results_mutex{};

			std::vector<std::future<void>> comparing_threads_futures{};
			comparing_threads_futures.reserve(threads_count_);

			for (size_t comparing_thread_index = 0; comparing_thread_index < threads_count_; ++comparing_thread_index)
			{
				auto comparing_thread_future = std::async(std::launch::async, 
					&funcs_pairwise_comparing_multithreaded::comparing_thread_function, this,
					std::ref(first_set_of_functions), std::ref(second_set_of_functions), 
					std::ref(first_func_index), std::ref(second_func_index), std::ref(comparing_results_by_funcs_indices),
					std::ref(indices_mutex), std::ref(results_mutex));

				comparing_threads_futures.emplace_back(std::move(comparing_thread_future));
			}

			for (size_t comparing_thread_index = 0; comparing_thread_index < threads_count_; ++comparing_thread_index)
			{
				comparing_threads_futures[comparing_thread_index].get();
			}

			return funcs_pairwise_comparing_result<CompTraits>{ std::move(comparing_results_by_funcs_indices) };
		}

		void comparing_thread_function(
			const std::vector<func_implementation_info>& first_set_of_functions,
			const std::vector<func_implementation_info>& second_set_of_functions,
			size_t& first_func_index, size_t& second_func_index,
			std::map<pair_of_funcs_indices, funcs_comparing_result<CompTraits>>& comparing_results_by_funcs_indices,
			std::mutex& indices_mutex, std::mutex& results_mutex)
		{
			if (first_set_of_functions.empty() || second_set_of_functions.empty())
			{
				return;
			}

			while(true)
			{
				const func_implementation_info* first_func_info, second_func_info;
				size_t first_func_index_under_lock, second_func_index_under_lock;

				{
					std::lock_guard indices_guard{ indices_mutex };
					if (first_func_index == first_set_of_functions.size())
					{
						break;
					}

					first_func_index_under_lock = first_func_index;
					second_func_index_under_lock = second_func_index;
					first_func_info = &first_set_of_functions[first_func_index_under_lock];
					second_func_info = &second_set_of_functions[second_func_index_under_lock];

					increase_indices(first_func_index, second_func_index, second_set_of_functions.size());
				}

				const pair_of_funcs_indices funcs_indices{ func_index{first_func_index_under_lock},
					func_index{second_func_index_under_lock} };

				auto matrices_comparing_result = (*matrices_comparator_)(
					first_func_info->vars_usage_count_matrix(), second_func_info->vars_usage_count_matrix());

				{
					std::lock_guard results_guard{ results_mutex };
					comparing_results_by_funcs_indices.emplace(funcs_indices, std::move(matrices_comparing_result));
				}
			}
		}

		static void increase_indices(size_t& first_func_index, size_t& second_func_index, 
			const size_t second_funcs_count) noexcept
		{
			if (++second_func_index != second_funcs_count)
			{
				return;
			}

			second_func_index = 0;
			++first_func_index;
		}
	};
}
