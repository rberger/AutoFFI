
#include <iostream>

#include "AutoFFI/ClangSourceAnalyser.h"
#include "AutoFFI/CountMap.hpp"
#include "AutoFFI/Format.h"

#include "llvm/Support/CommandLine.h"

using namespace llvm;
using namespace autoffi;

//static cl::opt<std::string> OutFile("o", cl::desc("Location of output file"));

static cl::opt<WriteOptions::Format> OutFormat(cl::desc("Available Optimizations:"),
        cl::values(
             clEnumValN(WriteOptions::Format::JSON, "json", "Transmit JSON"),
             clEnumValN(WriteOptions::Format::PROTOBUF, "protobuf", "Google's protocol buffer format"),
             clEnumValN(WriteOptions::Format::DEBUGSTR, "debug", "Debug output")));
static cl::list<std::string> Files(cl::Positional, cl::desc("Files to parse"), cl::OneOrMore);

int main(int argc, const char* argv[]) {

  auto sep(std::find_if(argv, argv+argc, [](const char* arg) {
    return !strcmp(arg, "--");
  }));

  std::vector<const char*> compilerArgs(sep == argv+argc ? sep : sep+1, argv+argc);

  cl::ParseCommandLineOptions(sep-argv, argv);
  ClangSourceAnalyser a;
  if (int res = a.analyse(Files, compilerArgs))
    return res;

  WriteOptions opts;
  opts.format = OutFormat;
  Format format;
  format.write(a.getCatalog(), std::cout, opts);
}

