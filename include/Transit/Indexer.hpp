
#ifndef TRANSIT_INDEXER_H_
#define TRANSIT_INDEXER_H_

#include "boost/range/iterator_range.hpp"

#include <map>

namespace transit {

template<typename V, typename Comp>
class Indexer : std::map<V, unsigned int, Comp> {
public:
  
  using std::map<V, unsigned int, Comp>::size;

  template<typename It>
  Indexer(It begin, It end) {
    unsigned int i(0);
    for (auto& val: boost::make_iterator_range(begin, end)) {
      emplace(val, i++);
    }
  }

  unsigned int getID(V& val) {
    auto match(find(val));
    if (match == std::map<V, unsigned int, Comp>::end())
      throw std::runtime_error("no ID found for value");
    return match->second;
  }

  V& getChipped(unsigned int id);

};

}

#endif

