/**
 * @file Points.cpp
 * @brief Implementation of Points functions.
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
#include "Points.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>


/**
 * @brief  Default constructor for empty initialization.
 *
 * @tparam PointType  Datatype of the points.
 */
template <typename PointType>
Points<PointType>::Points(
) : m_points()
{
}

/**
 * @brief  Constructor for reading the points from the given file.
 *
 * @tparam PointType   Datatype of the points.
 * @param  pointsFile  Name of the file from which points are to be read.
 */
template <typename PointType>
Points<PointType>::Points(
  const std::string& pointsFile
) : m_points()
{
  std::ifstream points(pointsFile);
  std::string line;
  while (std::getline(points, line)) {
    std::istringstream is(line);
    PointType z;
    is >> z;
    m_points.push_back(z);
  }
}

/**
 * @brief  Constructor for generating random points.
 *
 * @tparam PointType              Datatype of the points.
 * @tparam RandomNumberGenerator  Type of the random number generator. 
 * @param  numRandom              Number of random points to be generated.
 * @param  generator              Instance of the random number generator. 
 */
template <typename PointType>
template <typename RandomNumberGenerator>
Points<PointType>::Points(
  const size_t,
  RandomNumberGenerator&
) : m_points()
{
  throw std::runtime_error("Random generation of points hasn't been implemented for the datatype.");
}

// Specialization of the random point generator constructor for integer datatypes.
#define INITIALIZE_INTEGER(IntegerType) \
template <> \
template <typename RandomNumberGenerator> \
Points<IntegerType>::Points( \
  const size_t numRandom, \
  RandomNumberGenerator& generator \
) : m_points() \
{ \
  IntegerType lower = std::numeric_limits<IntegerType>::min(); \
  IntegerType upper = std::numeric_limits<IntegerType>::max(); \
  std::uniform_int_distribution<IntegerType> distribution(lower, upper); \
  std::cout << "Following are the randomly generated points:" << std::endl; \
  for (size_t i = 0; i < numRandom; ++i) { \
    IntegerType z = distribution(generator); \
    std::cout << z << std::endl; \
    m_points.push_back(z); \
  } \
  std::cout << std::endl; \
}

INITIALIZE_INTEGER(uint32_t);
INITIALIZE_INTEGER(int32_t);
INITIALIZE_INTEGER(uint64_t);
INITIALIZE_INTEGER(int64_t);

// Specialization of the random point generator constructor for real datatypes.
#define INITIALIZE_REAL(RealType) \
template <> \
template <typename RandomNumberGenerator> \
Points<RealType>::Points( \
  const size_t numRandom, \
  RandomNumberGenerator& generator \
) : m_points() \
{ \
  RealType lower = std::numeric_limits<RealType>::min(); \
  RealType upper = std::numeric_limits<RealType>::max(); \
  std::default_random_engine generator(seed); \
  std::uniform_real_distribution<RealType> distribution(lower, upper); \
  std::cout << "Following are the randomly generated points:" << std::endl; \
  for (size_t i = 0; i < numRandom; ++i) { \
    RealType z = distribution(generator); \
    std::cout << z << std::endl; \
    m_points.push_back(z); \
  } \
  std::cout << std::endl; \
}

//INITIALIZE_REAL(float);
//INITIALIZE_REAL(double);

/**
 * @brief  Function for accessing the point at a given index.
 *
 * @tparam PointType  Datatype of the points.
 * @param  index      Index of the point to be accessed. 
 *
 * @return  A const reference to the point at the given index.
 */
template <typename PointType>
const PointType&
Points<PointType>::get(
  const size_t index
) const
{
  return m_points[index];
}

/**
 * @brief  Function for getting the number of points in the container. 
 *
 * @tparam PointType  Datatype of the points.
 *
 * @return  The size of the internal points congtainer. 
 */
template <typename PointType>
size_t
Points<PointType>::count(
) const
{
  return m_points.size();
}

/**
 * @brief  Default destructor.
 *
 * @tparam LimitType  Datatype of the points.
 */
template <typename LimitType>
Points<LimitType>::~Points(
)
{
}

// Explicit class instantiation.
template class Points<uint32_t>;
template class Points<int32_t>;
template class Points<uint64_t>;
template class Points<int64_t>;
template class Points<float>;
template class Points<double>;

template Points<uint32_t>::Points(const size_t, std::default_random_engine&);
template Points<int32_t>::Points(const size_t, std::default_random_engine&);
template Points<uint64_t>::Points(const size_t, std::default_random_engine&);
template Points<int64_t>::Points(const size_t, std::default_random_engine&);
template Points<float>::Points(const size_t, std::default_random_engine&);
template Points<double>::Points(const size_t, std::default_random_engine&);
