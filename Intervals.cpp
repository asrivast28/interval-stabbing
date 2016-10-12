/**
 * @file Intervals.cpp
 * @brief Implementation of Intervals functions.
 */
#include "Intervals.hpp"

#include "apsdk/Anml.hpp"
#include "apsdk/Device.hpp"
#include "LabelingAlgorithms.hpp"

#include <cstring>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <unordered_map>

/**
 * @brief  Function for copying a chunk of memory in reverse.
 *
 * @param destPtr  Pointer to the destination memory.
 * @param srcPtr   Pointer to the source memory.
 * @param size     Size of the chunk to be copied.
 */
static
void
reverse_memcpy(
  void* destPtr,
  const void* srcPtr,
  size_t size
)
{
  unsigned char* dest = static_cast<unsigned char*>(destPtr);
  const unsigned char* src = static_cast<const unsigned char*>(srcPtr);
  src += (size - 1);
  for (size_t i = 0; i < size; ++i, ++dest, --src) {
    memcpy(dest, src, 1);
  }
}


/**
 * @brief  Default constructor for initializing empty intervals. 
 *
 * @tparam LimitType  Datatype of the interval limits.
 */
template <typename LimitType>
Intervals<LimitType>::Intervals(
) : m_intervals()
{
}

/**
 * @brief  Constructor for reading the intervals from the given file.
 *
 * @tparam LimitType      Datatype of the interval limits.
 * @param  intervalsFile  Name of the file from which intervals are to be read.
 */
template <typename LimitType>
Intervals<LimitType>::Intervals(
  const std::string& intervalsFile
) : m_intervals()
{
  std::ifstream intervals(intervalsFile);
  std::string line;
  while (std::getline(intervals, line)) {
    std::istringstream is(line);
    LimitType x, y;
    is >> x >> y;
    m_intervals.push_back(std::make_pair(x, y));
  }
}

/**
 * @brief  Constructor for generating random points.
 *
 * @tparam LimitType              Datatype of the interval limits.
 * @tparam RandomNumberGenerator  Type of the random number generator. 
 * @param  numRandom              Number of random intervals to be generated.
 * @param  generator              Instance of the random number generator. 
 */
template <typename LimitType>
template <typename RandomNumberGenerator>
Intervals<LimitType>::Intervals(
  const size_t,
  RandomNumberGenerator&
) : m_intervals()
{
  throw std::runtime_error("Random generation of intervals hasn't been implemented for the datatype.");
}

// Specialization of the random interval generator constructor for integer datatypes.
#define INITIALIZE_INTEGER(IntegerType) \
template <> \
template <typename RandomNumberGenerator> \
Intervals<IntegerType>::Intervals( \
  const size_t numRandom, \
  RandomNumberGenerator& generator \
) : m_intervals() \
{ \
  IntegerType lower = std::numeric_limits<IntegerType>::min(); \
  IntegerType upper = std::numeric_limits<IntegerType>::max(); \
  std::uniform_int_distribution<IntegerType> distribution(lower, upper); \
  std::cout << "Following are the randomly generated intervals:" << std::endl; \
  for (size_t i = 0; i < numRandom; ++i) { \
    IntegerType x = distribution(generator); \
    IntegerType y = distribution(generator); \
    if (x > y) { \
      IntegerType temp = x; \
      x = y; \
      y = temp; \
    } \
    m_intervals.push_back(std::make_pair(x, y)); \
    std::cout << "[" << x << "," << y << "]" << std::endl; \
  } \
  std::cout << std::endl; \
}

INITIALIZE_INTEGER(uint32_t);
INITIALIZE_INTEGER(int32_t);
INITIALIZE_INTEGER(uint64_t);
INITIALIZE_INTEGER(int64_t);

