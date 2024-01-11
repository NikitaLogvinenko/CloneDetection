#pragma once
#include "edge_weight.h"

namespace cm
{
	template <utility::non_const_arithmetic DistanceT>
	class distance_between_count_vectors final
	{
		edge_weight<DistanceT> distance_{};

	public:
		constexpr distance_between_count_vectors() noexcept = default;
		constexpr explicit distance_between_count_vectors(const edge_weight<DistanceT> distance) noexcept : distance_(distance) {}

		[[nodiscard]] constexpr edge_weight<DistanceT> to_edge_weight() const noexcept
		{
			return distance_;
		}
	};
}
