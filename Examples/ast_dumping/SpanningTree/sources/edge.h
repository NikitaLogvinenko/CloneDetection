#pragma once
#include "vertex.h"
#include <stdexcept>
#include <memory>

namespace graph_ns
{
	template <typename VertexT>
   class edge final
	{
		VertexT vertex_begin_;
		VertexT vertex_end_;
		double weight_;
		bool directed_;

	public:
		explicit edge(VertexT vertex_begin, VertexT vertex_end, double weight, bool directed = false) noexcept;

		[[nodiscard]] bool operator==(const edge& other) const noexcept;

		[[nodiscard]] const VertexT& vertex_begin() const noexcept;
		[[nodiscard]] const VertexT& vertex_end() const noexcept;
		[[nodiscard]] double weight() const noexcept;
		[[nodiscard]] bool directed() const noexcept;
	};

	template <typename VertexT>
	edge<VertexT>::edge(VertexT vertex_begin, VertexT vertex_end, const double weight, const bool directed) noexcept
		: vertex_begin_(std::move(vertex_begin)), vertex_end_(std::move(vertex_end)), weight_(weight), directed_(directed) { }

	template <typename VertexT>
	bool edge<VertexT>::operator==(const edge& other) const noexcept
	{
		return (vertex_begin() == other.vertex_begin() && vertex_end() == other.vertex_end()
				|| !directed() && vertex_begin() == other.vertex_end() && vertex_end() == other.vertex_begin())
				&& weight() == other.weight()
				&& directed() == other.directed();
	}

	template <typename VertexT>
	const VertexT& edge<VertexT>::vertex_begin() const noexcept
	{
		return vertex_begin_;
	}

	template <typename VertexT>
	const VertexT& edge<VertexT>::vertex_end() const noexcept
	{
		return vertex_end_;
	}

	template <typename VertexT>
	double edge<VertexT>::weight() const noexcept
	{
		return weight_;
	}

	template <typename VertexT>
	bool edge<VertexT>::directed() const noexcept
	{
		return directed_;
	}
}

template<typename VertexT>
struct std::hash<graph_ns::edge<VertexT>>
{
	size_t operator() (const graph_ns::edge<VertexT>& edge) const noexcept
	{
		return std::hash<VertexT>{}(edge.vertex_begin()) + std::hash<VertexT>{}(edge.vertex_end());
	}
};
