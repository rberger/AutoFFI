
#include "Transit/AST.h"

namespace transit {

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

}
