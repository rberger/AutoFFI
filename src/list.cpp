
#include <iostream>
#include <sstream>
#include <string.h>
#include <vector>

#include "AutoFFI/Analyser.h"

//#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"

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
  transit::Analyser a;
  if (int res = a.scan(Files, compilerArgs))
    return res;

  for (auto ex: a.getExports()) {
    std::cout << ex->name << std::endl;
  }

  return 0;
}

