
/**
 * This file contains the definitions for the Transit AST.
 *
 * Read more on http://github.com/samvv/Transit/tree/master/doc/types.md
 */

#ifndef TRANSIT_AST_H_
#define TRANSIT_AST_H_

// FIXME: increase performance by removing these
#include <boost/range.hpp>
#include <boost/any.hpp>

#include <sstream> // for dumping
#include <string>
#include <vector>

namespace transit {

enum class PrimitiveKind {
  Bool,
  Char_U,
  UChar,
  WChar_U,
  Char16,
  Char32,
  UShort,
  UInt,
  ULong,
  ULongLong,
  UInt128,
  Char_S,
  SChar,
  WChar_S,
  Short,
  Int,
  Long,
  LongLong,
  Int128,
  Half,
  Float,
  Double,
  LongDouble,
  NullPtr,
  Void
};

struct Type {

  enum Kind {
    STRUCT,
    UNION,
    ENUM,
    FUNCTION,
    PRIMITIVE,
    POINTER,
    FIXED_ARRAY,
    VARIADIC_ARRAY,
    QUALIFIED
  };

  virtual Kind getKind() const = 0;

  bool isStruct() const { return getKind() == STRUCT; }
  bool isUnion() const { return getKind() == UNION; }
  bool isEnum() const { return getKind() == ENUM; }
  bool isFunction() const { return getKind() == FUNCTION; }
  bool isPrimitive() const { return getKind() == PRIMITIVE; }
  bool isPtr() const { return getKind() == POINTER; }
  bool isFixedArr() const { return getKind() == FIXED_ARRAY; }
  bool isVarArr() const { return getKind() == VARIADIC_ARRAY; }
  bool isQual() const { return getKind() == QUALIFIED; }

  bool isRecord() const {
    return getKind() == STRUCT
      || getKind() == UNION;
  }

  bool isArray() const {
    return getKind() == FIXED_ARRAY 
      || getKind() == VARIADIC_ARRAY;
  }

  virtual std::string toString() const = 0;

};

struct QualType : public Type {
  const Type* underlyingType;
  enum Qualifier {
    CONST, VOLATILE
  };
  std::bitset<2> qualifiers;
  inline bool isConst() const { return qualifiers[CONST]; }
  inline bool isVolatile() const { return qualifiers[VOLATILE]; };
  inline void setConst(bool isConst) { qualifiers[CONST] = isConst; }
  inline void setVolatile(bool isVolatile) { qualifiers[VOLATILE] = isVolatile; };
  Kind getKind() const override { return QUALIFIED; };

  std::string toString() const override {
    std::ostringstream ss;
    ss << "(qualified-type";
    if (isConst())
      ss << " const";
    if (isVolatile())
      ss << " volatile";
    ss << ")";
    return ss.str();
  }

};

struct EnumValue {
  inline EnumValue(std::string name, unsigned long long value): name(name), value(value) {};
  std::string name;
  unsigned long long value;
};

struct EnumType : public Type {

  std::vector<EnumValue> values;
  
  Kind getKind() const override { return ENUM; }

  inline void addValue(std::string name, unsigned long long value) {
    values.push_back(EnumValue(name, value));
  }

  std::string toString() const override { return "(enum-type)"; }
};

struct RecordField {
  inline RecordField(std::string name, Type* type): name(name), type(type) {}
  std::string name;
  Type* type;
};

struct RecordType : public Type {

  std::vector<RecordField> fields;

