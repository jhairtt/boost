// Copyright (C) 2009 by Jhair Tocancipa Triana

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <utility>
#include <algorithm>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/foreach.hpp>
#include <boost/graph/properties.hpp>
#include <boost/random.hpp>
#include <boost/graph/random.hpp>

#define foreach         BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::undirectedS,
                              boost::no_property,
                              boost::no_property> Graph;

typedef std::pair<int, int> Edge;
typedef boost::property_map<Graph, boost::vertex_index_t>::type VertexIndexMap;
typedef boost::graph_traits<Graph> GraphTraits;  
typedef GraphTraits::vertex_descriptor vertex_t;
typedef GraphTraits::edge_descriptor edge_t;

typedef boost::adjacency_list<boost::listS,
                              boost::vecS,
                              boost::directedS,
                              boost::no_property,
                              boost::property
                              <boost::edge_weight_t, int> > WeightedGraph;

std::map<edge_t, int> greedyEdgeColoring(Graph g)
{
  std::map<edge_t, int> colorMap;
  std::pair<edge_t, int> colorMapEntry;

  // Random number generator
  boost::mt19937 gen;  

  // Loop until all edges have been colored
  while (colorMap.size() < boost::num_edges(g))
  {
    edge_t e = boost::random_edge(g, gen);

    if (colorMap.find(e) == colorMap.end())
    {
      // This edge hasn't been colored
      GraphTraits::out_edge_iterator oi, oi_end;
      std::set<int> min_colors;

      // Get the colors of all neighbours of this edge
      for (boost::tie(oi, oi_end) = boost::out_edges(boost::source(e, g), g);
           oi != oi_end;
           ++oi)
      {
        if (colorMap.find(*oi) != colorMap.end())
        {
          min_colors.insert(colorMap[*oi]);          
        }
      }
      for (boost::tie(oi, oi_end) = boost::out_edges(boost::target(e, g), g);
           oi != oi_end;
           ++oi)
      {
        if (colorMap.find(*oi) != colorMap.end())
        {
          min_colors.insert(colorMap[*oi]);          
        }
      }

      // Now the greedy part. The color of this edge is the minimum
      // possible given the colors of all its neighbours.
      int min_color = *std::max_element(min_colors.begin(), min_colors.end());
      colorMap[e] = min_color + 1;

      // Reset this set since we are done with the coloring of this
      // edge.
      min_colors.clear();
    }
  }

  // Return the edge coloring
  return colorMap;
}

// Visitor to record predecessor's while running the Dijsktra
// algorithm
template <class PredecessorMap>
class record_predecessors : public boost::dijkstra_visitor<>
{
public:
  record_predecessors(PredecessorMap p) : m_predecessor(p) {};

  template<class Edge, class WeightedGraph>
  void edge_relaxed(Edge e, WeightedGraph &g)
  {
    boost::put(m_predecessor, boost::target(e, g), boost::source(e, g));
  }
  
protected:
  PredecessorMap m_predecessor;
};

template<class Graph>
class exercise_vertex
{
public:
  exercise_vertex(Graph &g_) : g(g_) {}

  // Vertex descriptor
  void operator()(const vertex_t& v) const
  {
    VertexIndexMap index = boost::get(boost::vertex_index, g);

    // Visit the out edges for v
    std::cout
      << "out-edges ("
      << v
      << "):";
    GraphTraits::out_edge_iterator oi, oi_end;
    GraphTraits::edge_descriptor ed;
    for (boost::tie(oi, oi_end) = boost::out_edges(v, g); oi != oi_end; ++oi)
    {
      ed = *oi;
      vertex_t src = boost::source(ed, g);
      vertex_t targ = boost::target(ed, g);
      std::cout
        << "("
        << index[src]
        << ","
        << index[targ]
        << ") ";
    }
    std::cout << std::endl;

    // Visit the in edges for v
    std::cout
      << " in-edges ("
      << v
      << "):";
    GraphTraits::in_edge_iterator ii, ii_end;
    for (boost::tie(ii, ii_end) = boost::in_edges(v, g); ii != ii_end; ++ii)
    {
      ed = *ii;
      vertex_t src = boost::source(ed, g);
      vertex_t targ = boost::target(ed, g);
      std::cout
        << "("
        << index[src]
        << ","
        << index[targ]
        << ") ";
    }
    std::cout << std::endl;

    // Visit the adjacent vertices (neighbours) for v
    std::cout
      << "neighbours("
      << v
      << "):";
    GraphTraits::adjacency_iterator ai, ai_end;
    for (boost::tie(ai, ai_end) = boost::adjacent_vertices(v, g);
         ai != ai_end;
         ++ai)
    {
      std::cout
        << "("
        << index[*ai]
        << ") ";
    }
    std::cout << std::endl;
  }

  Graph &g;
};

template <class PredecessorMap>
record_predecessors<PredecessorMap> make_predecessor_recorder(PredecessorMap p)
{
  return record_predecessors<PredecessorMap>(p);
}

#endif // GRAPH_HPP
