
#include "Transit/TypeSorter.h"

#include "boost/graph/topological_sort.hpp"

namespace transit {

void TypeSorter::addDependency(Type* a, Type* b) {
  auto u(getVertex(a)), v(getVertex(b));
  if (!boost::edge(u, v, g).second) {
    //std::cerr << getVertex(a) << " " << getVertex(b) << std::endl;
    boost::add_edge(getVertex(b), getVertex(a), g);
  }
}

void TypeSorter::visitFunctionType(FunctionType* type) {
  addDependency(type, type->getReturnType());
  for (auto paramType: type->getParamTypes())
    addDependency(type, paramType);
}

void TypeSorter::visitQualType(QualType* type) {
  addDependency(type, type->getUnderlyingType());
}

void TypeSorter::visitPointerType(PointerType* type) {
  addDependency(type, type->getReferencedType());
}

void TypeSorter::visitArrayType(ArrayType* type) {
  addDependency(type, type->getElementType());
}

void TypeSorter::visitRecordType(RecordType* type) {
  for (auto field: type->getFields())
    addDependency(type, field.type);
}

//void TypeSorter::addType(Type* type) {
  //vertexMap.insert(VertexMap::value_type(type, boost::add_vertex(g)));
  //visit(type);
//}

void TypeSorter::sort() {
  std::deque<Graph::vertex_descriptor> sorted;
  boost::topological_sort(g, std::front_inserter(sorted));
  for (auto v: sorted) {
    push_back(g[v].type);
  }
}

}

