
#include "AutoFFI/AST.h"

#include <iostream>
#include <set>

namespace autoffi {

void EnumType::addValue(std::string name, unsigned long long value) {
  values.push_back(EnumValue(name, value));
}

void RecordType::addField(std::string name, Type* type) {
  fields.push_back(RecordField { name, type });
}

void FunctionType::addParamType(Type* paramType) {
  paramTypes.push_back(paramType); 
}

std::string PrimitiveType::toString() const {
  std::ostringstream oss;
  oss << "(primitive-type " << (int)primitiveKind << ")";
  return oss.str();
}

std::string StructType::toString() const {
  std::ostringstream oss;
  oss << "(struct-type";
  for (auto& field: getFields())
    oss << "(field \"" << field.name << "\" " << field.type->toString() << ")";
  oss << ")";
  return oss.str();;
}

std::string FunctionType::toString() const {
  return "(function-type " + returnType->toString() + ")";
}

std::string VariadicArrayType::toString() const {
  return "(variadic-array-type " + elementType->toString() + ")";
}

std::string FixedArrayType::toString() const {
  std::ostringstream oss; 
  oss << "(fixed-array-type " << count << " " << elementType->toString() << ")";
  return oss.str();
}

std::string QualType::toString() const {
  std::ostringstream ss;
  ss << "(qualified-type";
  if (isConst())
    ss << " const";
  if (isVolatile())
    ss << " volatile";
  ss << underlyingType->toString();
  ss << ")";
  return ss.str();
}


class ReplaceVisitor : public TypeVisitor<ReplaceVisitor> {
public:

  Type* src;
  Type* dest;

  ReplaceVisitor(Type* src, Type* dest): src(src), dest(dest) {}

  std::set<Type*> visited;

  void visitFunctionType(FunctionType* type) {
    if (visited.count(type) > 0)
      return;
    visited.emplace(type);
    if (type->returnType == src)
      type->returnType = dest;
    for (auto& paramType: type->paramTypes) {
      if (paramType == src) {
        paramType = dest;
      } else {
        visit(paramType);
      }
    }
  }

  void visitArrayType(ArrayType* type) {
    if (visited.count(type) > 0)
      return;
    visited.emplace(type);
    if (type->elementType == src)
      type->elementType = dest;
    else
      visit(type->elementType);
  }

  void visitPointerType(PointerType* type) {
    if (visited.count(type) > 0)
      return;
    visited.emplace(type);
    if (type->referencedType == src)
      type->referencedType = dest;
    else
      visit(type->referencedType);
  }

  void visitRecordType(RecordType* type) {
    if (visited.count(type) > 0)
      return;
    visited.emplace(type);
    for (auto& field: type->fields) {
      if (field.type == src)
        field.type = dest;
      else
        visit(field.type); 
    }
  }

};

void Type::replace(Type* src, Type* dest) {
  ReplaceVisitor replacer(src, dest);
  replacer.visit(this);
}

}
