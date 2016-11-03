
#ifndef TRANSIT_ANALYSER_H_
#define TRANSIT_ANALYSER_H_

#include <set>

#include "Transit/AST.h"
#include "Transit/Catalog.h"

namespace transit {

class ClangSourceAnalyser {
  Catalog catalog;
public:
  inline Catalog& getCatalog() { return catalog; }
  int analyse(std::vector<std::string> sources, std::vector<const char*> compilerArgs = {});;
};

}

#endif

