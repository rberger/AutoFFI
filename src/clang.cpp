
#include <iostream> // for debugging only
#include <sstream>
#include <string>

#include "config.h"
#include "AutoFFI/AST.h"
#include "AutoFFI/ClangSourceAnalyser.h"

#include "clang/AST/Decl.h"
#include "clang/AST/Type.h"
#include "clang/AST/TypeVisitor.h"
#include "clang/AST/DeclVisitor.h"
#include "boost/range.hpp"
#include "llvm/Option/ArgList.h"

#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"
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

#include <boost/filesystem.hpp>
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

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>

namespace mi = boost::multi_index;

struct ClangType {};
struct TypeSeq {};

struct TypeRepr {
  TypeRepr(clang::QualType clang, autoffi::Type* autoffi): clang(clang), autoffi(autoffi) {}
  clang::QualType clang;
  autoffi::Type* autoffi;
  bool dangling = false;
};

typedef mi::multi_index_container<
  TypeRepr,
  mi::indexed_by<
    mi::sequenced < mi::tag <TypeSeq > >,
    mi::ordered_unique< 
      mi::tag<ClangType>, 
      mi::member< TypeRepr, clang::QualType, &TypeRepr::clang >,
      ClangQualTypeLT
    >
  > 
> TypeSet;

/**
 * Converts an arbitrary Clang type to a Transit type.
 */
struct TypeConverter : public clang::TypeVisitor<TypeConverter, autoffi::Type*> {

  //std::map<const clang::QualType, autoffi::Type*, ClangQualTypeLT> types;
  TypeSet types;

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
    auto match(types.get<ClangType>().find(clang::QualType(type, 0)));
    if (match != types.get<ClangType>().end())
      return match->autoffi;
    auto count = type->getSize().getLimitedValue();
    auto converted(new autoffi::FixedArrayType(NULL, count));
    types.get<TypeSeq>().push_back(TypeRepr(clang::QualType(type, 0), converted));
    converted->elementType = VisitQualType(type->getElementType());;
    return converted;
  }

  autoffi::Type* VisitPointerType(const clang::PointerType* type) {
    auto match(types.get<ClangType>().find(clang::QualType(type, 0)));
    if (match != types.get<ClangType>().end())
      return match->autoffi;
    auto converted(new autoffi::PointerType(NULL));
    types.get<TypeSeq>().push_back(TypeRepr(clang::QualType(type, 0), converted));
    converted->referencedType = VisitQualType(type->getPointeeType());
    return converted;
  }

  autoffi::Type* VisitBuiltinType(const BuiltinType* type) {
    auto match(types.get<ClangType>().find(clang::QualType(type, 0)));
    if (match != types.get<ClangType>().end())
      return match->autoffi;
    auto primKind = clangBuiltinTypeKindToTransitPrimitiveKind(type);
    auto converted(new PrimitiveType(primKind));
    types.get<TypeSeq>().push_back(TypeRepr(clang::QualType(type, 0), converted));
    return converted;
  }

  autoffi::Type* VisitQualType(const clang::QualType& qt) {
    auto match(types.get<ClangType>().find(qt));
    if (match != types.get<ClangType>().end())
      return match->autoffi;
    // Skip creation of qualified types where no qualifiers are present
    if (qt.hasLocalQualifiers()) {
      auto converted = new autoffi::QualType(NULL);
      converted->setConst(qt.isConstQualified());
      converted->setVolatile(qt.isVolatileQualified());
      types.get<TypeSeq>().push_back(TypeRepr(qt, converted));
      converted->underlyingType = Visit(qt.getTypePtr());
      return converted;
    } else {
      auto converted = Visit(qt.getTypePtr());
      // NOTE: I do not have to be added to the map
      types.get<TypeSeq>().push_back(TypeRepr(qt, converted));
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
    auto match(types.get<ClangType>().find(clang::QualType(type, 0)));
    if (match != types.get<ClangType>().end())
      return match->autoffi;
    auto decl(type->getDecl());
    auto converted = new autoffi::EnumType;
    for (auto value: decl->enumerators())
      converted->addValue(value->getNameAsString(), value->getInitVal().getLimitedValue());
    types.get<TypeSeq>().push_back(TypeRepr(clang::QualType(type, 0), converted));
    return converted;
  }

  autoffi::Type* VisitRecordType(const clang::RecordType* type) {
    auto match(types.get<ClangType>().find(clang::QualType(type, 0)));
    if (match != types.get<ClangType>().end())
      return match->autoffi;
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
    types.get<TypeSeq>().push_back(TypeRepr(clang::QualType(type, 0), converted));
    for (auto field: decl->fields())
      converted->addField(field->getNameAsString(), VisitQualType(field->getType()));
    return converted;
  }

  autoffi::Type* VisitBlockPointerType(const BlockPointerType* type) {
    auto match(types.get<ClangType>().find(clang::QualType(type, 0)));
    if (match != types.get<ClangType>().end())
      return match->autoffi;
    auto converted = new autoffi::PointerType(NULL, autoffi::PointerType::BLOCK);
    types.get<TypeSeq>().push_back(TypeRepr(clang::QualType(type, 0), converted));
    converted->referencedType = VisitQualType(type->getPointeeType());
    return converted;
  }

	autoffi::Type* VisitFunctionNoProtoType(const FunctionNoProtoType* type) {
    auto match(types.get<ClangType>().find(clang::QualType(type, 0)));
    if (match != types.get<ClangType>().end())
      return match->autoffi;
    auto converted = new autoffi::FunctionType(NULL);
    types.get<TypeSeq>().push_back(TypeRepr(clang::QualType(type, 0), converted));
    converted->returnType = VisitQualType(type->getReturnType());
    return converted;
	};

  autoffi::Type* VisitFunctionProtoType(const FunctionProtoType* type) {
    auto match(types.get<ClangType>().find(clang::QualType(type, 0)));
    if (match != types.get<ClangType>().end())
      return match->autoffi;
    auto converted = new autoffi::FunctionType(NULL);
    types.get<TypeSeq>().push_back(TypeRepr(clang::QualType(type, 0), converted));
    converted->returnType = VisitQualType(type->getReturnType());
    for (auto paramType: type->param_types())
      converted->addParamType(VisitQualType(paramType));
    return converted;
  }

};

