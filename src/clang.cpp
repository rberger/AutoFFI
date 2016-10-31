
#include <iostream> // for debugging only
#include <sstream>

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

using namespace transit;
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

DeclarationMatcher MyMatcher = anyOf(
  typedefDecl().bind("ndecl"),
  typeAliasDecl().bind("ndecl"),
  //typedefDecl().bind("ndecl"),
  functionDecl().bind("ndecl"),
  recordDecl().bind("ndecl"),
  enumDecl().bind("ndecl"),
  varDecl(isConstexpr()).bind("ndecl")
);

struct ClangQualTypeLT {
  bool operator()(const clang::QualType& a, const clang::QualType& b) const {
    return a.getTypePtr() < b.getTypePtr()
			|| ((a.getTypePtr() == b.getTypePtr()) && a.getLocalFastQualifiers() < b.getLocalFastQualifiers());
  } 
};

PrimitiveKind clangBuiltinTypeKindToTransitPrimitiveKind(const BuiltinType* type) {
  switch (type->getKind()) {
  case BuiltinType::Bool:
    return PrimitiveKind::Bool;
  case BuiltinType::Char_U:
    return PrimitiveKind::Char_U;
  case BuiltinType::UChar:
    return PrimitiveKind::UChar;
  case BuiltinType::WChar_U:
    return PrimitiveKind::WChar_U;
  case BuiltinType::Char16:
    return PrimitiveKind::Char16;
  case BuiltinType::Char32:
    return PrimitiveKind::Char32;
  case BuiltinType::UShort:
    return PrimitiveKind::UShort;
  case BuiltinType::UInt:
    return PrimitiveKind::UInt;
  case BuiltinType::ULong:
    return PrimitiveKind::ULong;
  case BuiltinType::ULongLong:
    return PrimitiveKind::ULongLong;
  case BuiltinType::UInt128:
    return PrimitiveKind::UInt128;
  case BuiltinType::Char_S:
    return PrimitiveKind::Char_S;
  case BuiltinType::SChar:
    return PrimitiveKind::SChar;
  case BuiltinType::WChar_S:
    return PrimitiveKind::WChar_S;
  case BuiltinType::Short:
    return PrimitiveKind::Short;
  case BuiltinType::Int:
    return PrimitiveKind::Int;
  case BuiltinType::Long:
    return PrimitiveKind::Long;
  case BuiltinType::LongLong:
    return PrimitiveKind::LongLong;
  case BuiltinType::Int128:
    return PrimitiveKind::Int128;
  case BuiltinType::Half:
    return PrimitiveKind::Half;
  case BuiltinType::Float:
    return PrimitiveKind::Float;
  case BuiltinType::Double:
    return PrimitiveKind::Double;
  case BuiltinType::LongDouble:
    return PrimitiveKind::LongDouble;
  case BuiltinType::NullPtr:
    return PrimitiveKind::NullPtr;
  case BuiltinType::Void:
    return PrimitiveKind::Void;
  default: {
    LangOptions lopts;
    PrintingPolicy popts(lopts);
    throw std::runtime_error("encountered an unsupported primitive type: " + std::string(type->getNameAsCString(popts)));

  }
  }
}

/**
 * Converts an arbitrary Clang type to a Transit type.
 */
struct TypeConverter : public clang::TypeVisitor<TypeConverter, transit::Type*> {

  std::map<const clang::QualType, transit::Type*, ClangQualTypeLT> types;

  transit::Type* VisitType(const clang::Type* type) {
    std::cerr << "warning: could not process the following type:" << std::endl;
    type->dump();
    throw std::runtime_error("unconvertable type");
  }

