#pragma once
#include <string>

namespace code_clones_analysis_top_level
{
	class cmcd_results_saver_config final
	{
		double min_similarity_{};
		size_t min_variables_{};
		std::unordered_set<std::filesystem::path> excluded_dirs_{};
		std::unordered_set<std::filesystem::path> excluded_sources_{};

	public:
		cmcd_results_saver_config() noexcept = default;

		cmcd_results_saver_config(const double min_similarity, const size_t min_variables, 
			std::unordered_set<std::filesystem::path> excluded_dirs, std::unordered_set<std::filesystem::path> excluded_sources) noexcept
			: min_similarity_(min_similarity), min_variables_(min_variables), excluded_dirs_(std::move(excluded_dirs)),
		excluded_sources_(std::move(excluded_sources)) {}

		[[nodiscard]] double min_similarity() const noexcept
		{
			return min_similarity_;
		}

		[[nodiscard]] size_t min_variables() const noexcept
		{
			return min_variables_;
		}

		[[nodiscard]] bool exclude(const std::filesystem::path& path) const
		{
			const auto absolute_path = absolute(path);
			if (excluded_sources_.contains(absolute_path))
			{
				return true;
			}

			if (excluded_dirs_.empty())
			{
				return false;
			}

			auto possibly_excluded_directory = absolute_path;
			do
			{
				if (excluded_dirs_.contains(possibly_excluded_directory))
				{
					return true;
				}
				possibly_excluded_directory = possibly_excluded_directory.parent_path();
			} while (possibly_excluded_directory.has_relative_path());
		}
	};
}