struct ValueConverter {

};

using ExportList = std::list<Export*>;

struct make_dangling {
  void operator()(TypeRepr& repr) {
    repr.dangling = true;
  }
};

/**
 * Converts a named declaration to a Transit symbol export.
 */
struct ExportCollector : public ConstDeclVisitor<ExportCollector> {

  TypeConverter typeConverter;
  ValueConverter valueConverter;
  ExportList exports;

  void replaceType(autoffi::Type* src, autoffi::Type* dest) {

    for (auto& ex: exports) {
      if (ex->type == src) {
        ex->type = dest;
      }
    }

    auto& typeSeq(typeConverter.types.get<TypeSeq>());
    for (auto it = typeSeq.begin(); it != typeSeq.end(); ++it) {
      auto& repr = *it;
      if (repr.autoffi == src) {
        typeSeq.modify(it, make_dangling());
      } else {
        TypeRepr newRepr(repr.clang, repr.autoffi);
        newRepr.autoffi->replace(src, dest);
        typeSeq.replace(it, newRepr);
      }
    }
  }

  void VisitTypedefDecl(const TypedefDecl* decl) {
    auto name = decl->getNameAsString();
    auto type = typeConverter.VisitQualType(decl->getUnderlyingType());
    exports.push_back(new autoffi::Export(name, type));
  }

  void VisitFunctionDecl(const FunctionDecl* decl) {
    auto name = decl->getNameAsString();
    auto type = typeConverter.VisitQualType(decl->getType());
    exports.push_back(new autoffi::Export(name, type));
  };

  void VisitVarDecl(const VarDecl* decl) {
    auto name = decl->getNameAsString();
    auto type = typeConverter.VisitQualType(decl->getType());
    //auto value = new autoffi::Primti;
    exports.push_back(new autoffi::Export(name, type));
  }

  void VisitRecordDecl(const RecordDecl* decl) {

    auto name = decl->getNameAsString();
    auto type = typeConverter.Visit(decl->getTypeForDecl());

    if (!decl->isFirstDecl()) {
      auto prev(decl->getPreviousDecl());
      auto prevType = typeConverter.Visit(prev->getTypeForDecl());
      replaceType(prevType, type);
    }

    exports.push_back(new autoffi::Export(name, type));
  }

  void VisitEnumDecl(const EnumDecl* decl) {
    auto name = decl->getNameAsString();
    auto type = typeConverter.Visit(decl->getTypeForDecl());
    exports.push_back(new autoffi::Export(name, type));
  }

};

// http://stackoverflow.com/a/15549954/1173521
bool path_contains_file(boost::filesystem::path filter, boost::filesystem::path file)
{
  
  // If dir ends with "/" and isn't the root directory, then the final
  // component returned by iterators will include "." and will interfere
  // with the std::equal check below, so we strip it before proceeding.
  if (filter.filename() == ".")
    filter.remove_filename();

  // If filter is pointing to a single file, we just have to resolve symlinks
  // and check if it is the same path.
  if (filter.has_filename())
    return boost::filesystem::canonical(filter) == boost::filesystem::canonical(file);

  // We're also not interested in the file's name.
  if (file.has_filename())
    file.remove_filename();

  // If dir has more components than file, then file can't possibly
  // reside in dir.
  auto dir_len = std::distance(filter.begin(), filter.end());
  auto file_len = std::distance(file.begin(), file.end());
  if (dir_len > file_len)
    return false;

  // This stops checking when it reaches dir.end(), so it's OK if file
  // has more directory components afterward. They won't be checked.
  return std::equal(filter.begin(), filter.end(), file.begin());
}

