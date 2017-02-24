
#include <iostream> // for debugging only
#include <sstream>
#include <string>

#include "config.h"
#include "AutoFFI/AST.h"
#include "AutoFFI/ClangSourceAnalyser.h"
#include "AutoFFI/TypeSorter.h"

#include "clang/AST/Decl.h"
#include "clang/AST/Type.h"
#include "clang/AST/TypeVisitor.h"
#include "clang/AST/DeclVisitor.h"
#include "boost/range.hpp"
#include "llvm/Option/ArgList.h"

#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "Clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
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
#include "clang/Driver/Driver.h"
#include "clang/Driver/Compilation.h"

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

void Dummy() {

}

#include "clang/Basic/TargetInfo.h"
#include "clang/CodeGen/CodeGenAction.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Tool.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/FrontendDiagnostic.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include <memory>

//using namespace clang;
using namespace clang::driver;

// This function isn't referenced outside its translation unit, but it
// can't use the "static" keyword because its address is used for
// GetMainExecutable (since some platforms don't support taking the
// address of main, and some platforms can't implement GetMainExecutable
// without being given the address of a function in the main executable).
std::string GetExecutablePath(const char *Argv0) {
  // This just needs to be some symbol in the binary; C++ doesn't
  // allow taking the address of ::main however.
  void *MainAddr = (void*) (intptr_t) GetExecutablePath;
  return llvm::sys::fs::getMainExecutable(Argv0, MainAddr);
}

using HeaderCollection = std::vector<std::string>;

class EmitAutoFFIAction : public ASTFrontendAction {
  std::set<const NamedDecl*> Decls;
public:
  HeaderCollection& Headers;

  EmitAutoFFIAction(HeaderCollection& Headers): Headers(Headers) {};

  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
      clang::CompilerInstance &Compiler, llvm::StringRef InFile) override {
    NameCollector nameCollector(Headers, Decls);
    MatchFinder Finder;
    Finder.addMatcher(MyMatcher, &nameCollector);
    return Finder.newASTConsumer();
  }
};

#include "llvm/ProfileData/InstrProf.h"

int autoffi::ClangSourceAnalyser::analyse(std::vector<const char*> compilerArgs) {

  compilerArgs.push_back("-fsyntax-only");
  StringRef execDir(llvm::sys::path::remove_leading_dotslash(llvm::sys::path::parent_path(compilerArgs[0])));

  // FIXME: really, there should be a more straightforward way for doing this
  //SmallString<0xfff> v(execDir);
  //std::copy(execDir.begin(), execDir.end(), std::back_inserter(v));
  //llvm::sys::fs::make_absolute(v);

  compilerArgs.insert(compilerArgs.begin()+1, "-isystem./clang/include/");
  compilerArgs.insert(compilerArgs.begin()+1, "-isystem./libcxx/include/");

  void *MainAddr = (void*) (intptr_t) GetExecutablePath;
  std::string Path = GetExecutablePath(compilerArgs[0]);
  IntrusiveRefCntPtr<DiagnosticOptions> DiagOpts = new DiagnosticOptions();
  TextDiagnosticPrinter *DiagClient =
    new TextDiagnosticPrinter(llvm::errs(), &*DiagOpts);

  IntrusiveRefCntPtr<DiagnosticIDs> DiagID(new DiagnosticIDs());
  DiagnosticsEngine Diags(DiagID, &*DiagOpts, DiagClient);

  std::string TripleStr = llvm::sys::getDefaultTargetTriple();
  llvm::Triple T(TripleStr);

  Driver TheDriver(Path, T.str(), Diags);
  TheDriver.setTitle("clang autoffi");
  TheDriver.setCheckInputsExist(false);

  // FIXME: This is a hack to try to force the driver to do something we can
  // recognize. We need to extend the driver library to support this use model
  // (basically, exactly one input, and the operation mode is hard wired).
  //SmallVector<const char *, 16> Args(argv, argv + argc);
  //Args.push_back("--analyze");
  std::unique_ptr<Compilation> C(TheDriver.BuildCompilation(compilerArgs));
  if (!C)
    throw std::runtime_error("could not create dummy compilation");

  // FIXME: This is copied from ASTUnit.cpp; simplify and eliminate.

  // We expect to get back exactly one command job, if we didn't something
  // failed. Extract that job from the compilation.
  const driver::JobList &Jobs = C->getJobs();
  //if (Jobs.size() != 1 || !isa<driver::Command>(*Jobs.begin())) {
    //SmallString<256> Msg;
    //llvm::raw_svector_ostream OS(Msg);
    //Jobs.Print(OS, "; ", true);
    //Diags.Report(diag::err_fe_expected_compiler_job) << OS.str();
    //return 1;
  //}

  const driver::Command &Cmd = cast<driver::Command>(*Jobs.begin());
  std::cout << llvm::StringRef(Cmd.getCreator().getName()).str() << std::endl;
  //if (llvm::StringRef(Cmd.getCreator().getName()) != "clang") {
    //throw std::runtime_error("err_fe_expected_clang_command");
    //Diags.Report(diag::err_fe_expected_clang_command);
    //return 1;
  //}

  //// Initialize a compiler invocation object from the clang (-cc1) arguments.
  const driver::ArgStringList &CCArgs = Cmd.getArguments();
  for (auto& arg: CCArgs) std::cout << arg << std::endl;
  std::unique_ptr<CompilerInvocation> CI(new CompilerInvocation);
  CompilerInvocation::CreateFromArgs(*CI,
                                     const_cast<const char **>(CCArgs.data()),
                                     const_cast<const char **>(CCArgs.data()) +
                                       CCArgs.size(),
                                     Diags);

  // Show the invocation, with -v.
  //if (CI->getHeaderSearchOpts().Verbose) {
    //llvm::errs() << "clang invocation:\n";
    //Jobs.Print(llvm::errs(), "\n", true);

    //llvm::errs() << "\n";
  //}

  // FIXME: This is copied from cc1_main.cpp; simplify and eliminate.
  
  // Create a compiler instance to handle the actual work.
  CompilerInstance Clang;

  // Infer the builtin include path if unspecified.
  if (Clang.getHeaderSearchOpts().UseBuiltinIncludes &&
      Clang.getHeaderSearchOpts().ResourceDir.empty())
    Clang.getHeaderSearchOpts().ResourceDir =
      CompilerInvocation::GetResourcesPath(compilerArgs[0], MainAddr);

  //Clang.getPreprocessorOpts();
  Clang.setInvocation(CI.release());
  Clang.createDiagnostics();
  std::shared_ptr<clang::TargetOptions> pto( new clang::TargetOptions());
  pto->Triple = llvm::sys::getDefaultTargetTriple();
  clang::TargetInfo *pti = clang::TargetInfo::CreateTargetInfo(Clang.getDiagnostics(), pto);
  Clang.setTarget(pti);
  Clang.createFileManager();
  Clang.createSourceManager(Clang.getFileManager());
  Clang.createPreprocessor(TU_Complete);

  std::vector<std::string> Inputs;
  for (FrontendInputFile& f: Clang.getFrontendOpts().Inputs) {
    if (f.isFile())
      Inputs.push_back(f.getFile());
  }

  std::unique_ptr<EmitAutoFFIAction> Act(new EmitAutoFFIAction(Inputs));
  if (!Clang.ExecuteAction(*Act))
    throw std::runtime_error("extraction did not succeed; see errors above");
  
}

