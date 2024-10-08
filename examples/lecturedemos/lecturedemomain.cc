// **********************************************************************
// This file is part of the LehrFEM++ finite element library developed
// from 2018 at the Seminar of Applied Mathematics of ETH Zurich for
// teaching purposes.
// This header must not be removed.
// **********************************************************************
/**
 * @file
 * @brief Driver function for simple LehrFEM++ demo
 * @author Ralf Hiptmair
 * @date   January 2019
 * @copyright MIT License
 */

#include <boost/program_options.hpp>

#include "lecturedemoassemble.h"
#include "lecturedemodof.h"
#include "lecturedemomesh.h"
#include "lecturedemomeshfunction.h"
#include "lecturedemoquad.h"
#include "lecturedemorefine.h"
#include "lecturedemotwonorm.h"

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
int main(int argc, char **argv) {
  // We rely on Boost's program_option library for parsing command line
  // arguments
  namespace po = boost::program_options;
  // We specify what to do for the two allowed options -h and -d, which are
  // linked to the keys 'help' and 'demo_number'. We tell the computer that -d
  // expects an integer argument and that the default value is 0
  po::options_description desc("Allowed options");
  // clang-format off
  desc.add_options()
  ("help,h", "This message")
  ("demo_number,d", po::value<int>()->default_value(0), "Selector for demo code");
  // clang-format on
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  if (vm.count("help") > 0) {
    std::cout << desc << '\n';
    std::cout << "No arg: run all demos" << '\n';
    std::cout << "N = 1: demo of LehrFEM++ mesh capabilities" << '\n';
    std::cout << "N = 2: demo of LehrFEM++ DofHandler capabilities" << '\n';
    std::cout << "N = 3: demo of LehrFEM++ assembly of LSE" << '\n';
    std::cout << "N = 4: demo of numerical quadrature in LehrFEM++" << '\n';
    std::cout << "N = 5: demo of solving a Dirichlet BVP" << '\n';
    std::cout << "N = 6: demo of mesh refinement" << '\n';
    std::cout << "N = 7: Various of ways of computing an L2-norm" << '\n';
    std::cout << "N = 8: Demo for MeshFunction" << '\n';
  } else {
    const int selector = vm["demo_number"].as<int>();
    if ((selector == 1) || (selector == 0)) {
      lecturedemo::lecturedemomesh();
    }
    if ((selector == 2) || (selector == 0)) {
      lecturedemo::lecturedemodof();
    }
    if ((selector == 3) || (selector == 0)) {
      lecturedemo::lecturedemoassemble();
    }
    if ((selector == 4) || (selector == 0)) {
      lecturedemo::lecturedemoquad();
    }
    if ((selector == 5) || (selector == 0)) {
      lecturedemo::lecturedemoDirichlet();
    }
    if ((selector == 6) || (selector == 0)) {
      lecturedemo::lecturedemorefine();
    }
    if ((selector == 7) || (selector == 0)) {
      lecturedemo::lecturedemotwonorm();
    }
    if ((selector == 8) || (selector == 0)) {
      lecturedemo::lecturedemomeshfunction();
    }
  }
  return 0L;
}