// Specialization of the random interval generator constructor for real datatypes.
#define INITIALIZE_REAL(RealType) \
template <> \
template <typename RandomNumberGenerator> \
Intervals<RealType>::Intervals( \
  const size_t numRandom, \
  RandomNumberGenerator& generator \
) : m_intervals() \
{ \
  RealType lower = std::numeric_limits<RealType>::min(); \
  RealType upper = std::numeric_limits<RealType>::max(); \
  std::uniform_real_distribution<RealType> distribution(lower, upper); \
  std::cout << "Following are the randomly generated intervals:" << std::endl; \
  for (size_t i = 0; i < numRandom; ++i) { \
    RealType x = distribution(generator); \
    RealType y = distribution(generator); \
    if (x > y) { \
      RealType temp = x; \
      x = y; \
      y = temp; \
    } \
    m_intervals.push_back(std::make_pair(x, y)); \
    std::cout << "[" << x << "," << y << "]" << std::endl; \
  } \
  std::cout << std::endl; \
}

//INITIALIZE_REAL(float);
//INITIALIZE_REAL(double);


/**
 * @brief  Function for accessing the interval at a given index.
 *
 * @tparam LimitType  Datatype of the interval limits.
 * @param  index      Index of the interval to be accessed. 
 *
 * @return  A const reference to the interval at the given index.
 */
template <typename LimitType>
const std::pair<LimitType, LimitType>&
Intervals<LimitType>::get(
  const size_t index
) const
{
  return m_intervals[index];
}

/**
 * @brief  Function for generating the automaton for all the intervals.
 *
 * @tparam LimitType  Datatype of the interval limits.
 *
 * @return  The automaton for all the intervals and a map for identifying the interval from macro reference.  
 */
template <typename LimitType>
std::pair<ap::Automaton, typename Intervals<LimitType>::ElementRefIntervalMap>
Intervals<LimitType>::program(
) const
{
  std::string networkName(std::to_string(B) + "bytes_network");
  // Create ANML workspace.
  ap::Anml anml;
  ap::AnmlNetwork network(anml.createNetwork(networkName));

  // Load comparator macro.
  std::string c = "comparators/" + std::to_string(B) + "bytes_compiled.anml";
  ap::AnmlMacro comparator(anml.loadMacro(c));

  // Get and store reference for all the macro parameters.
  std::unordered_map<unsigned, ap::AnmlMacro::ParamRef> paramRefMap;
  for (size_t p = 1; p <= (4*B)-1; ++p) {
    if ((p == 3) || (p == (4*B)-3)) {
      continue;
    }
    paramRefMap[p] = comparator.getParamFromName("%p" + std::to_string(p));
  }

  for (size_t i = 0; i < m_intervals.size(); ++i) {
    network.addMacroRef(comparator, "comparator_" + std::to_string(i));
  }

  network.exportAnml(networkName + ".anml");

  // Compile the complete automaton for all the intervals.
  std::pair<ap::Automaton, ap::ElementMap> result = anml.compileAnml();
  ap::Automaton automaton(std::move(result.first));
  ap::ElementMap elementMap(std::move(result.second));
  // Print information about the compiled automaton.
  std::cout << "Following are the stats of the compiled automaton:" << std::endl;
  automaton.printInfo();
  std::cout << std::endl;

  // Container for storing element ref to interval index mapping.
  ElementRefIntervalMap macroIntervalMap; 
  // Total number of substitutions needed.
  size_t changeCount = paramRefMap.size() * m_intervals.size();
  // Substitute the symbols for all the comparators.
  ap::SymbolChange changes(changeCount);
  std::array<unsigned char, B> x, y;
  for (size_t i = 0; i < m_intervals.size(); ++i) {
    // Get element reference for the current macro.
    std::string macroName("comparator_" + std::to_string(i));
    ap::ElementRef elementRef = elementMap.getElementRef(networkName + "." + macroName);
    // Reinterpret the limits of the interval as stream of unsigned char bytes.
    reverse_memcpy(&x[0], &m_intervals[i].first, B);
    reverse_memcpy(&y[0], &m_intervals[i].second, B);
    assignLabels<LimitType>(&x[0], &y[0], elementRef, paramRefMap, changes);
    macroIntervalMap.insert(std::make_pair(elementRef, i));
  }
  automaton.setSymbol(elementMap, changes);
  automaton.save(networkName + ".fsm");
  return std::pair<ap::Automaton, ElementRefIntervalMap>(std::move(automaton), macroIntervalMap);
}

