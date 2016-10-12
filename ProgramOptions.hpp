/**
 * @file ProgramOptions.hpp
 * @brief Declaration of ProgramOptions functions.
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
  macroFile() const;

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
