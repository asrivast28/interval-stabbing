/**
 * @file ProgramOptions.hpp
 * @brief Declaration of ProgramOptions functions.
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
#ifndef PROGRAMOPTIONS_HPP_
#define PROGRAMOPTIONS_HPP_

#include <string>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

/**
 * Utility class for parsing command line arguments.
 */
class ProgramOptions {
public:
  ProgramOptions();

  void
  parse(int, char**);

  std::string
  deviceName() const;

  std::string
  macrosDir() const;

  std::string
  fsmName() const;

  std::string
  intervalsFile() const;

  std::string
  pointsFile() const;

  size_t
  numBytes() const;

  size_t
  randomSeed() const;

  size_t
  numIntervals() const;

  size_t
  numPoints() const;

  size_t
  maxChunkSize() const;

  bool
  isReal() const;

  bool
  isSigned() const;

  ~ProgramOptions();

private:
  po::options_description m_options;
  std::string m_deviceName;
  std::string m_macrosDir;
  std::string m_fsmName;
  std::string m_intervalsFile;
  std::string m_pointsFile;
  size_t m_numBytes;
  size_t m_randomSeed;
  size_t m_numIntervals;
  size_t m_numPoints;
  size_t m_maxChunkSize;
  bool m_isReal;
  bool m_isSigned;
}; // class ProgramOptions

#endif // PROGRAMOPTIONS_HPP_
