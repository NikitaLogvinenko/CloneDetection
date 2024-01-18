#pragma once
#include "edge.h"
#include <vector>
#include <algorithm>

namespace graphs
{
	template <utility::non_const_arithmetic WeightT>
	class bipartite_matching_result final
	{
		WeightT sum_of_matching_edges_{};
		std::vector<edge<WeightT>> matching_edges_{};

	public:
		constexpr bipartite_matching_result() noexcept = default;

		constexpr bipartite_matching_result(const WeightT sum_of_matching_edges, 
			std::vector<edge<WeightT>> matching_edges) noexcept
		: sum_of_matching_edges_(sum_of_matching_edges), matching_edges_(std::move(matching_edges)) {}

		[[nodiscard]] constexpr WeightT sum_of_matching_edges() const noexcept
		{
			return sum_of_matching_edges_;
		}

		[[nodiscard]] constexpr auto& matching_edges() const noexcept
		{
			return matching_edges_;
		}

		[[nodiscard]] constexpr auto&& matching_edges() && noexcept
		{
			return std::move(matching_edges_);
		}
	};
}
