#pragma once
#include "bipartite_graph_weights_matrix.h"
#include "count_vectors_metrics_abstract.h"
#include "count_matrix.h"

namespace cm
{
	template <utility::non_const_arithmetic DistanceT>
	class bipartite_graph_on_count_matrices final
	{
		bipartite_graph_weights_matrix<DistanceT> distances_matrix_{};

	public:
		constexpr bipartite_graph_on_count_matrices() = default;

		template <size_t CountVectorLength>
		constexpr bipartite_graph_on_count_matrices(
			const count_matrix<CountVectorLength>& first_matrix,
			const count_matrix<CountVectorLength>& second_matrix,
			const count_vectors_metrics_abstract<CountVectorLength, DistanceT>& metrics)
		: distances_matrix_(calculate_weights(first_matrix, second_matrix, metrics)) {}
		

		[[nodiscard]] constexpr distance_between_count_vectors<DistanceT> at(
			const index_of_count_vector index_from_first_count_matrix,
			const index_of_count_vector index_from_second_count_matrix) const
		{
			const size_t row = index_from_first_count_matrix.to_size_t();
			const size_t column = index_from_second_count_matrix.to_size_t();

			const vertex_index vertex_from_first_part(row);
			const vertex_index vertex_from_second_part(column);

			const edge_weight distance = distances_matrix_.at(vertex_from_first_part, vertex_from_second_part);

			return distance_between_count_vectors<DistanceT>(distance.to_size_t());
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
			const count_vectors_metrics_abstract<CountVectorDimension, DistanceT>& distance_func)
		{
			if (first_matrix.vectors_count() == 0 || second_matrix.vectors_count() == 0)
			{
				return std::vector<std::vector<edge_weight<DistanceT>>>{};
			}

			std::vector<std::vector<edge_weight<DistanceT>>> weights_matrix{};
			weights_matrix.reserve(first_matrix.vectors_count());

			size_t row = 0;
			for (const auto& vector_from_first_matrix : first_matrix)
			{
				weights_matrix.emplace_back();
				weights_matrix[row].reserve(second_matrix.vectors_count());

				for (const auto& vector_from_second_matrix : second_matrix)
				{
					const distance_between_count_vectors distance = distance_func.calculate(vector_from_first_matrix, vector_from_second_matrix);
					weights_matrix[row].emplace_back(distance.to_edge_weight());
				}

				++row;
			}

			return weights_matrix;
		}
	};
}
