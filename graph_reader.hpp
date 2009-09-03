// Copyright (C) 2009 by Jhair Tocancipa Triana

#ifndef GRAPH_READER_HPP
#define GRAPH_READER_HPP

#include "graph.hpp"
#include "util.hpp"
#include "fileio.hpp"

#include <boost/graph/graphviz.hpp>

class GraphReader
{
public:
  GraphReader() : read_section(Unknown) {}

  bool readLine(const std::string &line)
  {
    const std::string l(line);

    if (l.compare("[Vertices]") == 0)
    {
      read_section = Vertices;
    }
    else if (l.compare("[Edges]") == 0)
    {
      read_section = Edges;
    }

    switch (read_section)
    {
    case Vertices:
      if (l.compare("[Vertices]") != 0)
      {
        std::vector<std::string> v;
        v = Util::splitString(l);

        std::vector<std::string>::iterator i = v.begin();
        int j = 0;        
        for (; i != v.end(); ++i)
        {
          vertices[*i] = (j);
          reverse_vertices[j] = vertices[*i];
          j++;
        }
      }
      break;
    case Edges:
      if (l.compare("[Edges]") != 0)
      {
        std::vector<std::string> e = Util::splitString(l);

        if (e.size () == 2)
        {
          edges.push_back(Edge(vertices[e[0]], vertices[e[1]]));
        }
        else
        {
          std::cout << "Ignoring invalid edge, check input file!" << std::endl;
        }
      }
      break;
    case Unknown:
    default:
      break;
    }

    return true;
  }

  Graph loadGraph(std::string fileName)
  {
    FileIOHandle fio_handle(fileName);

    fio_handle.read(*(dynamic_cast <GraphReader *> (this)),
                    &GraphReader::readLine);

    // Create the graph
    return Graph(&edges[0], &edges[0] + edges.size(), vertices.size());
  }

  void printGraph(Graph g)
  {
    std::cout << "Vertices:" << std::endl;
    VertexIndexMap index = boost::get(boost::vertex_index, g);
    GraphTraits::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi)
    {
      std::cout << index[*vi] << " ";
    }
    std::cout << std::endl;

    std::cout << "Edges:" << std::endl; GraphTraits::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei)
    {
      std::cout
        << "(" << index[boost::source(*ei, g)]
        << "," << index[boost::target(*ei, g)]
        << ") "
        << std::endl;
    }
  }

  void reset()
  {
    vertices.clear();
    reverse_vertices.clear();
    edges.clear();
  }


private:
  enum 
    {
      Unknown, Vertices, Edges
    };
  int read_section;
  std::map<std::string, int> vertices;
  std::map<int, std::string> reverse_vertices;
  std::vector<Edge> edges;
};

#endif // GRAPH_READER_HPP
