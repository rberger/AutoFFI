
/**
 * This file contains the definitions for the Transit AST.
 *
 * Read more on http://github.com/samvv/Transit/tree/master/doc/types.md
 */

#ifndef AUTOFFI_AST_H_
#define AUTOFFI_AST_H_

// FIXME: increase performance by removing these
#include <boost/range.hpp>
#include <boost/any.hpp>

#include <bitset>
#include <sstream> // for dumping
#include <string>
#include <vector>

namespace autoffi {

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
public:
  enum Qualifier {
    CONST, VOLATILE
  };
private:
  Type* underlyingType;
  std::bitset<2> qualifiers;
public:

  QualType(Type* underlyingType): underlyingType(underlyingType) {}

  inline Type* getUnderlyingType() const { return underlyingType; }

  inline bool isConst() const { return qualifiers[CONST]; }
  inline bool isVolatile() const { return qualifiers[VOLATILE]; };
  inline void setConst(bool isConst) { qualifiers[CONST] = isConst; }
  inline void setVolatile(bool isVolatile) { qualifiers[VOLATILE] = isVolatile; };

  Kind getKind() const override { return QUALIFIED; };

  std::string toString() const override;

};

struct EnumValue {
  inline EnumValue(std::string name, unsigned long long value): name(name), value(value) {};
  std::string name;
  unsigned long long value;
};

struct EnumType : public Type {
private:
  std::vector<EnumValue> values;
public:
  //using value_range = boost::iterator_range<std::vector<EnumValue>::iterator>;
  using const_value_range = boost::iterator_range<std::vector<const EnumValue>::iterator>;
  //inline value_range getValues() { return boost::make_iterator_range(values.begin(), values.end()); }
  inline const_value_range getValues() const { return boost::make_iterator_range(values.begin(), values.end()); }
  
  Kind getKind() const override { return ENUM; }

  //inline bool hasValueNamed(std::string name);
  //inline bool hasValueOf(unsigned long long value);
  void addValue(std::string name, unsigned long long value);
  std::string toString() const override { return "(enum-type)"; }
};

struct RecordField {
  inline RecordField(std::string name, Type* type): name(name), type(type) {}
  std::string name;
  Type* type;
};

class RecordType : public Type {
  std::vector<RecordField> fields;
public:
  //using field_range = boost::iterator_range<std::vector<RecordField>::iterator>;;
  using const_field_range = boost::iterator_range<std::vector<const RecordField>::iterator>;;
  //inline field_range getFields() { return boost::make_iterator_range(fields.begin(), fields.end()); } 
  inline const_field_range getFields() const { return boost::make_iterator_range(fields.begin(), fields.end()); } 

  //bool hasFieldTyped(const Type* type) const;
  //bool hasFieldNamed(const std::string name) const;
  void addField(std::string name, Type* type);
};

struct StructType : public RecordType {
  Kind getKind() const override { return STRUCT; }
  std::string toString() const override;
};

struct UnionType : public RecordType {
  Kind getKind() const override { return UNION; }
  std::string toString() const override { return "(union-type)"; }
};

class PrimitiveType : public Type {
  PrimitiveKind primitiveKind;
public:
  inline PrimitiveType(PrimitiveKind primKind): primitiveKind(primKind) {}
  inline PrimitiveKind getPrimitiveKind() const { return primitiveKind; }
  Kind getKind() const override { return PRIMITIVE; }
  std::string toString() const override;
};

struct FunctionType : public Type {
protected:
  Type* returnType;
  std::vector<Type*> paramTypes;
public:
  inline FunctionType(Type* returnType): returnType(returnType) {}
  using param_type_range = boost::iterator_range<std::vector<Type*>::iterator>;
  using const_param_type_range = boost::iterator_range<std::vector<const Type* const>::iterator>;
  inline param_type_range getParamTypes() { return boost::make_iterator_range(paramTypes.begin(), paramTypes.end()); }
  inline const_param_type_range getParamTypes() const { return boost::make_iterator_range(paramTypes.begin(), paramTypes.end()); }
  inline Type* getReturnType() const { return returnType; }
  void addParamType(Type* paramType);
  Kind getKind() const override { return FUNCTION; }
  std::string toString() const override;
};

