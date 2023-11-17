#pragma once
#include "edge_weight.h"

namespace cm
{
	template <utility::non_const_arithmetic DistanceT>
	class distance_between_cv final
	{
		edge_weight<DistanceT> distance_{};

	public:
		constexpr distance_between_cv() noexcept = default;
		constexpr explicit distance_between_cv(const DistanceT value)
			noexcept(noexcept(edge_weight<DistanceT>(std::declval<DistanceT&>()))) : distance_(value) {}

		[[nodiscard]] constexpr const edge_weight<DistanceT>& to_edge_weight() const noexcept
		{
			return distance_;
		}
	};
}