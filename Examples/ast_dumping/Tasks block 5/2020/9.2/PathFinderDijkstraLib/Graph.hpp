#pragma once
#include <string>
#include <list>
#include <unordered_map>

class Graph final{
public:
    typedef std::string Vertex;
    typedef unsigned int Weight;

    typedef std::list<std::pair<Vertex, Weight>> AdjList;
    typedef std::list<Vertex> Vertices;

    void AddEdge(const Vertex& src, const Weight& len, const Vertex& dest);

    const AdjList& GetAdjList(const Vertex& src) const;
    const Vertices& GetVerticesList() const;

    ~Graph() = default;

private:
    std::unordered_map<Vertex, AdjList> data;
    Vertices vertices_list;

    const AdjList empty_adj_list;
    const Vertices empty_vertices_list;
};