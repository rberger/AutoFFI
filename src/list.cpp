
#include <iostream>
#include <sstream>

#include "config.h"

#include "llvm/Support/raw_ostream.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/TypeVisitor.h"
#include "clang/AST/DeclVisitor.h"

using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace clang;
using namespace llvm;

static cl::OptionCategory MyToolCategory("bstruct list options");
static cl::opt<bool> ParseSystemHeaders("sys", cl::desc("Include system header types"), cl::cat(MyToolCategory));
static cl::opt<bool> ParseIncludedHeaders("r", cl::desc("Parse included headers"), cl::cat(MyToolCategory));
//static cl::list<std::string> Files(cl::Positional, cl::desc("List of headers to parse"), cl::OneOrMore, cl::cat(MyToolCategory));

struct QualTypeLT {
  bool operator()(const QualType& a, const QualType& b) {
    return a.getTypePtr() < b.getTypePtr()
      && a.getLocalFastQualifiers() < b.getLocalFastQualifiers();
  }
};

DeclarationMatcher MyMatcher = anyOf(
  typedefDecl().bind("typedef"),
  functionDecl().bind("func"),
  recordDecl().bind("record"),
  varDecl(isConstexpr()).bind("var")
);

class TypeExtractor : public ConstDeclVisitor<TypeExtractor, const QualType> {
public:
  const QualType VisitTypeDecl(const TypeDecl* decl) {
    return QualType(decl->getTypeForDecl(), 0);
  }
  const QualType VisitValueDecl(const ValueDecl* decl) {
    return decl->getType();
  }
};

class DependantTypesCollector : public TypeVisitor<DependantTypesCollector> {
  std::set<const QualType, QualTypeLT> types;
  TypeExtractor extractor;
public:
  void Visit(const QualType& type) {
    types.emplace(type);
  }
  void Visit(const FunctionProtoType* type) {
    Visit(type);
    for (auto paramType: type->param_types())
      Visit(paramType);
    types.emplace(QualType(type, 0));
  }
};

//struct DeclSourceLocFilter {
  //SourceManager& sourceManager;
  //DeclSourceLocFilter(SourceManager& mngr): sourceManager(mngr) {}
  //bool operator()(Decl* decl) {
    //FullSourceLoc fullSourceLoc(decl->getLocation(), sourceManager);
    //return fullSourceLoc.isValid() && !fullSourceLoc.isInSystemHeader();
  //}
//};

//class TypeDeclFinder : public TypeVisitor<TypeDeclFinder, Decl*> {
//protected:
//public:
  //Decl* VisitTagType(const TagType* t) {
    //return t->getDecl();
  //}
  //Decl* VisitTypedefType(const TypedefType* t) {
    //return t->getDecl();
  //}
  //Decl* VisitFunctionType(const FunctionType* t) {

  //}
  
//};

class NameCollector : public MatchFinder::MatchCallback {
public:
  std::set<const NamedDecl*> decls;
  NameCollector(std::vector<std::string> headers) {}

  bool isLocationValid(const FullSourceLoc& loc) {
    return loc.isValid() && !loc.isInSystemHeader();
  }

  void run(const MatchFinder::MatchResult &Result) override {

    auto& sourceManager(Result.Context->getSourceManager());

    //const Type* type = Result.Nodes.getNodeAs<Type>("type");
    //if (type) {
      //auto typeDecl(typeDeclFinder.Visit(type));
      //if (typeDecl && sourceLocFilter(typeDecl))
        //types.emplace(type);
    //}

    const NamedDecl* decl = Result.Nodes.getNodeAs<NamedDecl>("record");
      //|| Result.Nodes.getNodeAs<FunctionDecl>("func")
      //|| Result.Nodes.getNodeAs<TypedefDecl>("typedef")
      //|| Result.Nodes.getNodeAs<VarDecl>("var");
    if (decl) {
      FullSourceLoc fullSourceLoc(decl->getLocation(), sourceManager);
      if (isLocationValid(fullSourceLoc)) {
        decls.emplace(decl);
      }
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
  static std::vector<const char*> defaultArgs DEFAULT_ARGS_INITIALIZER;
  std::vector<const char*> args(argv, argv+argc);
  args.push_back("--");
  std::copy(defaultArgs.begin(), defaultArgs.end(), std::back_inserter(args));
  //std::find(args.begin(), args.end(), "--");

  int newArgc(args.size());
  CommonOptionsParser op(newArgc, args.data(), MyToolCategory);
  auto& db(op.getCompilations());
  auto headers(op.getSourcePathList());
  ClangTool tool(db, headers);

  NameCollector nameCollector(headers);
  MatchFinder Finder;
  Finder.addMatcher(MyMatcher, &nameCollector);

  if (const int res = tool.run(newFrontendActionFactory(&Finder).get())) {
    //llvm::errs() << "type analyser failed to run properly\n";
    std::cerr << "bstruct was unable to analyse source file" << std::endl;
    return res;
  }

  DependantTypesCollector depColl;
  TypeExtractor e;

  for (auto decl: nameCollector.decls) {
    //decl->dump()
    depColl.Visit(e.Visit(decl));
    std::cout << decl->getNameAsString() << std::endl;
  }

}

