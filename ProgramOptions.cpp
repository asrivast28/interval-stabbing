/**
 * @file ProgramOptions.cpp
 * @brief Implementation of ProgramOptions functions.
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
#include "ProgramOptions.hpp"

#include <iostream>
#include <numeric>

#include <boost/filesystem.hpp>


ProgramOptions::ProgramOptions(
) : m_options("Determines which of the given intervals were stabbed by the given points"),
    m_deviceName(),
    m_macrosDir(),
    m_fsmName(),
    m_intervalsFile(),
    m_pointsFile(),
    m_numBytes(),
    m_randomSeed(),
    m_numIntervals(),
    m_numPoints(),
    m_isReal(),
    m_isSigned()
{
  m_options.add_options()
    ("help,h", "Print this message.")
    ("device,d", po::value<std::string>(&m_deviceName), "Name of the AP device to be used for stabbing intervals.")
    ("macros,m", po::value<std::string>(&m_macrosDir)->default_value("./comparators"), "Directory which contains all the comparator macros.")
    ("fsm,f", po::value<std::string>(&m_fsmName), "Name of the FSM file to be written.")
    ("intervals,i", po::value<std::string>(&m_intervalsFile), "Name of the file from which intervals are to be read.")
    ("points,p", po::value<std::string>(&m_pointsFile), "Name of the file from which points are to be read.")
    ("bytes,b", po::value<size_t>(&m_numBytes)->default_value(4), "Number of bytes.")
    ("seed,s", po::value<size_t>(&m_randomSeed)->default_value(0), "Seed for random number generator.")
    ("random-intervals,I", po::value<size_t>(&m_numIntervals)->default_value(0), "Number of random intervals to be programmed.")
    ("random-points,P", po::value<size_t>(&m_numPoints)->default_value(0), "Number of random points to be used for stabbing.")
    ("chunks,c", po::value<size_t>(&m_maxChunkSize)->default_value(std::numeric_limits<size_t>::max()), "Maximum chunk size for flows to the AP.")
    ("real", po::bool_switch(&m_isReal)->default_value(false), "Use real numbers for labeling.")
    ("signed", po::bool_switch(&m_isSigned)->default_value(false), "Use signed numbers for labeling.")
    ;
}

void
ProgramOptions::parse(
  int argc,
  char** argv
)
{
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, m_options), vm);
  po::notify(vm);

  if ((argc == 1) || (vm.count("help") > 0)) {
    std::stringstream ss;
    ss << m_options;
    throw po::error(ss.str());
  }
  if (!m_intervalsFile.empty() && !boost::filesystem::exists(boost::filesystem::path(m_intervalsFile))) {
    throw po::error("Couldn't find the intervals file.");
  }
  if (!m_pointsFile.empty() && !boost::filesystem::exists(boost::filesystem::path(m_pointsFile))) {
    throw po::error("Couldn't find the points file.");
  }
  if ((!m_intervalsFile.empty()) && (m_numIntervals > 0)) {
    std::cerr << "WARNING: \"intervals\" and \"random-intervals\" argument provided together. \"random-intervals\" will be ignored." << std::endl;
  }
  if ((!m_pointsFile.empty()) && (m_numPoints > 0)) {
    std::cerr << "WARNING: \"points\" and \"random-points\" argument provided together. \"random-points\" will be ignored." << std::endl;
  }
}

std::string
ProgramOptions::deviceName(
) const
{
  return m_deviceName;
}

std::string
ProgramOptions::macrosDir(
) const
{
  return m_macrosDir;
}

std::string
ProgramOptions::fsmName(
) const
{
  return m_fsmName;
}

std::string
ProgramOptions::intervalsFile(
) const
{
  return m_intervalsFile;
}

std::string
ProgramOptions::pointsFile(
) const
{
  return m_pointsFile;
}

size_t
ProgramOptions::numBytes(
) const
{
  return m_numBytes;
}

size_t
ProgramOptions::randomSeed(
) const
{
  return m_randomSeed;
}

size_t
ProgramOptions::numIntervals(
) const
{
  return m_numIntervals;
}

size_t
ProgramOptions::numPoints(
) const
{
  return m_numPoints;
}

size_t
ProgramOptions::maxChunkSize(
) const
{
  return m_maxChunkSize;
}

bool
ProgramOptions::isReal(
) const
{
  return m_isReal;
}

bool
ProgramOptions::isSigned(
) const
{
  return m_isSigned;
}

ProgramOptions::~ProgramOptions(
)
{
}
