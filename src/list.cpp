
#include <iostream>
#include <sstream>

#include "config.h"

#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace clang;
using namespace llvm;

static llvm::cl::OptionCategory MyToolCategory("bstruct list options");
static cl::opt<bool> ParseSystemHeaders("sys", cl::desc("Include system header types"), cl::cat(MyToolCategory));
static cl::opt<bool> ParseIncludedHeaders("r", cl::desc("Parse included headers"), cl::cat(MyToolCategory));
static cl::list<std::string> Files(cl::Positional, cl::desc("List of headers to parse"), cl::Required, cl::cat(MyToolCategory));

class TypeCollector : public MatchFinder::MatchCallback {
public:
  std::set<const Type*> types;
  TypeCollector(std::vector<std::string> headers) {}
  virtual void run(const MatchFinder::MatchResult &Result) {
    const Type* decl = Result.Nodes.getNodeAs<Type>("type");
    if (decl) {
      types.emplace(decl);
    }
    //const RecordDecl * decl = Result.Nodes.getNodeAs<RecordDecl>("struct");
    //if (decl) { //&& !decl->isImplicit()) {
      //FullSourceLoc fullSourceLoc(decl->getLocation(), Result.Context->getSourceManager());
      //if (fullSourceLoc.isValid() && !fullSourceLoc.isInSystemHeader()) {
        //std::cout << decl->getNameAsString() << std::endl;
        //types.emplace(decl->getTypeForDecl());
      //}
    //}
  }
};

int main(int argc, const char* argv[]) {
  static std::vector<const char*> defaultArgs DEFAULT_ARGS_INITIALIZER ;
  std::vector<const char*> args(argv, argv+argc);
  std::copy(defaultArgs.begin(), defaultArgs.end(), std::back_inserter(args));

  std::cout << defaultArgs.size() << std::endl; 
  //args.push_back("-isystem/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include");
  //args.push_back("-isystem/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/4.2.1");
  //args.push_back("-isystem/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/8.0.0/include");
  //args.push_back("-isystem/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/4.2.1/tr1");



  int newArgc(args.size());
  CommonOptionsParser op(newArgc, args.data(), MyToolCategory);
  auto& db(op.getCompilations());
  auto headers(op.getSourcePathList());
  ClangTool tool(db, headers);

  //auto fileFilter(isExpansionInFileMatching(Files));
  DeclarationMatcher VarMatcher = varDecl(isConstexpr()).bind("var");
  //DeclarationMatcher StructMatcher = recordDecl().bind("struct");
  TypeMatcher TypeMatcher = type().bind("type");

  TypeCollector collector(headers);
  MatchFinder Finder;
  Finder.addMatcher(VarMatcher, &collector);
  Finder.addMatcher(TypeMatcher, &collector);

  if (const int res = tool.run(newFrontendActionFactory(&Finder).get())) {
    //llvm::err() << "type analyser failed to run properly" << std::endl;
    return res;
  }

  for (auto type: collector.types) {
    std::cout << type << std::endl;
  }
}

