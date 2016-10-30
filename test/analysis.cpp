
#include "gtest/gtest.h"

#include "Transit/Analyser.h"

using namespace transit;

TEST(canAnalyse, typedefs) {
  Analyser a;
  ASSERT_EQ(a.scan({"resources/primitiveTypedefs.hpp"}), 0);
  auto& exports = a.getExports();
  ASSERT_EQ(exports.size(), 4);
  auto it(exports.begin());
  ASSERT_EQ((*it)->name, "foo");  
  ASSERT_EQ((*it)->type->kind, Type::PRIMITIVE);
  ASSERT_EQ(static_cast<PrimitiveType*>((*it)->type)->primitiveKind, PrimitiveType::UInt);
  ++it;
  ASSERT_EQ((*it)->name, "bar");  
  ASSERT_EQ((*it)->type->kind, Type::PRIMITIVE);
  ASSERT_EQ(static_cast<PrimitiveType*>((*it)->type)->primitiveKind, PrimitiveType::SChar);
  ++it;
  ASSERT_EQ((*it)->name, "baz");  
  ASSERT_EQ((*it)->type->kind, Type::PRIMITIVE);
  ASSERT_EQ(static_cast<PrimitiveType*>((*it)->type)->primitiveKind, PrimitiveType::Float);
  ++it;
  ASSERT_EQ((*it)->name, "bax");  
  ASSERT_EQ((*it)->type->kind, Type::PRIMITIVE);
  ASSERT_EQ(static_cast<PrimitiveType*>((*it)->type)->primitiveKind, PrimitiveType::LongLong);
}

TEST(canAnalyse, sturctOfPrimitives) {
  Analyser a;
  ASSERT_EQ(a.scan({"resources/structOfPrimitives.hpp"}), 0);
  auto& exports = a.getExports();
  ASSERT_EQ(exports.size(), 1);
  auto testStruct(*exports.begin());
  ASSERT_EQ(testStruct->name, "Test");
}

