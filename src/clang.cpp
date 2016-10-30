
#include <iostream> // for debugging only

#include "config.h"
#include "Transit/AST.h"
#include "Transit/Analyser.h"

#include "clang/AST/Decl.h"
#include "clang/AST/Type.h"
#include "clang/AST/TypeVisitor.h"
#include "clang/AST/DeclVisitor.h"
#include "boost/range.hpp"

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

#include "boost/range.hpp"
#include "boost/range/algorithm/copy.hpp"
#include "boost/range/adaptor/map.hpp"
//#include "boost/iterator/filter_iterator.hpp"

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

DeclarationMatcher MyMatcher = anyOf(
  typedefDecl().bind("ndecl"),
  functionDecl().bind("ndecl"),
  recordDecl().bind("ndecl"),
  enumDecl().bind("ndecl"),
  varDecl(isConstexpr()).bind("ndecl")
);

struct QualTypeLT {
  bool operator()(const QualType& a, const QualType& b) const {
    return a.getTypePtr() < b.getTypePtr()
      || a.getLocalFastQualifiers() < b.getLocalFastQualifiers();
  } 
};

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

//using NamedDeclSet = std::set<const NamedDecl*>;
//using TypeSet = std::set<const QualType, QualTypeLT>;

//class TypesCollector : public TypeVisitor<TypesCollector> {
//protected:
  //TypeSet types;
//public:
  //TypeSet& getTypes() { return types; };
  //inline void add(const QualType& q) { types.emplace(q); };
  //void VisitType(const Type* type);
  //void VisitBuiltinType(const BuiltinType* type);
  //void VisitQualType(const QualType& type);
  //void VisitTypedefType(const TypedefType* type);
  //void VisitFunctionProtoType(const FunctionProtoType* type);
  //void VisitRecordType(const RecordType* type);
//};

//void TypesCollector::VisitBuiltinType(const BuiltinType* type) {
  //add(QualType(type, 0));
//}

//void TypesCollector::VisitQualType(const QualType& type) {
  //add(type);
  //if (!type.isNull())
    //Visit(type.getTypePtr());
//}

//void TypesCollector::VisitTypedefType(const TypedefType* type) {
  //VisitQualType(type->desugar());
//}

//void TypesCollector::VisitType(const Type* type) {
  //std::cerr << "skipping the following type:" << std::endl;
  //type->dump();
//}

//void TypesCollector::VisitRecordType(const RecordType* type) {
  //add(QualType(type, 0));
  //for (auto field: type->getDecl()->fields()) {
    //VisitQualType(field->getType());
  //}
//}

//void TypesCollector::VisitFunctionProtoType(const FunctionProtoType* type) {
  //add(QualType(type, 0));
  //for (auto paramType: type->param_types())
    //VisitQualType(paramType);
//}

/**
 * Converts an arbitrary Clang type to a Truss type.
 */
struct TypeConverter : public clang::TypeVisitor<TypeConverter, transit::Type*> {

  std::map<const QualType, transit::Type*, QualTypeLT> types;

  transit::Type* VisitQualType(const QualType& qt) {
    if (qt.isNull()) // only allow complete types
      return NULL;
    auto match(types.find(qt));
    if (match != types.end())
      return match->second;
    auto converted = new transit::QualType(Visit(qt.getTypePtr()));
    converted->setConst(qt.isConstQualified());
    converted->setVolatile(qt.isVolatileQualified());
    types.emplace(qt, converted);
    return converted;
  }