class ArrayType : public Type {
protected:
  Type* elementType;
public:
  inline ArrayType(Type* elType): elementType(elType) {};
  inline Type* getElementType() const { return elementType; }
};

class FixedArrayType : public ArrayType {
  size_t count;
  Kind getKind() const override { return FIXED_ARRAY; }
public:
  inline FixedArrayType(Type* elType, size_t count): ArrayType(elType), count(count) {}
  inline size_t getCount() const { return count; }
  std::string toString() const override;
};

class VariadicArrayType : public ArrayType {
public:
  Kind getKind() const override { return VARIADIC_ARRAY; }
  std::string toString() const override;
};

class PointerType : public Type {
public:
  enum PointerKind {
    REGULAR,
    BLOCK
  };
protected:
  PointerKind pointerKind = REGULAR;
  Type* referencedType;
public:
  inline PointerType(Type* referencedType, PointerKind pkind = REGULAR): referencedType(referencedType), pointerKind(pkind) {};
  inline bool isBlockPtr() const { return pointerKind == BLOCK; }
  inline bool isRegularPtr() const { return pointerKind == REGULAR; }
  inline Type* getReferencedType() const { return referencedType; }
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

class Export {
  std::string name;
  Type* type;
  Value* value;
public:
  inline Export(std::string name, Type* type, Value* value = NULL):
    name(name), type(type), value(value) {}
  inline std::string getName() const { return name; }
  inline Type* getType() const { return type; }
  inline Value* getValue() const { return value; }
};

/** Visitor definitions */

template<typename Derived, typename R = void>
struct TypeVisitor {

  R visit(Type* type) {
    switch (type->getKind()) {
    case Type::QUALIFIED:
      return static_cast<Derived*>(this)->visitQualType(static_cast<QualType*>(type));
    case Type::ENUM:
      return static_cast<Derived*>(this)->visitEnumType(static_cast<EnumType*>(type));
    case Type::UNION:
      return static_cast<Derived*>(this)->visitUnionType(static_cast<UnionType*>(type));
    case Type::STRUCT:
      return static_cast<Derived*>(this)->visitStructType(static_cast<StructType*>(type));
    case Type::POINTER:
      return static_cast<Derived*>(this)->visitPointerType(static_cast<PointerType*>(type));
    case Type::FUNCTION:
      return static_cast<Derived*>(this)->visitFunctionType(static_cast<FunctionType*>(type));
    case Type::PRIMITIVE:
      return static_cast<Derived*>(this)->visitPrimitiveType(static_cast<PrimitiveType*>(type));
    case Type::FIXED_ARRAY:
      return static_cast<Derived*>(this)->visitFixedArrayType(static_cast<FixedArrayType*>(type));
    case Type::VARIADIC_ARRAY:
      return static_cast<Derived*>(this)->visitVariadicArrayType(static_cast<VariadicArrayType*>(type));
    }
  }
  
  R visitType(Type* type) { };
  R visitEnumType(EnumType* type) { return visitType(type); }
  R visitQualType(QualType* type) { return visitType(type); }
  R visitArrayType(ArrayType* type) { return visitType(type); }
  R visitRecordType(RecordType* type) { return visitType(type); }
  R visitPointerType(PointerType* type) { return visitType(type); }
  R visitFunctionType(FunctionType* type) { return visitType(type); }
  R visitUnionType(RecordType* type) { return visitRecordType(type); }
  R visitStructType(RecordType* type) { return visitRecordType(type); }
  R visitPrimitiveType(PrimitiveType* type) { return visitType(type); }
  R visitFixedArrayType(FixedArrayType* type) { return visitArrayType(type); }
  R visitVariadicArrayType(VariadicArrayType* type) { return visitArrayType(type); }

};

// FIXME: there should be a standard way to remove this code duplication
template<typename Derived, typename R = void>
struct ConstTypeVisitor {

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
  R visitUnionType(const UnionType* type) { return visitRecordType(type); }
  R visitStructType(const StructType* type) { return visitRecordType(type); }
  R visitPrimitiveType(const PrimitiveType* type) { return visitType(type); }
  R visitFixedArrayType(const FixedArrayType* type) { return visitArrayType(type); }
  R visitVariadicArrayType(const VariadicArrayType* type) { return visitArrayType(type); }

};

}


#endif