/**
 * @brief  Function for checking which intervals are stabbed by the given points. 
 *
 * @tparam LimitType     Datatype of the interval limits.
 * @param  points        Points to be checked.
 * @param  deviceName    Name of the AP device to be used for checking intervals.
 * @param  maxChunkSize  Maximum size of the flow that can be streamed to the AP.
 *
 * @return  A map from point index to index of the intervals which are stabbed by the points.
 */
template <typename LimitType>
std::unordered_map<size_t, std::vector<size_t> >
Intervals<LimitType>::stab(
  const Points<LimitType>& points,
  const std::string& deviceName,
  const size_t maxChunkSize
) const
{
  // Get the automaton for the intervals.
  std::pair<ap::Automaton, ElementRefIntervalMap> automaton(program());

  std::unordered_map<size_t, std::vector<size_t> > stabbedIntervals;

  // Create a byte stream from all the points for streaming to the device.
  std::vector<unsigned char> allPoints(points.count()*B);
  unsigned char* stream = &allPoints[0];
  for (size_t p = 0; p < points.count(); ++p) {
    reverse_memcpy(stream, &points.get(p), B);
  }

  if (!deviceName.empty()) {
    // Open the device.
    ap::Device device(deviceName);
    // Load the automaton on the device.
    device.load(ap::Automaton(automaton.first));

    // Ensure that flow chunks end at number boundaries.
    size_t flowChunkSize = (maxChunkSize / B) * B;
    // Search for all the points and get the results.
    std::vector<std::pair<ap::ElementRef, size_t> > allStabs = device.search(allPoints, flowChunkSize);

    const ElementRefIntervalMap& macroIntervalMap = automaton.second;
    for (const std::pair<ap::ElementRef, size_t>& stab : allStabs) {
      ap::ElementRef macroRef = stab.first;
      size_t pointIndex = stab.second / B;
      size_t intervalIndex = macroIntervalMap.at(macroRef);
      std::unordered_map<size_t, std::vector<size_t> >::iterator it = stabbedIntervals.find(points.get(pointIndex));
      if (it != stabbedIntervals.end()) {
        (it->second).push_back(intervalIndex);
      }
      else {
        stabbedIntervals.insert(std::make_pair(pointIndex, std::vector<size_t>(1, intervalIndex)));
      }
    }
    // Unload the automaton from the device.
    device.unload();
  }
  else {
    std::cerr << "WARNING: AP device name was not provided. Unable to determine stabbed intervals." << std::endl;
  }
  return stabbedIntervals;
}

/**
 * @brief  Default destructor.
 *
 * @tparam LimitType  Datatype of the interval limits.
 */
template <typename LimitType>
Intervals<LimitType>::~Intervals(
)
{
}

// Explicit class instantiation.
template class Intervals<uint32_t>;
template class Intervals<int32_t>;
template class Intervals<uint64_t>;
template class Intervals<int64_t>;
//template class Intervals<float>;
//template class Intervals<double>;

template Intervals<uint32_t>::Intervals(const size_t, std::default_random_engine&);
template Intervals<int32_t>::Intervals(const size_t, std::default_random_engine&);
template Intervals<uint64_t>::Intervals(const size_t, std::default_random_engine&);
template Intervals<int64_t>::Intervals(const size_t, std::default_random_engine&);
//template Intervals<float>::Intervals(const size_t, std::default_random_engine&);
//template Intervals<double>::Intervals(const size_t, std::default_random_engine&);
