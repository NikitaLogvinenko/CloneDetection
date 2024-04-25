#pragma once
#include <string>
#include <filesystem>
#include <unordered_map>

namespace code_clones_analysis_top_level
{
	class count_matrix_clones_detection_config
	{
		std::string results_output_{};

		std::string framework_{};
		std::filesystem::path first_project_dir_{};
		std::filesystem::path second_project_dir_{};

		double min_similarity_{};
		size_t min_variables_{};

		size_t code_analysis_threads_ = 1;
		size_t count_matrices_comparing_threads_ = 1;

		std::string matrices_comparing_algorithm_{};

		std::unordered_map<std::string, std::string> additional_params_{};

	public:
		count_matrix_clones_detection_config() = default;

		void set_results_output(std::string output) noexcept
		{
			std::swap(results_output_, output);
		}

		void set_framework(std::string name) noexcept
		{
			std::swap(framework_, name);
		}

		void set_first_project_dir(std::filesystem::path path) noexcept
		{
			std::swap(first_project_dir_, path);
		}

		void set_second_project_dir(std::filesystem::path path) noexcept
		{
			std::swap(second_project_dir_, path);
		}

		void set_min_similarity(const double value) noexcept
		{
			min_similarity_ = value;
		}

		void set_min_variables(const size_t value) noexcept
		{
			min_variables_ = value;
		}

		void set_code_analysis_threads(const size_t value) noexcept
		{
			code_analysis_threads_ = value;
		}

		void set_count_matrices_comparing_threads(const size_t value) noexcept
		{
			count_matrices_comparing_threads_ = value;
		}

		void set_matrices_comparing_algorithm(std::string algorithm) noexcept
		{
			std::swap(matrices_comparing_algorithm_, algorithm);
		}

		void add_param(const std::string& name, const std::string& param) noexcept
		{
			additional_params_[name] = param;
		}

		[[nodiscard]] const std::string& results_output() const noexcept
		{
			return results_output_;
		}

		[[nodiscard]] const std::string& framework() const noexcept
		{
			return framework_;
		}

		[[nodiscard]] const std::filesystem::path& first_project_dir() const noexcept
		{
			return first_project_dir_;
		}

		[[nodiscard]] const std::filesystem::path& second_project_dir() const noexcept
		{
			return second_project_dir_;
		}

		[[nodiscard]] double min_similarity() const noexcept
		{
			return min_similarity_;
		}

		[[nodiscard]] size_t min_variables() const noexcept
		{
			return min_variables_;
		}

		[[nodiscard]] size_t code_analysis_threads() const noexcept
		{
			return code_analysis_threads_;
		}

		[[nodiscard]] size_t count_matrices_comparing_threads() const noexcept
		{
			return count_matrices_comparing_threads_;
		}

		[[nodiscard]] const std::string& matrices_comparing_algorithm() const noexcept
		{
			return matrices_comparing_algorithm_;
		}

		[[nodiscard]] const std::unordered_map<std::string, std::string>& additional_params() const noexcept
		{
			return additional_params_;
		}
	};
}
