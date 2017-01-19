
#ifndef TRANSIT_CATALOG_H_
#define TRANSIT_CATALOG_H_

#include <set>
#include <deque>

#include "AST.h"
#include "SymbolTable.h"

namespace transit {

class Catalog {
  std::set<Export*> exports;
  std::deque<Type*> typeSeq;
  std::set<Type*> typeSet;
public:
  using export_range  = boost::iterator_range<std::set<Export*>::iterator>;
  using type_range = boost::iterator_range<std::deque<Type*>::iterator>;
  export_range getExports();
  type_range getTypes();
  void removeDanglingTypes();
  bool addType(Type* type);
  bool addExport(Export* ex);
  bool hasType(Type* type) const;
  bool hasExport(Export* ex) const;
  bool removeType(Type* type);
  bool removeExport(Export* ex);
};

}

#endif

