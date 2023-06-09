#pragma once
#include <string>
#include <stdexcept>
#include <format>

namespace count_matrix_ns
{
	class relative_similarity final
	{
		double value_{};

		inline static constexpr double min_value { 0.0 };
		inline static constexpr double max_value { 1.0 };

		inline static const std::string invalid_similarity_msg{
			std::format("relative similarity value must be from {} (absolutely different vectors) to {} (equal vectors)", 
				std::to_string(min_value), std::to_string(max_value)) };

	public:
		relative_similarity() noexcept = default;
		explicit relative_similarity(const double value) : value_(value)
		{
			if (value_ < min_value || value_ > max_value)
			{
				throw std::domain_error(invalid_similarity_msg);
			}
		}

		[[nodiscard]] double to_double() const noexcept
		{
			return value_;
		}

		[[nodiscard]] auto operator<=>(const relative_similarity& other) const = default;


		constexpr [[nodiscard]] static double min_similarity_value() noexcept
		{
			return min_value;
		}

		constexpr [[nodiscard]] static double max_similarity_value() noexcept
		{
			return max_value;
		}
	};
}
