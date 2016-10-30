
#ifndef TRANSIT_ANALYSER_H_
#define TRANSIT_ANALYSER_H_

#include <set>

#include "AST.h"

namespace transit {

class Analyser {
public:
  using ExportSet = std::set<const Export*>;
  using TypeSet = std::set<const Type*>;
protected:
  ExportSet exports;
  TypeSet types;
public:
  inline ExportSet& getExports() { return exports; };
  inline TypeSet& getTypes() { return types; };

  /**
   * Indexes the currently registered types in the analyser.
   *
   * The identifications of types are not guaranteed to be the same
   * between two distinct indexing operations.
   */
  void indexTypes();
  /**
   * Indexes the currently registered exports in the analyser.
   *
   * The identifications of exports are not guaranteed to be the same
   * between two distinct indexing operations.
   */
  void indexExports();

  unsigned int getTypeID(Type* type);
  unsigned int getExportID(Export* ex);

  int scan(std::vector<std::string> sources, std::vector<const char*> compilerArgs = {});
};

}

#endif

