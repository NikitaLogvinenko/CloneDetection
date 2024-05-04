#pragma once
#include "vertex.h"
#include <array>
#include <algorithm>
#include <numeric>

namespace graph_ns
{
	template<size_t Dim>
	class point_Rn_space final
	{

		std::array<double, Dim> coordinates_{};

	public:
		explicit point_Rn_space(std::array<double, Dim> coordinates) noexcept;

		[[nodiscard]] const std::array<double, Dim>& coordinates() const noexcept;
		[[nodiscard]] size_t dim() const noexcept;

		[[nodiscard]] bool operator==(const point_Rn_space& other) const noexcept = default;
		[[nodiscard]] bool approximately_equal(const point_Rn_space& other, double negligible_difference) const noexcept;
	};

	template <size_t N>
	point_Rn_space<N>::point_Rn_space(std::array<double, N> coordinates) noexcept : coordinates_(std::move(coordinates)) { }

	template <size_t N>
	const std::array<double, N>& point_Rn_space<N>::coordinates() const noexcept
	{
		return coordinates_;
	}

	template <size_t N>
	size_t point_Rn_space<N>::dim() const noexcept
	{
		return coordinates_.size();
	}

	template <size_t Dim>
	bool point_Rn_space<Dim>::approximately_equal(const point_Rn_space& other,
		const double negligible_difference) const noexcept
	{
		for (auto this_it = coordinates_.begin(), other_it = other.coordinates_.begin(); this_it != coordinates_.end(); ++this_it, ++other_it)
		{
			if (std::abs(*this_it - *other_it) > negligible_difference)
			{
				return false;
			}
		}
		return true;
	}

	template<typename Tid, size_t Dim>
	using vertex_Rn_space = vertex<Tid, point_Rn_space<Dim>>;

	template<size_t Dim>
	[[nodiscard]] double calc_Rn_distance(const point_Rn_space<Dim>& point_first, const point_Rn_space<Dim>& point_second)
	{
		const auto& coords_first = point_first.coordinates();
		const auto& coords_second = point_second.coordinates();
		std::array<double, Dim> coords_difference{};

		std::transform(coords_first.cbegin(), coords_first.cend(), coords_second.cbegin(),coords_difference.begin(), 
			[](const double& coord_first, const double& coord_second) { return pow(coord_first - coord_second, 2); });
		const double sum_of_squared_differences = std::accumulate(coords_difference.cbegin(),
			coords_difference.cend(), 0.0);
		return sqrt(sum_of_squared_differences);
	}
}
