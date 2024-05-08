#pragma once
#include "code_implementation_info_builder_abstract.h"
#include "id_hash.h"
#include <unordered_map>

namespace code_analysis_through_cm
{
	template <code_analysis_traits AnalysisTraits>
	class code_implementation_info_builder_default final
		: public code_implementation_info_builder_abstract<AnalysisTraits>
	{
	public:
		using analyzed_entity_id = typename AnalysisTraits::analyzed_entity_id;
		using nested_entity_id = typename AnalysisTraits::nested_entity_id;
		using implementations_info_map = typename code_implementation_info_builder_abstract<AnalysisTraits>::implementations_info_map;

	private:
		using conditions_counters = std::vector<cm::counted_value>;

		using conditions_counters_by_nested_id_unordered_map = std::unordered_map<
			nested_entity_id, conditions_counters, utility::id_hash<nested_entity_id>>;

		using nested_info_by_analyzed_id_unordered_map = std::unordered_map<
			analyzed_entity_id, conditions_counters_by_nested_id_unordered_map, utility::id_hash<analyzed_entity_id>>;

		nested_info_by_analyzed_id_unordered_map nested_info_by_analyzed_id_{};

	public:
		void increment_condition(const analyzed_entity_id& analyzed_entity_id, const nested_entity_id& nested_entity_id,
			condition_index<AnalysisTraits::conditions_count> condition) override
		{
			if (nested_info_by_analyzed_id_.contains(analyzed_entity_id))
			{
				auto& conditions_counters_by_nested_id = nested_info_by_analyzed_id_[analyzed_entity_id];
				if (conditions_counters_by_nested_id.contains(nested_entity_id))
				{
					++conditions_counters_by_nested_id[nested_entity_id][condition.to_size_t()];
					return;
				}

				auto [conditions_by_nested_id_iterator, _] = conditions_counters_by_nested_id.emplace(
					nested_entity_id, conditions_counters(AnalysisTraits::conditions_count));
				auto& conditions_counters = conditions_by_nested_id_iterator->second;
				++conditions_counters[condition.to_size_t()];
				return;
			}

			conditions_counters_by_nested_id_unordered_map new_nested_id_with_zeroed_counters{ { nested_entity_id, conditions_counters(AnalysisTraits::conditions_count) } };
			auto [nested_info_by_analysed_id_iterator, _] = nested_info_by_analyzed_id_.emplace(analyzed_entity_id, std::move(new_nested_id_with_zeroed_counters));
			auto& conditions_counters_by_nested_id = nested_info_by_analysed_id_iterator->second;
			auto& conditions_counters = conditions_counters_by_nested_id.begin()->second;
			++conditions_counters[condition.to_size_t()];
		}

		[[nodiscard]] implementations_info_map build_and_reset() override
		{
			implementations_info_map entities_implementations_info{};
			entities_implementations_info.reserve(nested_info_by_analyzed_id_.size());

			for (auto& [analysed_id, conditions_counters_by_nested_id] : nested_info_by_analyzed_id_)
			{
				auto implementation_info = construct_info(std::move(conditions_counters_by_nested_id));
				entities_implementations_info.emplace(analysed_id, std::move(implementation_info));
			}

			nested_info_by_analyzed_id_.clear();
			return entities_implementations_info;
		}

	private:
		[[nodiscard]] code_entity_implementation_info<AnalysisTraits> construct_info(
			conditions_counters_by_nested_id_unordered_map conditions_counters_by_nested_id) const
		{
			std::vector<nested_entity_id> vector_of_nested_ids{};
			vector_of_nested_ids.reserve(conditions_counters_by_nested_id.size());

			std::vector<cm::count_vector<AnalysisTraits::conditions_count>> vector_of_count_vectors{};
			vector_of_count_vectors.reserve(conditions_counters_by_nested_id.size());

			while (!conditions_counters_by_nested_id.empty())
			{
				auto nested_id_and_counters_node = conditions_counters_by_nested_id.extract(conditions_counters_by_nested_id.cbegin());
				vector_of_nested_ids.emplace_back(nested_id_and_counters_node.key());
				vector_of_count_vectors.emplace_back(std::move(nested_id_and_counters_node.mapped()));
			}

			cm::count_matrix<AnalysisTraits::conditions_count> conditions_count_matrix(std::move(vector_of_count_vectors));

			return code_entity_implementation_info<AnalysisTraits>{
				std::move(vector_of_nested_ids), std::move(conditions_count_matrix) };
		}
	};
}
