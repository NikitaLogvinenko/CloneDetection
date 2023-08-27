#pragma once
#include "initialized_value_out_of_range.h"
#include <string>
#include <format>

namespace cm
{
	class relative_similarity final
	{
		double value_{};

	public:
		relative_similarity() noexcept = default;

		explicit relative_similarity(const double value) : value_(value)
		{
			if (value_ < min_value || value_ > max_value)
			{
				const std::string invalid_similarity_msg{
					std::format(
						"relative similarity value must be from {} (absolutely different entities) to {} (equal entities)",
						std::to_string(min_value), std::to_string(max_value))
				};
				throw common_exceptions::initialized_value_out_of_range(invalid_similarity_msg);
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

	private:
		static constexpr double min_value{0.0};
		static constexpr double max_value{1.0};
	};
}
