
#include "gtest/gtest.h"

#include "AutoFFI/AST.h"

using namespace autoffi;

TEST(TypeTests, replaceTypeInStruct) {
  PrimitiveType b1(PrimitiveKind::Char_U);
  PrimitiveType b2(PrimitiveKind::Bool);
  PointerType ptr(&b1);
  StructType s;
  s.addField("test", &ptr);
  s.replace(&b1, &b2);
  ASSERT_TRUE(dynamic_cast<PointerType*>(s.fields[0].type)->referencedType == &b2);
}


TEST(TypeTests, replaceTypeInFunction) {
  PrimitiveType b1(PrimitiveKind::Char_U);
  PrimitiveType b2(PrimitiveKind::Bool);
  PointerType ptr(&b1);
  FunctionType f(&b1);
  f.addParamType(&ptr);
  f.replace(&b1, &b2);
  ASSERT_TRUE(dynamic_cast<PointerType*>(f.paramTypes[0])->referencedType == &b2);
  ASSERT_TRUE(dynamic_cast<PrimitiveType*>(f.returnType) == &b2);
}

