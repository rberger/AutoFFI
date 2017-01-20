
#include <iostream> // for debugging only
#include <sstream>

#include "config.h"
#include "AutoFFI/AST.h"
#include "AutoFFI/ClangSourceAnalyser.h"
#include "AutoFFI/TypeSorter.h"

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

using namespace autoffi;
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
struct TypeConverter : public clang::TypeVisitor<TypeConverter, autoffi::Type*> {

  std::map<const clang::QualType, autoffi::Type*, ClangQualTypeLT> types;

  autoffi::Type* VisitType(const clang::Type* type) {
    std::cerr << "warning: could not process the following type:" << std::endl;
    type->dump();
    throw std::runtime_error("unconvertable type");
  }

  autoffi::Type* VisitTypedefType(const TypedefType* type) {
    // Skip typedefs which are not captured as an export
    auto converted(VisitQualType(type->desugar()));
    // NOTE: I do not have to be added to the map
    //types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  autoffi::Type* VisitConstantArrayType(const clang::ConstantArrayType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto count = type->getSize().getLimitedValue();
    auto elType = VisitQualType(type->getElementType());
    auto converted(new autoffi::FixedArrayType(elType, count));
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  autoffi::Type* VisitPointerType(const clang::PointerType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto referencedType = VisitQualType(type->getPointeeType());
    auto converted(new autoffi::PointerType(referencedType));
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  autoffi::Type* VisitBuiltinType(const BuiltinType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto primKind = clangBuiltinTypeKindToTransitPrimitiveKind(type);
    auto converted(new PrimitiveType(primKind));
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  autoffi::Type* VisitQualType(const clang::QualType& qt) {
    auto match(types.find(qt));
    if (match != types.end())
      return match->second;
    // Skip creation of qualified types where no qualifiers are present
    if (qt.hasLocalQualifiers()) {
      auto underlyingType = Visit(qt.getTypePtr());
      auto converted = new autoffi::QualType(underlyingType);
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

  autoffi::Type* VisitElaboratedType(const clang::ElaboratedType* type) {
    // Elaborated types are ignored by the AST; their referenced type is used
    auto converted = VisitQualType(type->getNamedType());
    // NOTE: I do not have to be added to the map
    //types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  autoffi::Type* VisitParenType(const clang::ParenType* type) {
    // Parenhesed types are expanded
    auto converted = VisitQualType(type->desugar());
    // NOTE: I do not have to be added to the map
    //types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  autoffi::Type* VisitEnumType(const clang::EnumType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto decl(type->getDecl());
    auto converted = new autoffi::EnumType;
    for (auto value: decl->enumerators())
      converted->addValue(value->getNameAsString(), value->getInitVal().getLimitedValue());
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

  autoffi::Type* VisitRecordType(const clang::RecordType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto decl(type->getDecl());
    autoffi::RecordType* converted;
    switch (decl->getTagKind()) {
    case TTK_Struct:
    case TTK_Class:
      converted = new autoffi::StructType;
      break;
    case TTK_Union:
      converted = new autoffi::UnionType;
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

  autoffi::Type* VisitBlockPointerType(const BlockPointerType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto referencedType = VisitQualType(type->getPointeeType());
    auto converted = new autoffi::PointerType(referencedType, autoffi::PointerType::BLOCK);
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

	autoffi::Type* VisitFunctionNoProtoType(const FunctionNoProtoType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto returnType = VisitQualType(type->getReturnType());
    auto converted = new autoffi::FunctionType(returnType);
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
	};

  autoffi::Type* VisitFunctionProtoType(const FunctionProtoType* type) {
    auto match(types.find(clang::QualType(type, 0)));
    if (match != types.end())
      return match->second;
    auto returnType = VisitQualType(type->getReturnType());
    auto converted = new autoffi::FunctionType(returnType);
    for (auto paramType: type->param_types())
      converted->addParamType(VisitQualType(paramType));
    types.emplace(clang::QualType(type, 0), converted);
    return converted;
  }

};

struct ValueConverter {

};

/**
 * Converts a named declaration to a Transit symbol export.
 */
struct NamedDeclConverter : public ConstDeclVisitor<NamedDeclConverter, autoffi::Export*> {

  TypeConverter typeConverter;
  ValueConverter valueConverter;

  autoffi::Export* VisitTypedefDecl(const TypedefDecl* decl) {
    auto name = decl->getNameAsString();
    auto type = typeConverter.VisitQualType(decl->getUnderlyingType());
    return new autoffi::Export(name, type);
  }

  autoffi::Export* VisitFunctionDecl(const FunctionDecl* decl) {
    auto name = decl->getNameAsString();
    auto type = typeConverter.VisitQualType(decl->getType());
    return new autoffi::Export(name, type);
  };

  autoffi::Export* VisitVarDecl(const VarDecl* decl) {
    auto name = decl->getNameAsString();
    auto type = typeConverter.VisitQualType(decl->getType());
    //auto value = new autoffi::Primti;
    return new autoffi::Export(name, type);
  }

  autoffi::Export* VisitRecordDecl(const RecordDecl* decl) {
    auto name = decl->getNameAsString();
    auto type = typeConverter.Visit(decl->getTypeForDecl());
    return new autoffi::Export(name, type);
  }

  autoffi::Export* VisitEnumDecl(const EnumDecl* decl) {
    auto name = decl->getNameAsString();
    auto type = typeConverter.Visit(decl->getTypeForDecl());
    return new autoffi::Export(name, type);
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

int autoffi::ClangSourceAnalyser::analyse(std::vector<std::string> headers, std::vector<const char*> compilerArgs) {

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
    catalog.addExport(declConverter.Visit(decl));
  }
  // as a side-effect of the transformation, all types were converted
  // perform a topological sort on them
  for (auto type: declConverter.typeConverter.types | boost::adaptors::map_values) std::cerr << type << std::endl; std::cerr << std::endl;
  TypeSorter sorted(declConverter.typeConverter.types | boost::adaptors::map_values);
  // and add them to the catalog, in order
  for (auto type: sorted)
    catalog.addType(type);
  for (auto type: catalog.getTypes()) std::cerr << type << std::endl; std::cerr << std::endl;

  // clean up some things we don't need anymore
  delete db;
  
  return res;
};

