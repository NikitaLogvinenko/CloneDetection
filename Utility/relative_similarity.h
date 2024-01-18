#pragma once
#include "initialized_value_out_of_range.h"
#include <string>
#include <format>

namespace utility
{
	class relative_similarity final
	{
		double value_{};

	public:
		constexpr relative_similarity() noexcept = default;

		constexpr explicit relative_similarity(const double value) : value_(value)
		{
			if (value_ < min_value || value_ > max_value)
			{
				const std::string invalid_similarity_msg{std::format(
					"relative_similarity: value must be from {} (absolutely different entities) to "
					"{} (equal entities), but {} was gotten",
					std::to_string(min_value), std::to_string(max_value), std::to_string(value_))};

				throw common_exceptions::initialized_value_out_of_range(invalid_similarity_msg);
			}
		}

		[[nodiscard]] constexpr double to_double() const noexcept
		{
			return value_;
		}

		[[nodiscard]] constexpr static double min_similarity_value() noexcept
		{
			return min_value;
		}

		[[nodiscard]] constexpr static double max_similarity_value() noexcept
		{
			return max_value;
		}

		[[nodiscard]] constexpr static relative_similarity min_similarity() noexcept
		{
			return relative_similarity(min_value);
		}

		[[nodiscard]] constexpr static relative_similarity max_similarity() noexcept
		{
			return relative_similarity(max_value);
		}

	private:
		static constexpr double min_value{0.0};
		static constexpr double max_value{1.0};
	};
}
