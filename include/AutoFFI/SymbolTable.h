
#ifndef AUTOFFI_SYMBOL_TABLE_H_
#define AUTOFFI_SYMBOL_TABLE_H_

#include <map>
#include <boost/range/iterator_range.hpp>
#include "AST.h"

namespace autoffi {

/**
 * Provides efficient lookup of exports based on their name.
 */
class SymbolTable {

  std::map<const std::string, Export*> table;

public:

  template<typename Range>
  SymbolTable(Range range) {
    for (auto ex: range) {
      table.emplace(ex->name, ex);
    }
  };

  template<typename It> 
  SymbolTable(It begin, It end):
    SymbolTable(boost::make_iterator_range(begin, end)) {};

  size_t size() const;
  bool hasExport(std::string name) const;
  Export* getExport(std::string name);
  bool removeExport(std::string name);
};

}

#endif

