#pragma once
#include "code_entity_implementation_info.h"
#include "code_entities_pairwise_comparing_through_cm_result.h"
#include "funcs_pairwise_comparing_traits.h"

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class cmcd_result final
	{
	public:
		using traits = funcs_pairwise_comparing_traits<ConditionsCount>;
		using analyzed_entity_id = typename traits::code_analysis_traits::analyzed_entity_id;
		using nested_entity_id = typename traits::code_analysis_traits::nested_entity_id;

		using implementations_info_map = std::unordered_map<
			analyzed_entity_id, code_analysis_through_cm::code_entity_implementation_info<typename traits::code_analysis_traits>, utility::id_hash<analyzed_entity_id>>;

		using funcs_pairwise_comparing_result = code_clones_analysis_through_cm::code_entities_pairwise_comparing_through_cm_result<traits>;

	private:
		std::vector<analyzed_entity_id> first_set_of_entities_{};
		std::vector<analyzed_entity_id> second_set_of_entities_{};

		implementations_info_map first_project_implementations_info_{};
		implementations_info_map second_project_implementations_info_{};
		funcs_pairwise_comparing_result comparing_result_{};

	public:
		cmcd_result() = default;

		cmcd_result(std::vector<analyzed_entity_id> first_set_of_entities, std::vector<analyzed_entity_id> second_set_of_entities,
			implementations_info_map first_project_implementations_info, implementations_info_map second_project_implementations_info,
			funcs_pairwise_comparing_result comparing_result)
			: first_set_of_entities_(std::move(first_set_of_entities)), second_set_of_entities_(std::move(second_set_of_entities)),
			first_project_implementations_info_(std::move(first_project_implementations_info)),
			second_project_implementations_info_(std::move(second_project_implementations_info)),
			comparing_result_(std::move(comparing_result)) {}

		[[nodiscard]] const std::vector<analyzed_entity_id>& first_set_of_entities() const noexcept
		{
			return first_set_of_entities_;
		}

		[[nodiscard]] const std::vector<analyzed_entity_id>& second_set_of_entities() const noexcept
		{
			return second_set_of_entities_;
		}

		[[nodiscard]] const implementations_info_map& first_project_implementations_info() const noexcept
		{
			return first_project_implementations_info_;
		}

		[[nodiscard]] const implementations_info_map& second_project_implementations_info() const noexcept
		{
			return second_project_implementations_info_;
		}

		[[nodiscard]] const funcs_pairwise_comparing_result& comparing_result() const noexcept
		{
			return comparing_result_;
		}
	};
}