  inline void addField(std::string name, Type* type) {
    fields.push_back(RecordField { name, type });
  }
};

struct StructType : public RecordType {
  Kind getKind() const override { return STRUCT; }
  std::string toString() const override {
    std::ostringstream oss;
    oss << "(struct-type";
    for (auto& field: fields)
      oss << "(field \"" << field.name << "\" " << field.type->toString() << ")";
    oss << ")";
    return oss.str();;
  }
};

struct UnionType : public RecordType {
  Kind getKind() const override { return UNION; }
  std::string toString() const override { return "(union-type)"; }
};

struct PrimitiveType : public Type {
  Kind getKind() const override { return PRIMITIVE; }
  PrimitiveKind primitiveKind;
  std::string toString() const override {
    std::ostringstream oss;
    oss << "(primitive-type " << (int)primitiveKind << ")";
    return oss.str();
  }
};

struct FunctionType : public Type {
  Kind getKind() const override { return FUNCTION; }
  Type* returnType;
  std::vector<Type*> paramTypes;
  void addParamType(Type* paramType) { paramTypes.push_back(paramType); }
  std::string toString() const override { return "(function-type " + returnType->toString() + ")"; }
};

struct ArrayType : public Type {
  Type* elementType;
};

struct FixedArrayType : public ArrayType {
  size_t count;
  Kind getKind() const override { return FIXED_ARRAY; }
  std::string toString() const override {
    std::ostringstream oss;
    oss << "(fixed-array-type " << count << elementType->toString() << ")";
    return oss.str();
  }
};

struct VariadicArrayType : public ArrayType {
  Kind getKind() const override { return VARIADIC_ARRAY; }
  std::string toString() const override { return "(variadic-array-type " + elementType->toString() + ")"; }
};

struct PointerType : public Type {
  bool isBlockPtr = false;
  Type* referencedType;
  Kind getKind() const override { return POINTER; }
  std::string toString() const override { return "(pointer-type " + referencedType->toString() + ")"; }
};

//std::ostream& operator>>(std::ostream& out, const Type& t) {
  //out << t.toString();
  //return out;
//}

/** Values */

class ValueVisitor;

struct Value {
protected:
  virtual void visit(ValueVisitor& v) = 0;
};

struct RecordValue : public Value {
  std::vector<Value*> fieldValues;
};

struct PrimitiveValue : public Value {
  PrimitiveType::Kind kind;
  boost::any value;
};

/** Exports */

class ExportVisitor;

struct Export {
  friend class ExportVisitor;
  std::string name;
  Type* type;
  Value* value;
};

/** Visitor definitions */

template<typename Derived, typename R = void>
struct TypeVisitor {

  R visit(const Type* type) {
    switch (type->getKind()) {
    case Type::QUALIFIED:
      return static_cast<Derived*>(this)->visitQualType(static_cast<const QualType*>(type));
    case Type::ENUM:
      return static_cast<Derived*>(this)->visitEnumType(static_cast<const EnumType*>(type));
    case Type::UNION:
      return static_cast<Derived*>(this)->visitUnionType(static_cast<const UnionType*>(type));
    case Type::STRUCT:
      return static_cast<Derived*>(this)->visitStructType(static_cast<const StructType*>(type));
    case Type::POINTER:
      return static_cast<Derived*>(this)->visitPointerType(static_cast<const PointerType*>(type));
    case Type::FUNCTION:
      return static_cast<Derived*>(this)->visitFunctionType(static_cast<const FunctionType*>(type));
    case Type::PRIMITIVE:
      return static_cast<Derived*>(this)->visitPrimitiveType(static_cast<const PrimitiveType*>(type));
    case Type::FIXED_ARRAY:
      return static_cast<Derived*>(this)->visitFixedArrayType(static_cast<const FixedArrayType*>(type));
    case Type::VARIADIC_ARRAY:
      return static_cast<Derived*>(this)->visitVariadicArrayType(static_cast<const VariadicArrayType*>(type));
    }
  }
  
  R visitType(const Type* type) { };
  R visitEnumType(const EnumType* type) { return visitType(type); }
  R visitQualType(const QualType* type) { return visitType(type); }
  R visitArrayType(const ArrayType* type) { return visitType(type); }
  R visitRecordType(const RecordType* type) { return visitType(type); }
  R visitPointerType(const PointerType* type) { return visitType(type); }
  R visitFunctionType(const FunctionType* type) { return visitType(type); }
  R visitUnionType(const RecordType* type) { return visitRecordType(type); }
  R visitStructType(const RecordType* type) { return visitRecordType(type); }
  R visitPrimitiveType(const PrimitiveType* type) { return visitType(type); }
  R visitFixedArrayType(const FixedArrayType* type) { return visitArrayType(type); }
  R visitVariadicArrayType(const VariadicArrayType* type) { return visitArrayType(type); }

};

}


#endif

