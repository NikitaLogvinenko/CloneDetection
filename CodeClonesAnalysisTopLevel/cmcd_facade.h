#pragma once
#include "cmcd_config_parser_abstract.h"
#include "cmcd_output_provider_abstract.h"
#include "funcs_analysis_through_count_matrix_factory_abstract.h"
#include "funcs_comparing_through_count_matrix_factory_abstract.h"
#include "cmcd_results_saver_factory_abstract.h"
#include "funcs_analysis_traits.h"
#include "timer.h"

namespace code_clones_analysis_top_level
{
	template <size_t ConditionsCount>
	class cmcd_facade final
	{
		using func_id = code_analysis::func_id;
		using implementations_info_map = std::unordered_map<func_id,
			code_analysis_through_cm::code_entity_implementation_info<code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>>,
			utility::id_hash<func_id>>;

	public:
		cmcd_facade() = delete;

		// ReSharper disable once CppPassValueParameterByConstReference
		static void exec(std::istream& input, std::unique_ptr<const cmcd_config_parser_abstract> parser, 
		                 std::unique_ptr<const funcs_analysis_through_count_matrix_factory_abstract<ConditionsCount>> analysis_factory,
		                 std::unique_ptr<const funcs_comparing_through_count_matrix_factory_abstract<ConditionsCount>> comparing_factory,
		                 std::unique_ptr<const cmcd_results_saver_factory_abstract<ConditionsCount>> saver_factory,
		                 // ReSharper disable once CppPassValueParameterByConstReference
		                 std::unique_ptr<cmcd_output_provider_abstract> output_provider)
		{
			utility::timer total_timer{};
			total_timer.start();

			const cmcd_config config = parser->parse(input);
			const auto analysis_director = analysis_factory->create_director(config);

			utility::timer stages_timer{};
			stages_timer.start();
			auto first_project_implementations_info = analyse_implementations(
				config, config.first_project_dir(), *analysis_director, * analysis_factory);
			stages_timer.stop();
			const auto first_project_analysis_time = stages_timer.interval();
			stages_timer.start();
			auto second_project_implementations_info = analyse_implementations(
				config, config.second_project_dir(), *analysis_director, *analysis_factory);
			stages_timer.stop();
			const auto second_project_analysis_time = stages_timer.interval();

			const auto comparer = comparing_factory->create_funcs_comparer(config);

			auto joined_implementations_info = first_project_implementations_info;
			joined_implementations_info.insert(second_project_implementations_info.cbegin(), second_project_implementations_info.cend());
			comparer->set_implementations_info(std::make_shared<decltype(joined_implementations_info)>(std::move(joined_implementations_info)));

			auto first_project_ids = get_analyzed_ids(first_project_implementations_info);
			auto second_project_ids = get_analyzed_ids(second_project_implementations_info);
			stages_timer.start();
			auto _ = comparer->compare(first_project_ids, second_project_ids);
			stages_timer.stop();
			const auto comparing_time = stages_timer.interval();

			auto comparing_results = comparer->extract_detailed_results();
			const cmcd_result<ConditionsCount> final_result{std::move(first_project_ids), std::move(second_project_ids),
			std::move(first_project_implementations_info), std::move(second_project_implementations_info), std::move(comparing_results)};

			const auto results_saver = saver_factory->create_saver(config);
			output_provider->init(config);
			const cmcd_results_saver_config saver_config{ config.min_similarity(), config.min_variables(),
				config.excluded_dirs(), config.excluded_sources() };
			stages_timer.start();
			results_saver->save(output_provider->output(), final_result, saver_config);
			stages_timer.stop();
			const auto results_saving_time = stages_timer.interval();

			total_timer.stop();
			const auto total_time = total_timer.interval();


			output_provider->output() << '\n';
			output_provider->output() << std::format("Total time: {} seconds\n", total_time);
			output_provider->output() << std::format("First project analysis time: {} seconds\n", first_project_analysis_time);
			output_provider->output() << std::format("Second project analysis time: {} seconds\n", second_project_analysis_time);
			output_provider->output() << std::format("Comparing time: {} seconds\n", comparing_time);
			output_provider->output() << std::format("Results saving time: {} seconds", results_saving_time);
		}

	private:
		[[nodiscard]] static implementations_info_map analyse_implementations(const cmcd_config& config, const std::filesystem::path& project_dir,
			const code_analysis_through_cm::code_implementations_info_director_abstract<code_analysis_through_cm::funcs_analysis_traits<ConditionsCount>>& analysis_director,
			const funcs_analysis_through_count_matrix_factory_abstract<ConditionsCount>& analysis_factory)
		{
			auto project_analysis_builder = analysis_factory.create_builder(config);
			auto project_traversers_factory = analysis_factory.create_traversers_factory(config, project_dir);

			auto project_implementations_info = analysis_director.analyse_implementations(
				std::move(project_analysis_builder), std::move(project_traversers_factory));
			
			return project_implementations_info;
		}

		[[nodiscard]] static std::vector<func_id> get_analyzed_ids(const implementations_info_map& info)
		{
			std::vector<func_id> ids{};
			ids.reserve(info.size());

			for (const auto& [id, _] : info)
			{
				ids.emplace_back(id);
			}

			return ids;
		}
	};
}