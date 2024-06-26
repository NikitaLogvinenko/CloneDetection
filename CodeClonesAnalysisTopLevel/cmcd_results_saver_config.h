﻿#pragma once
#include <string>

namespace code_clones_analysis_top_level
{
	class cmcd_results_saver_config final
	{
		double min_similarity_{};
		size_t min_variables_{};
		std::unordered_set<std::filesystem::path> excluded_dirs_{};
		std::unordered_set<std::filesystem::path> excluded_sources_{};
		mutable std::unordered_map<std::filesystem::path, bool> already_seen_paths_{};

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
			if (already_seen_paths_.contains(path))
			{
				return already_seen_paths_.at(path);
			}

			if (excluded_sources_.contains(absolute_path))
			{
				return set_and_return(path, true);
			}

			if (excluded_dirs_.contains(absolute_path))
			{
				return set_and_return(path, true);
			}

			for (const auto& excluded_dir : excluded_dirs_)
			{
				const auto relative_path = relative(absolute_path, excluded_dir);
				if (relative_path.string() == "." || !relative_path.empty() && relative_path.native().at(0) != '.')
				{
					return set_and_return(path, true);
				}
			}

			return set_and_return(path, false);
		}

	private:
		bool set_and_return(const std::filesystem::path& path, const bool exclude) const
		{
			already_seen_paths_[path] = exclude;
			return exclude;
		}
	};
}