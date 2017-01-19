
#include "Transit/SymbolTable.h"

namespace transit {

size_t SymbolTable::size() const {
  return table.size();
}

bool SymbolTable::hasExport(std::string name) const {
  return table.count(name) > 0;
}

Export* SymbolTable::getExport(std::string name) {
  auto match(table.find(name));
  if (match == table.end())
    throw std::runtime_error("could not find export for symbol " + name);
  return match->second;
}

bool SymbolTable::removeExport(std::string name) {
  return table.erase(name) > 0;
}

}

