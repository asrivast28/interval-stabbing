/**
 * @file Points.hpp
 * @brief Declaration of Points functions.
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
