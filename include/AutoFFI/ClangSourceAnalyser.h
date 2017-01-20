
#ifndef AUTOFFI_ANALYSER_H_
#define AUTOFFI_ANALYSER_H_

#include <set>

#include "AST.h"
#include "Catalog.h"

namespace autoffi {

class ClangSourceAnalyser {
  Catalog catalog;
public:
  inline Catalog& getCatalog() { return catalog; }
  int analyse(std::vector<std::string> sources, std::vector<const char*> compilerArgs = {});;
};

}

#endif

