
#include <iostream> // for debugging only

#include "config.h"
#include "clang.h"

#include "llvm/Support/raw_ostream.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/TypeVisitor.h"
#include "clang/AST/DeclVisitor.h"
#include "llvm/Support/Path.h"
#include "llvm/ADT/SmallVector.h"
#include "boost/iterator/filter_iterator.hpp"

using namespace clang::tooling;
using namespace clang::ast_matchers;

DeclarationMatcher MyMatcher = anyOf(
  typedefDecl().bind("ndecl"),
  functionDecl().bind("ndecl"),
  recordDecl().bind("ndecl"),
  enumDecl().bind("ndecl"),
  varDecl(isConstexpr()).bind("ndecl")
);

bool QualTypeLT::operator()(const QualType& a, const QualType& b) const {
  return a.getTypePtr() < b.getTypePtr()
    || a.getLocalFastQualifiers() < b.getLocalFastQualifiers();
}

unsigned int QualTypeIDMapper::getID(const QualType& val) {
  auto match(find(val));
  if (match == end())
    throw std::runtime_error("no ID found for value");
  return match->second;
}

const QualType getTypeForNamedDecl(const NamedDecl* decl) {
  static class DeclTypeExtractor : public ConstDeclVisitor<DeclTypeExtractor, const QualType> {
  public:
    const QualType VisitTypeDecl(const TypeDecl* decl) {
      return QualType(decl->getTypeForDecl(), 0);
    }
    const QualType VisitValueDecl(const ValueDecl* decl) {
      return decl->getType();
    }
    const QualType VisitNamedDecl(const NamedDecl* decl) {
      throw std::runtime_error("could not convert types");
    }
  } declTypeExtractor;
  return declTypeExtractor.Visit(decl);
}

TypeSet& TypesCollector::getTypes() { 
  return types;
}

void TypesCollector::VisitBuiltinType(const BuiltinType* type) {
  add(QualType(type, 0));
}

void TypesCollector::VisitQualType(const QualType& type) {
  add(type);
  if (!type.isNull())
    Visit(type.getTypePtr());
}

void TypesCollector::VisitTypedefType(const TypedefType* type) {
  VisitQualType(type->desugar());
}

void TypesCollector::VisitType(const Type* type) {
  std::cerr << "skipping the following type:" << std::endl;
  type->dump();
}

void TypesCollector::VisitRecordType(const RecordType* type) {
  add(QualType(type, 0));
  for (auto field: type->getDecl()->fields()) {
    VisitQualType(field->getType());
  }
}

void TypesCollector::VisitFunctionProtoType(const FunctionProtoType* type) {
  add(QualType(type, 0));
  for (auto paramType: type->param_types())
    VisitQualType(paramType);
}

class NameCollector : public MatchFinder::MatchCallback {
public:
  NamedDeclSet& decls;
  std::vector<std::string>& headers;
  NameCollector(std::vector<std::string>& headers, NamedDeclSet& decls): headers(headers), decls(decls) {}

  static bool headerMatches(std::string pattern, std::string header) {
    return pattern == header;
  }

  bool isLocationValid(const FullSourceLoc& loc) {
    if (!loc.isValid() || loc.isInSystemHeader())
      return false;
    auto& mngr(loc.getManager());
    for (auto header: headers) {
      if (headerMatches(header, mngr.getFilename(loc)))
        return true;
    }
    return false;
  }

  void run(const MatchFinder::MatchResult &Result) override {
    auto& sourceManager(Result.Context->getSourceManager());
    const NamedDecl* decl = Result.Nodes.getNodeAs<NamedDecl>("ndecl");
    if (decl) { 
      auto type(getTypeForNamedDecl(decl));
      if (!type.isNull() && !decl->isImplicit()) {
        FullSourceLoc fullSourceLoc(decl->getLocation(), sourceManager);
        if (isLocationValid(fullSourceLoc)) {
          decls.emplace(decl);
        }
      }
    }
  }

};

boost::iterator_range<NamedDeclSet::iterator> ScanOperation::getDecls() {
  return boost::make_iterator_range(decls.begin(), decls.end());
}

int ScanOperation::scan(std::vector<std::string> headers, std::vector<const char*> compilerArgs) {

  static std::vector<const char*> defaultArgs DEFAULT_ARGS_INITIALIZER;
  compilerArgs.insert(compilerArgs.begin(), "foo");
  compilerArgs.insert(compilerArgs.begin(), "--");
  std::copy(defaultArgs.begin(), defaultArgs.end(), std::back_inserter(compilerArgs));
  
  int compilerArgsCount(compilerArgs.size());
  auto db(FixedCompilationDatabase::loadFromCommandLine(compilerArgsCount, compilerArgs.data()));

  // resolve headers to their absolute path
  std::vector<std::string> absoluteHeaders;
  for (auto& header: headers) {
    SmallString<1024> res(header);
    llvm::sys::fs::make_absolute(res);
    absoluteHeaders.push_back(res.str());
  }

  ClangTool tool(*db, headers);

  NameCollector nameCollector(absoluteHeaders, decls);
  MatchFinder Finder;
  Finder.addMatcher(MyMatcher, &nameCollector);

  int res(tool.run(newFrontendActionFactory(&Finder).get()));
  delete db;

  return res;
};

