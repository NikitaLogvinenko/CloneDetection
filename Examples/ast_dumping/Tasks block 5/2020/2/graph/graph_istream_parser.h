#pragma once

#include <istream>
#include <set>

#include "graph.h"

class graph_parser
{
public:
	virtual ~graph_parser() = default;
	virtual bool try_get_graph(graph& graph) = 0;
	virtual bool try_get_start_vertex(vertex& start) = 0;
};

class graph_istream_parser final : graph_parser
{
	const std::set<char> delimiters_ = {',', ';', '&', '/', '\\', '|'};
	std::istream& is_;

public:

	explicit graph_istream_parser(std::istream& is) : is_(is)
	{
	}

	bool try_get_graph(graph& graph) override;
	bool try_get_start_vertex(vertex& start) override;
};
