
#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <map>
#include <boost/range/iterator_range.hpp>
#include "AST.h"

namespace transit {

/**
 * Provides efficient lookup of exports based on their name.
 */
class SymbolTable {

  std::map<const std::string, const Export*> table;

public:

  template<typename Range>
  SymbolTable(Range range) {
    for (auto ex: range) {
      table.emplace(ex->name, ex);
    }
  };

  size_t size() {
    return table.size();
  }

  template<typename It> 
  SymbolTable(It begin, It end):
    SymbolTable(boost::make_iterator_range(begin, end)) {};

  bool hasExport(std::string name) {
    return table.count(name) > 0;
  };

  const Export* getExport(std::string name) {
    auto match(table.find(name));
    if (match == table.end())
      throw std::runtime_error("could not find export for symbol " + name);
    return match->second;
  }  

};

}

#endif

