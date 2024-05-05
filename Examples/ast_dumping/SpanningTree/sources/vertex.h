#pragma once

#include <algorithm>

namespace graph_ns
{
	template <typename Tid, typename Tvalue>
	class vertex
	{
		Tid id_;
		Tvalue value_;

	public:
		explicit vertex(Tid id, Tvalue value) noexcept;

		[[nodiscard]] Tid id() const noexcept;
		[[nodiscard]] const Tvalue& value() const noexcept;
	
		[[nodiscard]] bool operator==(const vertex& other) const noexcept;
	};

	template <typename Tid, typename Tvalue>
	vertex<Tid, Tvalue>::vertex(Tid id, Tvalue value) noexcept : id_{std::move(id)}, value_{ std::move(value) } { }

	template <typename Tid, typename Tvalue>
	Tid vertex<Tid, Tvalue>::id() const noexcept
	{
		return id_;
	}

	template <typename Tid, typename Tvalue>
	const Tvalue& vertex<Tid, Tvalue>::value() const noexcept
	{
		return value_;
	}

	template <typename Tid, typename Tvalue>
	bool vertex<Tid, Tvalue>::operator==(const vertex& other) const noexcept
	{
		static_assert(noexcept(id_ == other.id_), "vertex operator== mustn't throw");
		return id_ == other.id_;
	}
}

template<typename Tid, typename Tvalue>
struct std::hash<graph_ns::vertex<Tid, Tvalue>>
{
	size_t operator() (const graph_ns::vertex<Tid, Tvalue>& vertex) const noexcept
	{
		return std::hash<Tid>{}(vertex.id());
	}
};

