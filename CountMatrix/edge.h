#pragma once
#include "vertex_index.h"
#include "edge_weight.h"

namespace cm
{
	template <utility::non_const_arithmetic WeightT>
	class edge final
	{
		vertex_index first_index_{};
		vertex_index second_index_{};
		edge_weight<WeightT> weight_{};

	public:
		constexpr edge() noexcept = default;

		constexpr edge(const vertex_index first_index, const vertex_index second_index,
			const edge_weight<WeightT> weight) noexcept
			: first_index_(first_index), second_index_(second_index), weight_(weight) {}

		[[nodiscard]] constexpr const vertex_index& first_vertex_index() const noexcept
		{
			return first_index_;
		}

		[[nodiscard]] constexpr const vertex_index& second_vertex_index() const noexcept
		{
			return second_index_;
		}

		[[nodiscard]] constexpr const edge_weight<WeightT>& weight() const noexcept
		{
			return weight_;
		}
	};
}
