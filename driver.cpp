/**
 * @file driver.cpp
 * @brief File containing functions that drive the application.
 * @author Ankit Srivastava <asrivast@gatech.edu>
 *
 * Copyright 2018 Georgia Institute of Technology
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "Intervals.hpp"
#include "Points.hpp"
#include "ProgramOptions.hpp"

#include <iostream>


/**
 * @brief  Function for printing the intervals stabbed by the given points.
 *
 * @tparam DataType  Datatype of the interval limits and the points.
 * @param options    Program options.
 */
template <typename DataType>
static
void
stabIntervals(
  const ProgramOptions& options
)
{
  Intervals<DataType> intervals;
  // Read intervals from the file, if one is provided.
  // Otherwise, generate random intervals.
  std::default_random_engine generator(options.randomSeed());
  if (!options.intervalsFile().empty()) {
    intervals = Intervals<DataType>(options.intervalsFile());
  }
  else if (options.numIntervals() > 0) {
    intervals = Intervals<DataType>(options.numIntervals(), generator);
  }
  else {
    throw std::runtime_error("No intervals provided.");
  }
  Points<DataType> points;
  // Read points from the file, if one is provided.
  // Otherwise, generate random points.
  if (!options.pointsFile().empty()) {
    points = Points<DataType>(options.pointsFile());
  }
  else if (options.numPoints() > 0) {
    points = Points<DataType>(options.numPoints(), generator);
  }
  else {
    throw std::runtime_error("No points provided.");
  }
  std::unordered_map<size_t, std::vector<size_t> > stabs = intervals.stab(points, options.deviceName(), options.macrosDir(), options.fsmName(), options.maxChunkSize());

  // Print the stabbed intervals.
  if (stabs.empty()) {
    std::cout << "None of the points were found to be stabbing any intervals." << std::endl;
  }
  else {
    std::cout << "Point\tStabbed Intervals" << std::endl;
    for (size_t p = 0; p < points.count(); ++p) {
      std::cout << points.get(p);
      if (stabs.find(p) != stabs.end()) {
        for (const size_t& i : stabs[p]) {
          const std::pair<DataType, DataType>& interval = intervals.get(i);
          std::cout << "\t[" << interval.first << "," << interval.second << "]";
        }
      }
      std::cout << std::endl;
    }
  }
}

/**
 * @brief  Main function for parsing the arguments and instantiating the application with correct datatype.
 *
 * @param argc  Number of provided arguments.
 * @param argv  A char array containing arguments.
 *
 * @return  0 if the execution completed smoothly. A non-zero code otherwise.
 */
int
main(
  int argc,
  char** argv
)
{
  ProgramOptions options;
  try {
    options.parse(argc, argv);
  }
  catch (po::error& pe) {
    std::cerr << pe.what() << std::endl;
    return 1;
  }

  try {
    if (options.isReal()) {
      throw std::runtime_error("Real intervals aren't supported yet.");
      //if (options.numBytes() == 4) {
        //stabIntervals<float>(options);
      //}
      //else if (options.numBytes() == 8) {
        //stabIntervals<double>(options);
      //}
    }
    else if (options.isSigned()) {
      if (options.numBytes() == 4) {
        stabIntervals<int32_t>(options);
      }
      else if (options.numBytes() == 8) {
        stabIntervals<int64_t>(options);
      }
      else {
        throw std::runtime_error("Unsupported number of bytes.");
      }
    }
    else {
      if (options.numBytes() == 4) {
        stabIntervals<uint32_t>(options);
      }
      else if (options.numBytes() == 8) {
        stabIntervals<uint64_t>(options);
      }
      else {
        throw std::runtime_error("Unsupported number of bytes.");
      }
    }
  }
  catch (std::runtime_error& re) {
    std::cerr << re.what() << std::endl;
    return 1;
  }

  return 0;
}
