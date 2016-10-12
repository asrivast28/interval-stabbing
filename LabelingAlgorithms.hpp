/**
 * @file LabelingAlgorithms.hpp
 * @brief Implementation of labeling algorithms. Also contains declarations of a few helper functions.
 */
#ifndef LABELINGALGORITHMS_HPP_
#define LABELINGALGORITHMS_HPP_

#include "apsdk/SymbolChange.hpp"

#include <unordered_map>


std::string
getIntervalSymbols(const std::pair<unsigned char, bool>, const std::pair<unsigned char, bool>);

std::string
getIntervalSymbols(const std::vector<std::pair<std::pair<unsigned char, bool>, std::pair<unsigned char, bool> > >);


/**
 * @brief  Function for adding label changes for the given unsigned integer interval.
 *
 * @tparam LimitType  Datatype of the interval limits.
 * @param x           Byte representation of the lower limit of the interval.
 * @param y           Byte representation of the Upper limit of the interval.
 * @param elementRef  Reference of the macro element on which the interval is to be programmed.
 * @param paramRefMap A map from the index of the parameter to the corresponding reference.
 * @param changes     Changes to be made in the automaton.
 */
template <typename LimitType>
typename std::enable_if<std::is_unsigned<LimitType>::value && std::is_integral<LimitType>::value, void>::type
assignLabels(
  const unsigned char* const x,
  const unsigned char* const y,
  const ap::ElementRef& elementRef,
  const std::unordered_map<unsigned, ap::AnmlMacro::ParamRef>& paramRefMap,
  ap::SymbolChange& changes
)
{
  const size_t B = sizeof(LimitType);
  changes.add(elementRef, paramRefMap.at(2), getIntervalSymbols(std::make_pair(x[0], false), std::make_pair(y[0], false)));

  bool equalPrefix = true;
  for (unsigned i = 1; i < (B-1); ++i) {
    changes.add(elementRef, paramRefMap.at(4*(i-1)+1), ap::SymbolChange::getSymbolSet(ap::SymbolChange::getHexSymbol(x[i-1])));
    changes.add(elementRef, paramRefMap.at(4*(i-1)+4), ap::SymbolChange::getSymbolSet(ap::SymbolChange::getHexSymbol(y[i-1])));
    if (x[i-1] != y[i-1]) {
      equalPrefix = false;
    }
    if (equalPrefix) {
      changes.add(elementRef, paramRefMap.at(4*i+2), getIntervalSymbols(std::make_pair(x[i], false), std::make_pair(y[i], false)));
      changes.add(elementRef, paramRefMap.at(4*i+3), getIntervalSymbols(std::make_pair(x[i], false), std::make_pair(y[i], false)));
    }
    else {
      changes.add(elementRef, paramRefMap.at(4*i+2), getIntervalSymbols(std::make_pair(x[i], false), std::make_pair(255, true)));
      changes.add(elementRef, paramRefMap.at(4*i+3), getIntervalSymbols(std::make_pair(0, true), std::make_pair(y[i], false)));
    }
  }
  changes.add(elementRef, paramRefMap.at(4*(B-2)+1), ap::SymbolChange::getSymbolSet(ap::SymbolChange::getHexSymbol(x[B-2])));
  changes.add(elementRef, paramRefMap.at(4*(B-2)+4), ap::SymbolChange::getSymbolSet(ap::SymbolChange::getHexSymbol(y[B-2])));
  if (equalPrefix) {
    changes.add(elementRef, paramRefMap.at(4*(B-1)+2), getIntervalSymbols(std::make_pair(x[B-1], true), std::make_pair(y[B-1], true)));
    changes.add(elementRef, paramRefMap.at(4*(B-1)+3), getIntervalSymbols(std::make_pair(x[B-1], true), std::make_pair(y[B-1], true)));
  }
  else {
    changes.add(elementRef, paramRefMap.at(4*(B-1)+2), getIntervalSymbols(std::make_pair(x[B-1], true), std::make_pair(255, true)));
    changes.add(elementRef, paramRefMap.at(4*(B-1)+3), getIntervalSymbols(std::make_pair(0, true), std::make_pair(y[B-1], true)));
  }
}

/**
 * @brief  Function for adding label changes for the given signed integer interval.
 *
 * @tparam LimitType  Datatype of the interval limits.
 * @param x           Byte representation of the lower limit of the interval.
 * @param y           Byte representation of the Upper limit of the interval.
 * @param elementRef  Reference of the macro element on which the interval is to be programmed.
 * @param paramRefMap A map from the index of the parameter to the corresponding reference.
 * @param changes     Changes to be made in the automaton.
 */
template <typename LimitType>
typename std::enable_if<std::is_signed<LimitType>::value && std::is_integral<LimitType>::value, void>::type
assignLabels(
  const unsigned char* const x,
  const unsigned char* const y,
  const ap::ElementRef& elementRef,
  const std::unordered_map<unsigned, ap::AnmlMacro::ParamRef>& paramRefMap,
  ap::SymbolChange& changes
)
{
  const size_t B = sizeof(LimitType);
  if (((x[0] <= 127) && (y[0] <= 127)) || ((x[0] > 127) && (y[0] > 127))) {
    assignLabels<typename std::make_unsigned<LimitType>::type>(x, y, elementRef, paramRefMap, changes);
  }
  std::vector<std::pair<std::pair<unsigned char, bool>, std::pair<unsigned char, bool> > > intervals;
  intervals.push_back(std::make_pair(std::make_pair(x[0], false), std::make_pair(255, true)));
  intervals.push_back(std::make_pair(std::make_pair(0, true), std::make_pair(y[0], false)));
  changes.add(elementRef, paramRefMap.at(2), getIntervalSymbols(intervals));

  for (unsigned i = 1; i < (B-1); ++i) {
    changes.add(elementRef, paramRefMap.at(4*(i-1)+1), ap::SymbolChange::getSymbolSet(ap::SymbolChange::getHexSymbol(x[i-1])));
    changes.add(elementRef, paramRefMap.at(4*(i-1)+4), ap::SymbolChange::getSymbolSet(ap::SymbolChange::getHexSymbol(y[i-1])));
    changes.add(elementRef, paramRefMap.at(4*i+2), getIntervalSymbols(std::make_pair(x[i], false), std::make_pair(255, true)));
    changes.add(elementRef, paramRefMap.at(4*i+3), getIntervalSymbols(std::make_pair(0, true), std::make_pair(y[i], false)));
  }
  changes.add(elementRef, paramRefMap.at(4*(B-2)+1), ap::SymbolChange::getSymbolSet(ap::SymbolChange::getHexSymbol(x[B-2])));
  changes.add(elementRef, paramRefMap.at(4*(B-2)+4), ap::SymbolChange::getSymbolSet(ap::SymbolChange::getHexSymbol(y[B-2])));
  changes.add(elementRef, paramRefMap.at(4*(B-1)+2), getIntervalSymbols(std::make_pair(x[B-1], true), std::make_pair(255, true)));
  changes.add(elementRef, paramRefMap.at(4*(B-1)+3), getIntervalSymbols(std::make_pair(0, true), std::make_pair(y[B-1], true)));
}

#endif // LABELINGALGORITHMS_HPP_
