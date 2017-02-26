
#include <iostream>
#include <list>

#include <boost/program_options.hpp>

#include "AutoFFI/ClangSourceAnalyser.h"
#include "AutoFFI/CountMap.hpp"
#include "AutoFFI/Format.h"

//#include "llvm/Support/CommandLine.h"

//using namespace llvm;
using namespace autoffi;
using namespace boost::program_options;

//static cl::opt<std::string> OutFile("o", cl::desc("Location of output file"));

//static cl::opt<WriteOptions::Format> OutFormat(cl::desc("Available Optimizations:"),
        //cl::values(
           //clEnumValN(WriteOptions::Format::JSON, "json", "Transmit JSON"),
           //clEnumValN(WriteOptions::Format::PROTOBUF, "protobuf", "Google's protocol buffer format"),
           //clEnumValN(WriteOptions::Format::DEBUGSTR, "debug", "Debug output")
       //));
//static cl::list<std::string> Files(cl::Positional, cl::desc("Files to parse"), cl::OneOrMore);

int main(int argc, const char* argv[]) {

  try {
    options_description desc("Options");   
    desc.add_options()
      ("include",value<std::vector<std::string>>()->multitoken()->zero_tokens(), "Include exported members from the given file")
      ("exclude",value<std::vector<std::string>>()->multitoken()->zero_tokens(), "Exclude exported members from the given file");
    variables_map vm;
    parsed_options parsed = command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    store(parsed, vm);
    notify(vm);
    std::vector<std::string> compilerArgs = collect_unrecognized(parsed.options, include_positional);

    auto& includes (vm["include"].as<std::vector<std::string>>());
    auto& excludes (vm["exclude"].as<std::vector<std::string>>());
    AnalyzeOptions analyzeOpts;
    analyzeOpts.exePath = argv[0];
    analyzeOpts.compilerArgs = compilerArgs;
    analyzeOpts.filter.includes = includes;
    analyzeOpts.filter.excludes = excludes;
    ClangSourceAnalyser a;
    if (int res = a.analyse(analyzeOpts))
      return res;
  
    WriteOptions writeOpts;
    writeOpts.format = WriteOptions::Format::JSON;
    Format format;
    format.write(a.getCatalog(), std::cout, writeOpts);

  } catch(std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
  }
  //cl::ParseCommandLineOptions(sep-argv, argv);
}

