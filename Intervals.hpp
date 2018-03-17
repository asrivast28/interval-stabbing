/**
 * @file Intervals.hpp
 * @brief Declaration of Intervals functions.
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
#ifndef INTERVALS_HPP_
#define INTERVALS_HPP_

#include "apsdk/Automaton.hpp"
#include "Points.hpp"

#include <string>
#include <unordered_map>
#include <vector>


/**
 * @brief  Class for storing intervals and determining stabs.
 *
 * @tparam LimitType  Datatype of the limits of the intervals.
 */
template <typename LimitType>
class Intervals {
public:
  Intervals();

  Intervals(const std::string&);

  template <typename RandomNumberGenerator>
  Intervals(const size_t, RandomNumberGenerator&);

  const std::pair<LimitType, LimitType>&
  get(const size_t) const;

  std::unordered_map<size_t, std::vector<size_t> >
  stab(const Points<LimitType>&, const std::string&, const std::string&, const std::string&, const size_t) const;

  ~Intervals();

private:
  static const size_t B = sizeof(LimitType);

private:
  typedef std::unordered_map<ap::ElementRef, size_t, ap::ElementRefHasher> ElementRefIntervalMap;

private:
  std::pair<ap::Automaton, ElementRefIntervalMap>
  program(const std::string&, const std::string&) const;

private:
  std::vector<std::pair<LimitType, LimitType> > m_intervals;
};

#endif // INTERVALS_HPP_
