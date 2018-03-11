/**
 * @file Points.hpp
 * @brief Declaration of Points functions.
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
#ifndef POINTS_HPP_
#define POINTS_HPP_

#include <string>
#include <vector>


/**
 * @brief  Container class for points to be checked against intervals. 
 *
 * @tparam PointType  Datatype of the points.
 */
template <typename PointType>
class Points {
public:
  Points();

  Points(const std::string&);

  template <typename RandomNumberGenerator>
  Points(const size_t, RandomNumberGenerator&); 

  const PointType&
  get(const size_t) const;

  size_t
  count() const;

  ~Points();

private:
  std::vector<PointType> m_points;
};

#endif // POINTS_HPP_ 
