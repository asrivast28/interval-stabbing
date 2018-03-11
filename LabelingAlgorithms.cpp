/**
 * @file LabelingAlgorithms.cpp
 * @brief Implementation of helper functions for labeling algorithms.
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
#include "LabelingAlgorithms.hpp"

#include <string>


/**
 * @brief  Function for getting symbol set corresponding to the given limits of a 1-byte interval.
 *
 * @param lower  Lower limit of the interval. Setting the bool flag to true means that the limit is inclusive.
 * @param upper  Upper limit of the interval. Setting the bool flag to true means that the limit is inclusive.
 *
 * @return  The symbol set for the interval.
 */
std::string
getIntervalSymbols(
  const std::pair<unsigned char, bool> lower,
  const std::pair<unsigned char, bool> upper
)
{
  if (((lower.first == 255) && !lower.second) || ((upper.first == 0) && !lower.second)) {
    return "";
  }
  unsigned char x = lower.first + (lower.second ? 0 : 1);
  unsigned char y = upper.first - (upper.second ? 0 : 1);
  if (x > y) {
    return "";
  }
  else if (x == y) {
    return ap::SymbolChange::getSymbolSet(ap::SymbolChange::getHexSymbol(x));
  }
  else {
    return ap::SymbolChange::getSymbolSet(std::make_pair(ap::SymbolChange::getHexSymbol(x), ap::SymbolChange::getHexSymbol(y)));
  }
}

/**
 * @brief  Function for getting symbol set corresponding to multiple limits in the same 1-byte interval.
 *
 * @param  List of lower and upper limits for the interval.
 *
 * @return  The symbol set for the interval.
 */
std::string
getIntervalSymbols(
  const std::vector<std::pair<std::pair<unsigned char, bool>, std::pair<unsigned char, bool> > > intervals
)
{
  std::vector<std::pair<ap::SymbolChange::HexSymbolType, ap::SymbolChange::HexSymbolType> > allSymbols;
  for (const std::pair<std::pair<unsigned char, bool>, std::pair<unsigned char, bool> >& interval : intervals) {
    const std::pair<unsigned char, bool>& lower = interval.first;
    const std::pair<unsigned char, bool>& upper = interval.second;
    if (((lower.first == 255) && !lower.second) || ((upper.first == 0) && !lower.second)) {
      continue;
    }
    unsigned char x = lower.first + (lower.second ? 0 : 1);
    unsigned char y = upper.first - (upper.second ? 0 : 1);
    if (x > y) {
      continue;
    }
    else {
      allSymbols.push_back(std::make_pair(ap::SymbolChange::getHexSymbol(x), ap::SymbolChange::getHexSymbol(y)));
    }
  }
  return ap::SymbolChange::getSymbolSet(allSymbols);
}

//template void assignLabels<float>(const unsigned char* const, const unsigned char* const, const ap::ElementRef&, const std::unordered_map<unsigned, ap::AnmlMacro::ParamRef>&, ap::SymbolChange&);
//template void assignLabels<double>(const unsigned char* const, const unsigned char* const, const ap::ElementRef&, const std::unordered_map<unsigned, ap::AnmlMacro::ParamRef>&, ap::SymbolChange&);