  transit::Type* VisitEnumType(const EnumType* type) {
    auto match(types.find(QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto decl(type->getDecl());
    auto converted = new transit::EnumType;
    for (auto value: decl->enumerators())
      converted->addValue(value->getNameAsString(), value->getInitVal().getLimitedValue());
    types.emplace(QualType(type, 0), converted);
    return converted;
  }

  transit::Type* VisitRecordType(const clang::RecordType* type) {
    auto match(types.find(QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto decl(type->getDecl());
    auto converted = new transit::RecordType;
    for (auto field: decl->fields())
      converted->addField(field->getNameAsString(), VisitQualType(field->getType()));
    types.emplace(QualType(type, 0), converted);
    return converted;
  }

  transit::Type* VisitFunctionProtoType(const FunctionProtoType* type) {
    auto match(types.find(QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto converted = new transit::FunctionType;
    // TODO: implement me
    types.emplace(QualType(type, 0), converted);
    return converted;
  }

};

struct ValueConverter {};

/**
 * Converts a named declaration to a Truss symbol export.
 */
struct NamedDeclConverter : public ConstDeclVisitor<NamedDeclConverter, transit::Export*> {

  TypeConverter typeConverter;
  ValueConverter valueConverter;

  transit::Export* VisitEnumDecl(const EnumDecl* decl) {
    auto e = new transit::Export;
    e->name = decl->getNameAsString();
    e->type = typeConverter.Visit(decl->getTypeForDecl());
    e->value = NULL;
    return e;
  }

  // TODO: remove me?
  transit::Export* VisitDecl(const Decl* decl) {
    decl->dump();
    throw std::runtime_error("ran into a declaration that should not be matched");
  };

  transit::Export* VisitDecl(const TypedefDecl* decl) {
    auto e = new transit::Export;
    e->name = decl->getNameAsString();
    e->type = typeConverter.Visit(decl->getTypeForDecl());
    return e;
  }

  transit::Export* VisitFunctionDecl(const FunctionDecl* decl) {
    auto e = new transit::Export;
    e->name = decl->getNameAsString();
    e->type = typeConverter.VisitQualType(decl->getType());
    return e;
  };

  transit::Export* VisitVarDecl(const VarDecl* decl) {
    auto e = new transit::Export;
    e->name = decl->getNameAsString();
    e->type = typeConverter.VisitQualType(decl->getType());
    //e->value = new transit::PrimitiveValue;
    return e;
  }

  transit::Export* VisitRecordDecl(const RecordDecl* decl) {
    auto e = new transit::Export;
    e->name = decl->getNameAsString();
    e->type = typeConverter.VisitType(decl->getTypeForDecl());
    return e;
  }

};

class NameCollector : public MatchFinder::MatchCallback {
public:
  std::set<const NamedDecl*>& decls;
  std::vector<std::string>& headers;
  NameCollector(std::vector<std::string>& headers, std::set<const NamedDecl*>& decls): headers(headers), decls(decls) {}

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

int transit::Analyser::scan(std::vector<std::string> headers, std::vector<const char*> compilerArgs) {

  // do some magic to get the correct compiler args
  static std::vector<const char*> defaultArgs DEFAULT_ARGS_INITIALIZER;
  compilerArgs.insert(compilerArgs.begin(), "--");
  std::copy(defaultArgs.begin(), defaultArgs.end(), std::back_inserter(compilerArgs));
  int compilerArgsCount(compilerArgs.size());

  // initialize compiler database
  auto db(FixedCompilationDatabase::loadFromCommandLine(compilerArgsCount, compilerArgs.data()));

  // resolve headers to their absolute path
  std::vector<std::string> absoluteHeaders;
  for (auto& header: headers) {
    SmallString<1024> res(header);
    llvm::sys::fs::make_absolute(res);
    absoluteHeaders.push_back(res.str());
  }
 
  // gather named declarations
  ClangTool tool(*db, headers);
  std::set<const NamedDecl*> decls;
  NameCollector nameCollector(absoluteHeaders, decls);
  MatchFinder Finder;
  Finder.addMatcher(MyMatcher, &nameCollector);
  int res(tool.run(newFrontendActionFactory(&Finder).get()));

  // convert them to our local AST
  NamedDeclConverter declConverter;
  for (auto decl: decls) {
    exports.insert(declConverter.Visit(decl));
  }
  // and insert them into the analyser
  boost::copy(declConverter.typeConverter.types | boost::adaptors::map_values, std::inserter(types, types.end()));

  // clean up some things we don't need anymore
  delete db;
  
  return res;
};

