
#ifndef TRANSIT_INDEXER_H_
#define TRANSIT_INDEXER_H_

#include "boost/range/iterator_range.hpp"

#include <map>
#include <sstream>

namespace transit {

template<typename V>
std::string print(V val) {
  std::ostringstream ss;
  ss << val;
  return ss.str();
}

/**
 * Assigns an unique identifier to each element in the container.
 */
template<typename V, typename Comp = std::less<V>>
class Indexer : std::map<V, unsigned int, Comp> {
public:
  
  using std::map<V, unsigned int, Comp>::size;

  template<typename It>
  Indexer(It begin, It end): Indexer(boost::make_iterator_range(begin, end)) {}

  template<typename Range>
  Indexer(Range range) {
    unsigned int i(1);
    for (auto& val: range) {
      std::map<V, unsigned int, Comp>::emplace(val, i++);
    }
  }

  unsigned int getID(V val) {
    auto match(std::map<V, unsigned int, Comp>::find(val));
    if (match == std::map<V, unsigned int, Comp>::end())
      throw std::runtime_error("no ID found for value " + val->toString());
    return match->second;
  }

};

}

#endif

