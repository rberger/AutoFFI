
/**
 * This file contains the definitions for the Truss AST. You can parse this
 * file using bstruct or AutoFFI so that you can link to it in your own
 * applications.
 */

#ifndef TRANSIT_AST_H_
#define TRANSIT_AST_H_

// FIXME: increase performance by removing these
#include <boost/range.hpp>
#include <boost/any.hpp>

#include <string>
#include <vector>

namespace transit {

//class TypeVisitor;

struct Type {
  enum Kind {
    RECORD,
    ENUM,
    FUNCTION,
    PRIMITIVE,
    POINTER,
    ARRAY,
    QUALIFIED
  } kind;
};

struct QualType : public Type {
  const Type* underlyingType;
  QualType(const Type* underlyingType): underlyingType(underlyingType) {};
  enum Qualifier {
    CONST, VOLATILE
  };
  std::bitset<2> qualifiers;
  inline bool isConst() { return qualifiers[CONST]; }
  inline bool isVolatile() { return qualifiers[VOLATILE]; };
  inline void setConst(bool isConst) { qualifiers[CONST] = isConst; }
  inline void setVolatile(bool isVolatile) { qualifiers[VOLATILE] = isVolatile; };
};

struct EnumValue {
  inline EnumValue(std::string name, unsigned long long value): name(name), value(value) {};
  std::string name;
  unsigned long long value;
};

struct EnumType : public Type {
  std::vector<EnumValue> values;
  inline void addValue(std::string name, unsigned long long value) {
    values.push_back(EnumValue(name, value));
  }
};

struct RecordField {
  inline RecordField(std::string name, Type* type): name(name), type(type) {}
  std::string name;
  Type* type;
};

struct RecordType : public Type {

  std::vector<RecordField> fields;

  enum Kind {
    STRUCT, UNION
  } recordType;

  bool isStruct() const { return recordType == STRUCT; }
  bool isUnion() const { return recordType == UNION; }

  inline void addField(std::string name, Type* type) {
    fields.push_back(RecordField { name, type });
  }
};

struct PrimitiveType : public Type {
  enum PrimtiveKind {
    Bit,
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
    LongDouble
  } primitiveKind;
};

struct FunctionType : public Type {

};

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

class ExportVisitor;

struct Export {
  friend class ExportVisitor;
  std::string name;
  Type* type;
  Value* value;
};

/** Visitor definitions */

//template<typename Derived, typename R = void> 
//class TypeVisitor {
  //void visitFunctionType(FunctionType* type) { Derived::visitType(type); }
  //void visitRecordType(RecordType* type) { Derived::visitType(type); }
  //void visitEnumType(EnumType* type) { Derived::visitType(type); }
//};

}


#endif

