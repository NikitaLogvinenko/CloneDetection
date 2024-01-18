#pragma once
#include "bipartite_matching_abstract.h"
#include "dlib/optimization/max_cost_assignment.h"
#include "weight_to_integral_transformation.h"
#include "weights_transformation.h"

namespace graphs
{
	template <utility::non_const_arithmetic WeightT,
		utility::non_const_integral CalculationsT = size_t,
		class WeightsTransform = weight_to_integral_transformation<WeightT, CalculationsT>>
		requires weights_transformation<WeightsTransform, WeightT, CalculationsT>
		class bipartite_matching_max_total_weight final : public bipartite_matching_abstract<WeightT>
	{
		WeightsTransform weights_transform_{};
		WeightT fictitious_edges_weight_{};
		CalculationsT fictitious_matrix_cell_{};

	public:
		constexpr bipartite_matching_max_total_weight()
			noexcept(std::is_nothrow_default_constructible_v<WeightsTransform> 
				&& std::is_nothrow_invocable_r_v<CalculationsT, WeightsTransform, WeightT>)
		: fictitious_matrix_cell_(weights_transform_.to_calculations_t(fictitious_edges_weight_)) {}

		constexpr explicit bipartite_matching_max_total_weight(
			const WeightsTransform& weights_transform,
			const WeightT fictitious_edges_weight = 0)
			noexcept(std::is_nothrow_copy_constructible_v<WeightsTransform>
				&& std::is_nothrow_invocable_r_v<CalculationsT, WeightsTransform, WeightT>)
		: weights_transform_(weights_transform),
		fictitious_edges_weight_(fictitious_edges_weight),
		fictitious_matrix_cell_(weights_transform_.to_calculations_t(fictitious_edges_weight_)) {}

		[[nodiscard]] bipartite_matching_result<WeightT> match_parts(
			const bipartite_graph_weights_matrix<WeightT>& bipartite_graph_weights) const override
		{
			if (bipartite_graph_weights.rows() == 0 || bipartite_graph_weights.columns() == 0)
			{
				return bipartite_matching_result<WeightT>{};
			}

			const dlib::matrix<CalculationsT> assignment_matrix(create_assignment_matrix(bipartite_graph_weights));
			return assign(bipartite_graph_weights, assignment_matrix);
		}

	private:
		[[nodiscard]] dlib::matrix<CalculationsT> create_assignment_matrix(
			const bipartite_graph_weights_matrix<WeightT>& bipartite_graph_weights) const
		{
			const long rows = bipartite_graph_weights.rows(), columns = bipartite_graph_weights.columns();
			const long max_part_cardinality = std::max(rows, columns);
			dlib::matrix<CalculationsT> assignment_matrix(max_part_cardinality, max_part_cardinality);

			fill_cells_for_existed_edges(assignment_matrix, bipartite_graph_weights);
			fill_fictitious_cells(assignment_matrix, 0, columns, max_part_cardinality);
			fill_fictitious_cells(assignment_matrix, rows, 0, max_part_cardinality);

			return assignment_matrix;
		}

		void fill_cells_for_existed_edges(dlib::matrix<CalculationsT>& assignment_matrix, 
			const bipartite_graph_weights_matrix<WeightT>& bipartite_graph_weights) const
		{
			for (size_t row = 0; row < bipartite_graph_weights.rows(); ++row)
			{
				for (size_t column = 0; column < bipartite_graph_weights.columns(); ++column)
				{
					const long row_as_long = row, column_as_long = column;
					const WeightT weight = bipartite_graph_weights.at(row, column);
					const CalculationsT assignment_matrix_cell = weights_transform_.to_calculations_t(weight);
					assignment_matrix(row_as_long, column_as_long) = assignment_matrix_cell;
				}
			}
		}

		void fill_fictitious_cells(dlib::matrix<CalculationsT>& assignment_matrix, 
			const long rows_from, const long columns_from, const long max_part_cardinality) const
		{
			if (rows_from == max_part_cardinality || columns_from == max_part_cardinality)
			{
				return;
			}

			for (long row = rows_from; row < max_part_cardinality; ++row)
			{
				for (size_t column = columns_from; column < max_part_cardinality; ++column)
				{
					assignment_matrix(row, column) = fictitious_matrix_cell_;
				}
			}
		}

		[[nodiscard]] bipartite_matching_result<WeightT> assign(
			const bipartite_graph_weights_matrix<WeightT>& bipartite_graph_weights,
			const dlib::matrix<CalculationsT>& assignment_matrix) const
		{
			const std::vector assignment = dlib::max_cost_assignment(assignment_matrix);
			const CalculationsT max_cost = dlib::assignment_cost(assignment_matrix, assignment);
			const WeightT sum_of_matching_edges = weights_transform_.to_weight_t(max_cost);

			std::vector<edge<WeightT>> matching_edges = fill_matching_edges(bipartite_graph_weights, assignment);

			return bipartite_matching_result<WeightT>(sum_of_matching_edges, std::move(matching_edges));
		}

		[[nodiscard]] std::vector<edge<WeightT>> fill_matching_edges(
			const bipartite_graph_weights_matrix<WeightT>& bipartite_graph_weights, 
			const std::vector<long>& assignment) const
		{
			std::vector<edge<WeightT>> matching_edges{};
			matching_edges.reserve(assignment.size());

			long row = 0;
			for (const long assigned_column : assignment)
			{
				if (assigned_column >= static_cast<long>(bipartite_graph_weights.columns()))
				{
					row++;
					continue;
				}
				if (row >= static_cast<long>(bipartite_graph_weights.rows()))
				{
					break;
				}

				const vertex_index vertex_from_first_part(row);
				const vertex_index vertex_from_second_part(assigned_column);
				const WeightT matching_edge_weight = bipartite_graph_weights.at(vertex_from_first_part, vertex_from_second_part);
				matching_edges.emplace_back(vertex_from_first_part, vertex_from_second_part, edge_weight<WeightT>(matching_edge_weight));
				++row;
			}

			return matching_edges;
		}
	};
}
