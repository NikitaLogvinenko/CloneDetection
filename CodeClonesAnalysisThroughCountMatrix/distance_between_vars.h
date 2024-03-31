#pragma once
#include "distance_between_count_vectors.h"

namespace code_clones_analysis_through_cm
{
	template <utility::non_const_arithmetic DistanceT>
	class distance_between_vars final
	{
		cm::distance_between_count_vectors<DistanceT> distance_{};

	public:
		constexpr distance_between_vars() noexcept = default;

		constexpr explicit distance_between_vars(cm::distance_between_count_vectors<DistanceT> distance) noexcept : distance_(distance) {}

		[[nodiscard]] constexpr cm::distance_between_count_vectors<DistanceT> to_cv_distance() const noexcept
		{
			return distance_;
		}
	};
}
