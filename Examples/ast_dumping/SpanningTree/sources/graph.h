#pragma once
#include "edge.h"
#include "graph_exceptions.h"
#include <unordered_set>

namespace graph_ns
{
	template <typename VertexT>
   class graph final
	{
		std::unordered_set<VertexT> vertices_ {};
		std::unordered_set<edge<VertexT>> edges_ {};
		bool directed_ {false};

	public:
		graph() noexcept = default;
		explicit graph(bool directed) noexcept;
		explicit graph(std::unordered_set<VertexT> vertices, bool directed = false) noexcept;
		explicit graph(std::unordered_set<VertexT> vertices, std::unordered_set<edge<VertexT>> edges, bool directed = false);

		[[nodiscard]] const std::unordered_set<VertexT>& vertices() const noexcept;
		[[nodiscard]] const std::unordered_set<edge<VertexT>>& edges() const noexcept;
		[[nodiscard]] bool directed() const noexcept;
		[[nodiscard]] size_t vertices_count() const noexcept;
		[[nodiscard]] size_t edges_count() const noexcept;

		bool insert(VertexT vertex);
		bool insert(const edge<VertexT>& edge, bool insert_vertices);

	private:
		void edge_validation(const edge<VertexT>& edge);
		void edges_validation();
	};

	template <typename VertexT>
	graph<VertexT>::graph(const bool directed) noexcept : directed_(directed) { }

	template <typename VertexT>
	graph<VertexT>::graph(std::unordered_set<VertexT> vertices, const bool directed) noexcept
		: vertices_(std::move(vertices)), directed_(directed) { }

	template <typename VertexT>
	graph<VertexT>::graph(std::unordered_set<VertexT> vertices, std::unordered_set<edge<VertexT>> edges, const bool directed)
		: vertices_(std::move(vertices)), edges_(std::move(edges)), directed_(directed)
	{
		if (!edges_.empty())
		{
			edges_validation();
		}
	}

	template <typename VertexT>
	const std::unordered_set<VertexT>& graph<VertexT>::vertices() const noexcept
	{
		return vertices_;
	}

	template <typename VertexT>
	const std::unordered_set<edge<VertexT>>& graph<VertexT>::edges() const noexcept
	{
		return edges_;
	}

	template <typename VertexT>
	bool graph<VertexT>::directed() const noexcept
	{
		return directed_;
	}

	template <typename VertexT>
	size_t graph<VertexT>::vertices_count() const noexcept
	{
		return vertices_.size();
	}

	template <typename VertexT>
	size_t graph<VertexT>::edges_count() const noexcept
	{
		return edges_.size();
	}

	template <typename VertexT>
	bool graph<VertexT>::insert(VertexT vertex)
	{
		return vertices_.insert(vertex).second;
	}

	template <typename VertexT>
	bool graph<VertexT>::insert(const edge<VertexT>& edge, const bool insert_vertices)
	{
		try
		{
			edge_validation(edge);
			return edges_.insert(edge).second;
		}

		catch ([[maybe_unused]] const graph_incompatible_edge_direction& ex)
		{
			return false;
		}

		catch ([[maybe_unused]] const graph_non_existent_vertices& ex)
		{
			if (!insert_vertices)
			{
				return false;
			}

			insert(edge.vertex_begin());
			insert(edge.vertex_end());
			return edges_.insert(edge).second;
		}
	}

	template <typename VertexT>
	void graph<VertexT>::edge_validation(const edge<VertexT>& edge)
	{
		if (edge.directed() != directed_)
		{
			throw graph_incompatible_edge_direction();
		}
		if (!vertices_.contains(edge.vertex_begin()) || !vertices_.contains(edge.vertex_end()))
		{
			throw graph_non_existent_vertices();
		}
	}

	template <typename VertexT>
	void graph<VertexT>::edges_validation()
	{
		for (const auto& edge : edges_)
		{
			edge_validation(edge);
		}
	}
}
