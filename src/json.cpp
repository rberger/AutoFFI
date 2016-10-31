
#include <iostream>

#include "Transit/Analyser.h"
#include "Transit/Indexer.hpp"

#include "llvm/Support/CommandLine.h"

using namespace llvm;

#define JOIN(values, value, ops) { \
	bool first = true; \
	for (auto value: values) { \
		if (first) first = false; \
		else std::cout << ","; \
		ops; \
	}; }

//static cl::opt<std::string> OutFile("o", cl::desc("Location of output file"));
static cl::list<std::string> Files(cl::Positional, cl::desc("Files to parse"), cl::OneOrMore);

namespace transit {

std::string getCNameOfPrimitiveKind(PrimitiveKind kind) {
  switch (kind) {
	case PrimitiveKind::Bool:
	 return "bool";
	case PrimitiveKind::Char_U:
	 return "char";
	case PrimitiveKind::UChar:
	 return "unsigned char";
	case PrimitiveKind::WChar_U:
	 return "wchar_t";
	case PrimitiveKind::Char16:
	 return "char16_t";
	case PrimitiveKind::Char32:
	 return "char32_t";
	case PrimitiveKind::UShort:
	 return "unsigned short";
	case PrimitiveKind::UInt:
	 return "unsigned int";
	case PrimitiveKind::ULong:
	 return "unsigned long";
	case PrimitiveKind::ULongLong:
	 return "unsigned long long";
	case PrimitiveKind::UInt128:
	 return "__uint128_t";
	case PrimitiveKind::Char_S:
	 return "char";
	case PrimitiveKind::SChar:
	 return "signed char";
	case PrimitiveKind::WChar_S:
	 return "wchar_t";
	case PrimitiveKind::Short:
	 return "short";
	case PrimitiveKind::Int:
	 return "int";
	case PrimitiveKind::Long:
	 return "long";
	case PrimitiveKind::LongLong:
	 return "long long";
	case PrimitiveKind::Int128:
	 return "__int128_t";
	case PrimitiveKind::Half:
	 return "half";
	case PrimitiveKind::Float:
	 return "float";
	case PrimitiveKind::Double:
	 return "double";
	case PrimitiveKind::LongDouble:
	 return "long double";
  case PrimitiveKind::Void:
   return "void";
  default:
    throw std::runtime_error("unsupported primitive type");
  }
}

struct TypeSerializer : public TypeVisitor<TypeSerializer> {

  Indexer<const Type*>& index;
  TypeSerializer(Indexer<const Type*>& index): index(index) {}

  void visitType(const Type* type) {
    throw std::runtime_error("unvisited type");
    std::cerr << "warning: type not converted" << type->getKind() << std::endl;
  }

  void visitFixedArrayType(const FixedArrayType* type) {
    std::cout << "{\"kind\":\"array\",\"id\":" << index.getID(type)
              << "count" << type->count
              << ",\"elementType\":" << index.getID(type->elementType)
              << "}";
  }

  void visitVariadicArrayType(const VariadicArrayType* type) {
    std::cout << "{\"kind\":\"array\",\"id\":" << index.getID(type)
              << ",\"elementType\":" << index.getID(type->elementType) << "}";
  }

  void visitFunctionType(const FunctionType* type) {
    std::cout << "{\"kind\":\"function\",\"id\":" << index.getID(type) << ",\"paramTypes\":[";
    JOIN(type->paramTypes, paramType, std::cout << index.getID(paramType));
    std::cout << "],\"returnType\":" << index.getID(type->returnType) << "}";
  }

	void visitQualType(const QualType* type) {
		std::cout << "{\"kind\":\"qualified\",\"id\":" << index.getID(type) << ",\"qualifiers\":[";
    if (type->isConst())
      std::cout << "\"const\"";
    if (type->isVolatile()) {
      if (type->isConst())
        std::cout << ',';
      std::cout << "\"volatile\"";
    }
    std::cout << "],\"underlyingType\":" << index.getID(type->underlyingType) << "}";
	}	

  void visitPrimitiveType(const PrimitiveType* type) {
		std::cout << "{\"kind\":\"primitive\",\"id\":" << index.getID(type) << ",\"name\":\"" << getCNameOfPrimitiveKind(type->primitiveKind) << "\"}";
  }

  void visitPointerType(const PointerType* type) {
    std::cout << "{\"kind\":\"pointer\",\"id\":\"" << (int)index.getID(type) << ",\"referencedType\":" << (int)index.getID(type->referencedType);
    if (type->isBlockPtr)
      std::cout << ",\"isBlockPtr\":true";
     std::cout << "}";
  }

  void visitUnionType(const UnionType* type) {
    std::cout << "{\"kind\":\"union\",\"id\":" << (int)index.getID(type) << ",\"fields\":[";
		JOIN(type->fields, field, std::cout << '"' << field.name << "\":" << (int)index.getID(field.type));
    std::cout << "]}";
  }

  void visitStructType(const StructType* type) {
    std::cout << "{\"kind\":\"struct\",\"id\":" << (int)index.getID(type) << ",\"fields\":[";
		JOIN(type->fields, field, std::cout << '"' << field.name << "\":" << (int)index.getID(field.type));
    std::cout << "]}";
  }

  void visitEnumType(const EnumType* type) {
    std::cout << "{\"kind\":\"enum\",\"id\":" << (int)index.getID(type) << ",\"values\":[";
    JOIN(type->values, value, std::cout << '"' << value.name << "\":" << value.value);
    std::cout << "]}";
  }

};

void serialize(const Type* type,  Indexer<const Type*>& index) {
  TypeSerializer serializer(index);
  serializer.visit(type);
}

void serialize(const Export* ex, Indexer<const Type*>& index) {
  std::cout << "{\"name\":\"" << ex->name << "\",\"type\":" << index.getID(ex->type) << "}";
}

}

int main(int argc, const char* argv[]) {

  auto sep(std::find_if(argv, argv+argc, [](const char* arg) {
    return !strcmp(arg, "--");
  }));

  std::vector<const char*> compilerArgs(sep == argv+argc ? sep : sep+1, argv+argc);

  cl::ParseCommandLineOptions(sep-argv, argv);
  transit::Analyser a;
  if (int res = a.scan(Files, compilerArgs))
    return res;

  // assign to each type a number
  transit::Indexer<const transit::Type*> typeIndex(a.getTypes());

  // serialize to JSON
  std::cout << "{\"exports\":[";
	JOIN(a.getExports(), ex, serialize(ex, typeIndex));
	std::cout << "],\"types\":[";
	JOIN(a.getTypes(), type, serialize(type, typeIndex));
  std::cout << "]}";
}

