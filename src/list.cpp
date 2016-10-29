
#include <iostream>
#include <sstream>
#include <string.h>
#include <vector>

#include "config.h"
#include "clang.h"

#include "llvm/Support/raw_ostream.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/TypeVisitor.h"
#include "clang/AST/DeclVisitor.h"
#include "llvm/Support/Path.h"
#include "llvm/ADT/SmallVector.h"

using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace clang;
using namespace llvm;

static cl::OptionCategory MyToolCategory("bstruct list options");
static cl::opt<bool> ParseSystemHeaders("sys", cl::desc("Include system header types"), cl::cat(MyToolCategory));
static cl::opt<bool> ParseIncludedHeaders("r", cl::desc("Parse included headers"), cl::cat(MyToolCategory));
static cl::list<std::string> Files(cl::Positional, cl::desc("Files to parse"), cl::OneOrMore);
//cl::list<const char*>  CompilerArgList(cl::ConsumeAfter, cl::desc("<compiler arguments> ..."));

int main(int argc, const char* argv[]) {

  auto sep(std::find_if(argv, argv+argc, [](const char* arg) {
    return !strcmp(arg, "--");
  }));

  std::vector<const char*> compilerArgs(sep == argv+argc ? sep : sep+1, argv+argc);

  cl::ParseCommandLineOptions(sep-argv, argv);
  ScanOperation op;
  op.scan(Files, compilerArgs);

  for (auto decl: op.getDecls()) {
    std::cout << decl->getNameAsString() << std::endl;
  }

}

