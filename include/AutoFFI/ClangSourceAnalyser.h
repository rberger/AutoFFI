
#ifndef AUTOFFI_ANALYSER_H_
#define AUTOFFI_ANALYSER_H_

#include <set>

#include "AST.h"
#include "Catalog.h"

namespace autoffi {

enum SymbolVisibility {
  PUBLIC = 0, 
  PROTECTED = 1,
  HIDDEN = 2,
};

struct FilterOptions {
  std::vector<std::string> includes;
  std::vector<std::string> excludes;
  SymbolVisibility visibilityLevel = PUBLIC;
};

struct AnalyzeOptions {
  std::string exePath;
  FilterOptions filter;
  std::vector<std::string> compilerArgs;
};

class ClangSourceAnalyser {
  Catalog catalog;
public:
  inline Catalog& getCatalog() { return catalog; }
  int analyse(const AnalyzeOptions& opts);
};

}

#endif