class NameCollector : public MatchFinder::MatchCallback {
public:
  std::set<const NamedDecl*>& decls;
  std::vector<std::string>& headers;
  const FilterOptions& filter;
  NameCollector(std::vector<std::string>& headers, std::set<const NamedDecl*>& decls, const FilterOptions& filter): 
    headers(headers), decls(decls), filter(filter) {}

  bool isLocationValid(const FullSourceLoc& loc) {
    if (!loc.isValid() || loc.isInSystemHeader())
      return false;
    auto& sourceManager(loc.getManager());
    auto locationHeader(boost::filesystem::path(sourceManager.getFilename(loc)));
    for (auto header: filter.includes) {
      if (path_contains_file(boost::filesystem::path(header), locationHeader))
        return true;
    }
    return false;
    //return !mngr.getIncludeLoc(mngr.getFileID(loc)).isValid();
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

class GatherDeclsAction : public ASTFrontendAction {
public:
  std::set<const NamedDecl*> Decls;
  MatchFinder Finder;
  NameCollector Collector;

  GatherDeclsAction(HeaderCollection& Headers, const FilterOptions& Filter): Collector(Headers, Decls, Filter) {
    Finder.addMatcher(MyMatcher, &Collector);
  }

  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
      clang::CompilerInstance &Compiler, llvm::StringRef InFile) override {
    return Finder.newASTConsumer();
  }
};

#include "llvm/ProfileData/InstrProf.h"
#include "AutoFFI/filesystem.h"
#include "AutoFFI/TypeSorter.h"

int autoffi::ClangSourceAnalyser::analyse(const AnalyzeOptions& opts) {

  auto& Filter(opts.filter);

  auto exePath(executable_path(opts.exePath.c_str()));
  auto exeDir(boost::filesystem::path(exePath).parent_path());

  std::vector<const char*> compilerArgs { opts.exePath.c_str() };
  std::transform(opts.compilerArgs.begin(), opts.compilerArgs.end(), 
      std::back_inserter(compilerArgs), [](const std::string& arg) { return arg.c_str(); });

  //StringRef exeDir(llvm::sys::path::remove_leading_dotslash(llvm::sys::path::parent_path(compilerArgs[0])));

  //// FIXME: really, there should be a more straightforward way for doing this
  //SmallString<0xfff> v(exeDir);
  //llvm::sys::fs::make_absolute(v);

  auto libcxxDir(exeDir/"libcxx"/"include");
  auto glibcDir(exeDir/"glibc"/"include");
  auto clangDir(exeDir/"clang"/"include");

  compilerArgs.push_back("-fsyntax-only");
  compilerArgs.insert(compilerArgs.begin()+1, libcxxDir.c_str());
  compilerArgs.insert(compilerArgs.begin()+1, "-isystem");
  compilerArgs.insert(compilerArgs.begin()+1, clangDir.c_str());
  compilerArgs.insert(compilerArgs.begin()+1, "-isystem");

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

	for (auto& Job: Jobs) {

		const driver::Command &Cmd = cast<driver::Command>(Job);
		if (llvm::StringRef(Cmd.getCreator().getName()) != "clang") {
			throw std::runtime_error("err_fe_expected_clang_command");
			Diags.Report(diag::err_fe_expected_clang_command);
			return 1;
		}

		//// Initialize a compiler invocation object from the clang (-cc1) arguments.
		const driver::ArgStringList &CCArgs = Cmd.getArguments();
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

		std::unique_ptr<GatherDeclsAction> Act(new GatherDeclsAction(Inputs, Filter));
		if (!Clang.ExecuteAction(*Act))
			throw std::runtime_error("extraction did not succeed; see errors above");
	
		// convert and catalog the gathered types and exports
    // the types are topologically sorted
		
		ExportCollector Collector;
		for (auto& decl: Act->Decls)
			Collector.Visit(decl);
    
    std::list<autoffi::Type*> types;
    for (auto& repr: Collector.typeConverter.types.get<TypeSeq>())
      if (!repr.dangling)
        types.push_back(repr.autoffi);
    TypeSorter sorted(types);

    for (auto& ex: Collector.exports)
      catalog.addExport(ex);
		for (auto& type: sorted)
			catalog.addType(type);

	}

  return 0;
}

