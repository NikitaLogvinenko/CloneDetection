#pragma once
#include "edge.h"
#include "incorrect_vector_length.h"
#include <vector>

namespace cm
{
	template <utility::non_const_arithmetic WeightT>
	class bipartite_graph_weights_matrix final
	{
		std::vector<std::vector<edge_weight<WeightT>>> weights_matrix_{};

	public:
		constexpr bipartite_graph_weights_matrix() noexcept = default;

		constexpr explicit bipartite_graph_weights_matrix(
			std::vector<std::vector<edge_weight<WeightT>>> weights_matrix)
		: weights_matrix_(std::move(weights_matrix))
		{
			bipartite_graph_weights_matrix::validate_rows_lengths(weights_matrix_);			
		}

		[[nodiscard]] constexpr edge_weight<WeightT> at(
			const vertex_index index_from_first_part,
			const vertex_index index_from_second_part) const
		{
			const size_t row = index_from_first_part.to_size_t();
			const size_t column = index_from_second_part.to_size_t();
			return weights_matrix_.at(row).at(column);
		}

		[[nodiscard]] constexpr size_t rows() const noexcept
		{
			return weights_matrix_.size();
		}

		[[nodiscard]] constexpr size_t columns() const noexcept
		{
			if (weights_matrix_.empty())
			{
				return 0;
			}
			return weights_matrix_[0].size();
		}

	private:
		static constexpr void validate_rows_lengths(
			const std::vector<std::vector<edge_weight<WeightT>>>& weights_matrix)
		{
			if (weights_matrix.empty())
			{
				return;
			}

			const size_t columns_count = weights_matrix[0].size();
			for (size_t row_index = 1; row_index < weights_matrix.size(); ++row_index)
			{
				if (weights_matrix[row_index].size() != columns_count)
				{
					throw common_exceptions::incorrect_vector_length(
						"bipartite_graph_weights_matrix: rows have different length.");
				}
			}
		}
	};
}