  transit::Type* VisitTypedefType(const TypedefType* type) {
    // Skip typedefs which are not captured as an export
    auto converted(VisitQualType(type->desugar()));
    // NOTE: I do not have to be added to the map
    //types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  transit::Type* VisitConstantArrayType(const clang::ConstantArrayType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto converted(new transit::FixedArrayType);
    converted->count = type->getSize().getLimitedValue();
    converted->elementType = VisitQualType(type->getElementType());
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  transit::Type* VisitPointerType(const clang::PointerType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto converted(new transit::PointerType);
    converted->referencedType = VisitQualType(type->getPointeeType());
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  transit::Type* VisitBuiltinType(const BuiltinType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto converted(new PrimitiveType);
    converted->primitiveKind = clangBuiltinTypeKindToTransitPrimitiveKind(type);
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  transit::Type* VisitQualType(const clang::QualType& qt) {
    auto match(types.find(qt));
    if (match != types.end())
      return match->second;
    // Skip creation of qualified types where no qualifiers are present
    if (qt.hasLocalQualifiers()) {
      auto converted = new transit::QualType;
      converted->underlyingType = Visit(qt.getTypePtr());
      converted->setConst(qt.isConstQualified());
      converted->setVolatile(qt.isVolatileQualified());
      types.emplace(qt, converted);
      return converted;
    } else {
      auto converted = Visit(qt.getTypePtr());
      // NOTE: I do not have to be added to the map
      types.emplace(qt, converted);
      return converted;
    }
  }

  transit::Type* VisitElaboratedType(const clang::ElaboratedType* type) {
    // Elaborated types are ignored by the AST; their referenced type is used
    auto converted = VisitQualType(type->getNamedType());
    // NOTE: I do not have to be added to the map
    //types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  transit::Type* VisitParenType(const clang::ParenType* type) {
    // Parenhesed types are expanded
    auto converted = VisitQualType(type->desugar());
    // NOTE: I do not have to be added to the map
    //types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  transit::Type* VisitEnumType(const clang::EnumType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto decl(type->getDecl());
    auto converted = new transit::EnumType;
    for (auto value: decl->enumerators())
      converted->addValue(value->getNameAsString(), value->getInitVal().getLimitedValue());
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  transit::Type* VisitRecordType(const clang::RecordType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto decl(type->getDecl());
    transit::RecordType* converted;
    switch (decl->getTagKind()) {
    case TTK_Struct:
    case TTK_Class:
      converted = new transit::StructType;
      break;
    case TTK_Union:
      converted = new transit::UnionType;
      break;
    default:
      std::cerr << "unrecognised record type" << std::endl;
      decl->dump();
      throw std::runtime_error("cuu coo");
      return NULL; // FIXME: I am not properly handled
    }
    for (auto field: decl->fields())
      converted->addField(field->getNameAsString(), VisitQualType(field->getType()));
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  transit::Type* VisitBlockPointerType(const BlockPointerType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto converted = new transit::PointerType;
    converted->isBlockPtr = true;
    converted->referencedType = VisitQualType(type->getPointeeType());
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

	transit::Type* VisitFunctionNoProtoType(const FunctionNoProtoType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto converted = new transit::FunctionType;
    converted->returnType = VisitQualType(type->getReturnType());
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
	};

  transit::Type* VisitFunctionProtoType(const FunctionProtoType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto converted = new transit::FunctionType;
    for (auto paramType: type->param_types())
      converted->addParamType(VisitQualType(paramType));
    converted->returnType = VisitQualType(type->getReturnType());
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

};

struct ValueConverter {

};

/**
 * Converts a named declaration to a Transit symbol export.
 */
struct NamedDeclConverter : public ConstDeclVisitor<NamedDeclConverter, transit::Export*> {

  TypeConverter typeConverter;
  ValueConverter valueConverter;

  transit::Export* VisitTypedefDecl(const TypedefDecl* decl) {
    auto e = new transit::Export;
    e->name = decl->getNameAsString();
    e->type = typeConverter.VisitQualType(decl->getUnderlyingType());
    e->value = NULL;
    return e;
  }

  transit::Export* VisitFunctionDecl(const FunctionDecl* decl) {
    auto e = new transit::Export;
    e->name = decl->getNameAsString();
    e->type = typeConverter.VisitQualType(decl->getType());
    e->value = NULL;
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
    e->type = typeConverter.Visit(decl->getTypeForDecl());
    e->value = NULL;
    return e;
  }

  transit::Export* VisitEnumDecl(const EnumDecl* decl) {
    auto e = new transit::Export;
    e->name = decl->getNameAsString();
    e->type = typeConverter.Visit(decl->getTypeForDecl());
    e->value = NULL;
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
      if (!decl->isImplicit()) {
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

  // convert them to our local AST and insert them into the analyser
  NamedDeclConverter declConverter;
  for (auto decl: decls) {
    exports.insert(declConverter.Visit(decl));
  }
  // as a side-effect of the transformation, all types were converted
  // insert them too
  boost::copy(declConverter.typeConverter.types | boost::adaptors::map_values, std::inserter(types, types.end()));

  // clean up some things we don't need anymore
  delete db;
  
  return res;
};

