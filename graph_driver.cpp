// Copyright (C) 2009 by Jhair Tocancipa Triana

// This programs use a greedy algorithm to color the edges of a given
// graph. Additionally it tries to color the edges if enough channels
// are given.

#include "fileio.hpp"
#include "graph.hpp"
#include "graph_reader.hpp"

#include <boost/program_options.hpp>
namespace po = boost::program_options;
using namespace std;

int main(int ac, char* av[])
{
  std::string input_file;
  int number_of_channels = -1;

  try
  {
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ("input-file", po::value<std::string>(), "graph input file")
      ("nc", po::value<int>(), "number of channels")
      ;

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    po::positional_options_description p;
    p.add("input-file", -1);
    po::store(po::command_line_parser(ac, av).
              options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
      cout << desc << std::endl;
      return 1;
    }

    if (vm.count("nc"))
    {
      cout << "Number of channels is: "
           << vm["nc"].as<int>() << std::endl;

      number_of_channels = vm["nc"].as<int>();
    }
    else
    {
      cout << "Number of channels was not set.\n";
    }

    if (vm.count("input-file"))
    {
      std::cout << "Input file is: "
                << vm["input-file"].as<std::string>() << std::endl;

      input_file = vm["input-file"].as<std::string>();
    }
    else
    {
      cout << "Input file was not set.\n";
    }
  }
  catch(exception& e)
  {
    cerr << "error: " << e.what() << std::endl;
    return 1;
  }
  catch(...)
  {
    cerr << "Exception of unknown type!\n";
  }

  GraphReader gReader;
  Graph g; // = gReader.loadGraph(input_file);

  // Alternatively use this to create a random graph
  int n_verts = 50;
  int n_edges = 1225;
  std::size_t seed = 2;
  boost::minstd_rand gent(seed);
  boost::generate_random_graph(g, n_verts, n_edges, gent, false);

  // Print out the graph
  FileIOHandle rt(input_file + ".dot", std::ios_base::out);
  boost::write_graphviz(*(rt.getFileStream()), g);

  // Color the edges using the greedy algorithm
  std::map<edge_t, int> colorMap = greedyEdgeColoring(g);

  // Print out the color map and obtain the maximum number of colors
  // used
  std::set<int> colors_used;
  std::cout << "Result:" << std::endl;
  std::pair<edge_t, int> colorMapEntry;
  foreach (colorMapEntry, colorMap)
  {
    std::cout << colorMapEntry.first << "->"
              << colorMapEntry.second
              << std::endl;

    // Collect the colors to get the maximum number of colors used
    colors_used.insert(colorMapEntry.second);
  }

  int max_color = *std::max_element(colors_used.begin(), colors_used.end());

  std::cout << "Max color:" << max_color
            << " (Number of channels:" << number_of_channels << ")" << std::endl;

  colors_used.clear();

  if (max_color <= number_of_channels)
  {
    // Nothing to be done.
    std::cout << "Nothing to be done. There are enough channels to color the edges" << std::endl;
  }
  else if (number_of_channels != -1 && number_of_channels != 0)
  {
    // At this point we found a coloring but we don't have enough
    // channels available. So map the obtained colors onto the
    // available channels.
    //
    std::cout << "Adjusting mapping. There are no enough channels to color the edges!" << std::endl;

    foreach (colorMapEntry, colorMap)
    {
      colorMapEntry.second = (colorMapEntry.second % number_of_channels) + 1;

      std::cout << colorMapEntry.first << "->"
                << colorMapEntry.second
                << std::endl;

      // Collect the colors to get the maximum number of colors used
      colors_used.insert(colorMapEntry.second);
    }
  }

  return 0;
}
