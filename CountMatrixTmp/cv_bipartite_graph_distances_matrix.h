#pragma once
#include "bipartite_graph_weights_matrix.h"
#include "cv_abstract_metrics.h"
#include "count_matrix.h"

namespace cm
{
	template <utility::non_const_arithmetic DistanceT>
	class cv_bipartite_graph_distances_matrix final
	{
		bipartite_graph_weights_matrix<DistanceT> distances_matrix_{};

	public:
		constexpr cv_bipartite_graph_distances_matrix() = default;

		template <size_t CountVectorDimension>
		constexpr cv_bipartite_graph_distances_matrix(
			const count_matrix<CountVectorDimension>& first_matrix,
			const count_matrix<CountVectorDimension>& second_matrix,
			const cv_abstract_metrics<CountVectorDimension, DistanceT>& distance_func)
		: distances_matrix_(calculate_weights(first_matrix, second_matrix, distance_func)) {}
		

		[[nodiscard]] constexpr distance_between_cv<DistanceT> at(
			const index_of_count_vector index_from_first_cm,
			const index_of_count_vector index_from_second_cm) const
		{
			const size_t row = index_from_first_cm.to_size_t();
			const size_t column = index_from_second_cm.to_size_t();

			const vertex_index vertex_from_first_cm(row);
			const vertex_index vertex_from_second_cm(column);

			const edge_weight distance = distances_matrix_.at(vertex_from_first_cm, vertex_from_second_cm);

			return distance_between_cv(distance.value());
		}

		[[nodiscard]] constexpr const bipartite_graph_weights_matrix<DistanceT>& to_weights_matrix() const noexcept
		{
			return distances_matrix_;
		}

	private:
		template <size_t CountVectorDimension>
		[[nodiscard]] constexpr auto calculate_weights(
			const count_matrix<CountVectorDimension>& first_matrix,
			const count_matrix<CountVectorDimension>& second_matrix,
			const cv_abstract_metrics<CountVectorDimension, DistanceT>& distance_func)
		{
			if (first_matrix.vectors_count() == 0 || second_matrix.vectors_count() == 0)
			{
				return std::vector<std::vector<edge_weight<DistanceT>>>{};
			}

			std::vector<std::vector<edge_weight<DistanceT>>> weights_matrix{};
			weights_matrix.reserve(first_matrix.vectors_count());

			size_t row = 0;
			for (const auto& vector_from_first_cm : first_matrix)
			{
				weights_matrix.emplace_back();
				weights_matrix[row].reserve(second_matrix.vectors_count());

				for (const auto& vector_from_second_cm : second_matrix)
				{
					const distance_between_cv distance = distance_func.calculate(vector_from_first_cm, vector_from_second_cm);
					weights_matrix[row].emplace_back(distance.to_edge_weight());
				}

				++row;
			}

			return weights_matrix;
		}
	};
}